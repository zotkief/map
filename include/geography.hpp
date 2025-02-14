#ifndef GEOHPP
#define GEOHPP

class GeoPoint
{
    double x,y;
public:
    double getX(){return x;}
    double getY(){return y;}

    GeoPoint(double X,double Y): x(X), y(Y) {}
};

#endif