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
    ~MapDisplay();
    MapDisplay(Map map);    
};

#endif