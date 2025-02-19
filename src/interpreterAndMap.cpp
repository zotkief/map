#include <string>
#include <iostream>
#include <sstream>

#include "interpreter.hpp"
#include "map.hpp"
#include "databaseConnector.hpp"


std::string processManual()
{
    return "available commands:\n gen [preset name] [tag name]\n exit\n showPresets\n ";
}
void processGen(std::string command)
{
    std::istringstream sstream(command);
    std::string preset,tag;

    sstream>>preset;
    sstream>>preset;
    sstream>>tag;

    DatabaseConnector db;

    std::string result=db.getData(preset);
    std:istringstream sstream(result);

    
}
std::string processShow()
{
    DatabaseConnector connector;
    return connector.getPresets();
}
Map::Map(GeoPoint beg,GeoPoint end,std::string fl,std::string tg)
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
std::string Map::getBounds()
{
    std::string s="";
    s+=rootCorner->getY();
    s+=+",";
    s+=rootCorner->getX();
    s+=+",";
    s+=edgeCorner->getY();
    s+=+",";
    s+=edgeCorner->getX();

    return s;
}