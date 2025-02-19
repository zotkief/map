#ifndef MAPHPP
#define MAPHPP

#include <cmath>
#include "geography.hpp"

class Map
{
    GeoPoint *rootCorner,*edgeCorner;
    std::string fileName,tagName;
public:
    Map(GeoPoint beg,GeoPoint end,std::string fl,std::string tg);
    GeoPoint getRoot(){return *rootCorner;}
    GeoPoint getEdge(){return *edgeCorner;}
    std::string getFile(){return fileName;}
    std::string getTag(){return tagName;}
    std::string getBounds();
};

#endif