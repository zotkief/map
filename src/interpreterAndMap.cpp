#include <string>
#include <iostream>
#include <sstream>

#include "interpreter.hpp"
#include "map.hpp"
#include "databaseConnector.hpp"
#include "mapDisplay.hpp"


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
    std::istringstream dataStream(result);

    std::cout<<result<<std::endl;

    std::string tempValue;
    double x,y;

    dataStream>>tempValue;
    x=stod(tempValue);
    dataStream>>tempValue;
    y=stod(tempValue);


    GeoPoint root(x,y);

    dataStream>>tempValue;
    x=stod(tempValue);
    dataStream>>tempValue;
    y=stod(tempValue);

    GeoPoint edge(x,y);

    std::string fileName;

    dataStream>>fileName;

    Map map(root,edge,fileName,tag);
    std::cout<<map.getBounds()<<std::endl;

    MapDisplay mapDisplay(map);
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
        edgeCorner=new GeoPoint(end.getX(),end.getY());
    }
}
std::string Map::getBounds()
{
    std::string s="";
    s+=std::to_string(rootCorner->getY());
    s+=+",";
    s+=std::to_string(rootCorner->getX());
    s+=+",";
    s+=std::to_string(edgeCorner->getY());
    s+=+",";
    s+=std::to_string(edgeCorner->getX());

    return s;
}