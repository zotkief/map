#ifndef MAPSISPLAYHPP
#define MAPSISPLAYHPP

#define EPSILON 0.00001
#define ALPHA 8.0

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

    double** addColor(Map map,int width,int height);
    void processPoint(double** density,Map map,double x,double y,int width,int height);

public:
    ~MapDisplay();
    MapDisplay(Map map);    
};


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out);

#endif