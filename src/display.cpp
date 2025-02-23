#include <gdal/gdal_priv.h>
#include <opencv4/opencv2/opencv.hpp>  
#include <curl/curl.h>
#include <nlohmann/json.hpp> 
#include <cmath>
#include <math.h>

#include "map.hpp"
#include "geography.hpp"
#include "mapDisplay.hpp"

MapDisplay::~MapDisplay()
{
    GDALClose(dataset);
}
MapDisplay::MapDisplay(Map map)
{
    filename="../data/"+map.getFile();

    dataset = (GDALDataset *) GDALOpen(filename.c_str(), GA_ReadOnly);

    if (dataset == nullptr) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        //error
    }

    int width = dataset->GetRasterXSize();
    int height = dataset->GetRasterYSize();
    int bands = dataset->GetRasterCount();

    if (bands < 3) {
        std::cerr << "This image does not have 3 bands (RGB)." << std::endl;
        GDALClose(dataset);
        //error
    }

    std::vector<unsigned char> red_data(width * height);
    std::vector<unsigned char> green_data(width * height);
    std::vector<unsigned char> blue_data(width * height);

    GDALRasterBand *red_band = dataset->GetRasterBand(1);
    GDALRasterBand *green_band = dataset->GetRasterBand(2);
    GDALRasterBand *blue_band = dataset->GetRasterBand(3);

    red_band->RasterIO(GF_Read, 0, 0, width, height, red_data.data(), width, height, GDT_Byte, 0, 0);
    green_band->RasterIO(GF_Read, 0, 0, width, height, green_data.data(), width, height, GDT_Byte, 0, 0);
    blue_band->RasterIO(GF_Read, 0, 0, width, height, blue_data.data(), width, height, GDT_Byte, 0, 0);

    mapDisplay.create(height, width, CV_8UC3);

    cv::Vec3b* pixel_ptr;
    for (int i = 0; i < height; i++) {
        pixel_ptr = mapDisplay.ptr<cv::Vec3b>(i);
        for (int j = 0; j < width; j++) {
            pixel_ptr[j][0] = blue_data[i * width + j];
            pixel_ptr[j][1] = green_data[i * width + j];
            pixel_ptr[j][2] = red_data[i * width + j];
        }
    }

    addColor(map,width,height);

    //cv::Mat resized_image;
    //cv::resize(mapDisplay, resized_image, cv::Size(800, 600));

    cv::namedWindow("Test Map", cv::WINDOW_NORMAL);
    cv::resizeWindow("Test Map", 800, 600);

    // Wyświetlanie obrazu
    cv::imshow("Test Map", mapDisplay);
    cv::waitKey(0);

}
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}
double** MapDisplay::addColor(Map map,int width,int height)
{
    std::string tag=map.getTag();

    double** density=new double*[width];
    for(int i=0;i<width;i++)
    {
        density[i]=new double[height];
        for(int j=0;j<height;j++)
            density[i][j]=0;
    }


    CURL* curl;
    CURLcode resultCode;
    std::string readBuffer;

    curl=curl_easy_init();
    if(curl==NULL)
    {
        std::cerr<<"libcurl error"<<std::endl;
        return nullptr;
    }

    std::string query = "[out:json];node[\"amenity\"=\"";
    query+=tag;
    query+="\"](";
    query+=map.getBounds();
    query+=");out skel;";
   

    std::string url = "http://overpass-api.de/api/interpreter?data=";
    std::cout<<query<<std::endl;
    url+= curl_easy_escape(curl, query.c_str(), query.length());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    std::cout<<"jestem"<<std::endl;
    resultCode=curl_easy_perform(curl);

    if(resultCode!=CURLE_OK)
    {
        std::cerr<<"libcurl action error"<<std::endl;
        return nullptr;
    }

    curl_easy_cleanup(curl);

    //readBuffer - json response
    //std::cout<<readBuffer<<std::endl;

    //data extraction
    std::istringstream dataStream(readBuffer);
    std::string part;

    double x,y;
    while(dataStream>>part)
    {
        if(part=="\"lat\":")
        {
            dataStream>>part;
            part.erase(part.size()-1,1);
            y=std::stod(part);
        }
        else if(part=="\"lon\":")
        {
            dataStream>>part;
            x=std::stod(part);
            
            processPoint(density,map,x,y,width,height);
        }
    }

    //test
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
            std::cout<<density[i][j]<<" ";
        std::cout<<std::endl;
    }

    return density;
}
void MapDisplay::processPoint(double** density,Map map,double x,double y,int width,int height)
{
    int pixelX=std::round((x-map.getRoot().getX())/(map.getEdge().getX()-map.getRoot().getX())*((double)width));
    int pixelY=std::round((y-map.getRoot().getY())/(map.getEdge().getY()-map.getRoot().getY())*((double)height));

    int r=std::sqrt(-ALPHA*log(EPSILON));

    for(int i=pixelX-r;i<=pixelX+r;i++)
    {
        for(int j=pixelY-r;j<=pixelY+r;j++)
        {  
            double R=sqrt(pow(j-pixelY,2)+pow(i-pixelX,2));
            double value=std::pow(M_E,-ALPHA*R*R);
            if(value>EPSILON && i>0 && i<width && j>0 && j<height)
                density[i][j]+=value;
        }   
    }
}
