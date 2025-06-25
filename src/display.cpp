#include <gdal/gdal_priv.h>
#include <opencv4/opencv2/opencv.hpp>  
#include <curl/curl.h>
#include <nlohmann/json.hpp> 
#include <iostream>
#include <cmath>
#include <math.h>
#include <algorithm>

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

    display.create(height, width, CV_8UC3);


    double** density=addColor(map,width,height);


    cv::Vec3b* pixel_ptr;
    for (int i = 0; i < height; i++) {
        pixel_ptr = display.ptr<cv::Vec3b>(i);
        for (int j = 0; j < width; j++) {
            pixel_ptr[j][1] = green_data[i * width + j];
            if(blue_data[i * width + j]>200&&green_data[i * width + j]>200)
            {
                pixel_ptr[j][0] = 255-std::min((int)density[j][i],255);//blue_data[i * width + j];
                pixel_ptr[j][2] = 255-std::min((int)density[j][i],255);//red_data[i * width + j];
            }
            else
            {
                pixel_ptr[j][0] = blue_data[i * width + j];
                pixel_ptr[j][2] = red_data[i * width + j];
            }
        }
    }


    //cv::Mat resized_image;
    //cv::resize(display, resized_image, cv::Size(800, 600));

    cv::namedWindow("Test Map", cv::WINDOW_NORMAL);
    cv::resizeWindow("Test Map", 800, 600);

    // Wyświetlanie obrazu
    cv::imshow("Test Map", display);
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
    //data extraction
    std::istringstream dataStream(readBuffer);
    std::string part;

    double degreesPerPixel=(map.getEdge().getX()-map.getRoot().getX())/width;

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
            
            processPoint(density,map,x,y,width,height,degreesPerPixel);
        }
    }

    double maxDensity=0;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
            if(maxDensity<density[i][j])
                maxDensity=density[i][j];
    }
    double aplifier=255.0/maxDensity;

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
            density[i][j]*=aplifier;
    }

    return density;
}
void MapDisplay::processPoint(double** density,Map map,double x,double y,int width,int height,double DPP)
{
    int pixelX=std::round((x-map.getRoot().getX())/(map.getEdge().getX()-map.getRoot().getX())*((double)width));
    int pixelY=std::round((y-map.getRoot().getY())/(map.getEdge().getY()-map.getRoot().getY())*((double)height));

    //std::cout<<x<<" "<<y<<" "<<pixelX<<" "<<pixelY<<std::endl;

    int r=pow(log(EPSILON)/(-ALPHA),1);
    //std::cout<<x<<" "<<map.getEdge().getX()<<" "<<map.getRoot().getX()<<" "<<r<<std::endl;
    //std::cout<<r<<std::endl;

    for(int i=pixelX-r;i<=pixelX+r;i++)
    {
        for(int j=pixelY-r;j<=pixelY+r;j++)
        {  
            double R=pow(pow(j-pixelY,2)+pow(i-pixelX,2),0.5);
            double value=std::pow(M_E,-ALPHA*R);
            if(value>EPSILON && i>0 && i<width && j>0 && j<height)
                density[i][j]+=value;
        }   
    }
}
