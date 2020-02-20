#include"Point.h"

bool operator==(const Point2D<int> &p1,const Point2D<float> &p2){return p1.x==p2.x && p1.y==p2.y;}
bool operator==(const Point2D<float> &p1,const Point2D<int> &p2){return p1.x==p2.x && p1.y==p2.y;}

Point2D<int> operator+(const Point2D<int> &p1,const Point2D<float> &p2){return Point2D<int>(p1.x+p2.x,p1.y+p2.y);}
Point2D<int> operator+(const Point2D<float> &p1,const Point2D<int> &p2){return Point2D<int>(p1.x+p2.x,p1.y+p2.y);}

Point2D<int> operator-(const Point2D<int> &p1,const Point2D<float> &p2){return Point2D<int>(p1.x-p2.x,p1.y-p2.y);}
Point2D<int> operator-(const Point2D<float> &p1,const Point2D<int> &p2){return Point2D<int>(p1.x-p2.x,p1.y-p2.y);}