#ifndef LINE_H
#define LINE_H

#include"Point.h"

//线,T表示数据类型(int,uint,float等)
template<typename T>
struct Line2D{
	//构造/析构函数
	Line2D(const Point2D<T> &p0=Point2D<T>(),const Point2D<T> &p1=Point2D<T>()):p0(p0),p1(p1){}
	//成员变量
	Point2D<T> p0,p1;
};

#endif