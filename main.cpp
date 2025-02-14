#include <gdal/gdal_priv.h>
#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

#include "include/mapDisplay.hpp"
#include "include/map.hpp"
#include "include/geography.hpp"

int main() {
    GDALAllRegister();

    //test setup
    GeoPoint point(0.1,0.1); 
    Map map(point,point,"testMap.tif","tag");
    //test setup - end

    MapDisplay* mapDisplay=new MapDisplay(map);
    return 0;
}
