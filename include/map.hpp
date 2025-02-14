#ifndef MAPHPP
#define MAPHPP

#include <cmath>
#include "geography.hpp"

class Map
{
    GeoPoint *rootCorner,*edgeCorner;
    std::string fileName,tagName;
public:
    Map(GeoPoint beg,GeoPoint end,std::string fl,std::string tg)
        : fileName(fl),tagName(tg) 
    {
        if(beg.getX()>end.getX() || beg.getY()>end.getY())
        {
            //todo warning, adjusting input
            rootCorner=new GeoPoint(std::min(beg.getX(),end.getX()),std::min(beg.getY(),end.getY()));
            edgeCorner=new GeoPoint(std::max(beg.getX(),end.getX()),std::max(beg.getY(),end.getY()));
        }
        else
        {
            rootCorner=new GeoPoint(beg.getX(),beg.getY());
            edgeCorner=new GeoPoint(end.getX(),beg.getY());
        }
    }
    GeoPoint getRoot(){return *rootCorner;}
    GeoPoint getEdge(){return *edgeCorner;}
    std::string getFile(){return fileName;}
    std::string getTag(){return tagName;}
};

#endif