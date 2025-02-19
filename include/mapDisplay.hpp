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

    void addColor(Map map);
public:
    ~MapDisplay();
    MapDisplay(Map map);    
};


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out);

#endif