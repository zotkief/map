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
    std::preset,tag;

    sstream>>preset;
    sstream>>preset;
    sstream>>tag;

    //todo actual generation
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