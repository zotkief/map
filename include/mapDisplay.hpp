#ifndef MAPSISPLAYHPP
#define MAPSISPLAYHPP

#include <gdal/gdal_priv.h>
#include <opencv4/opencv2/opencv.hpp>

#include "map.hpp"
#include "geography.hpp"

class MapDisplay
{
private:
    cv::Mat mapDisplay;
    std::string filename;
    GDALDataset *dataset;
public:
    ~MapDisplay()
    {
        GDALClose(dataset);
    }
    MapDisplay(Map map)
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

        //cv::Mat resized_image;
        //cv::resize(mapDisplay, resized_image, cv::Size(800, 600));

        cv::namedWindow("Test Map", cv::WINDOW_NORMAL);
        cv::resizeWindow("Test Map", 800, 600);

        // Wyświetlanie obrazu
        cv::imshow("Test Map", mapDisplay);
        cv::waitKey(0);

    }    
};

#endif