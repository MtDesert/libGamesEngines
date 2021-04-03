#ifndef LINE_H
#define LINE_H

#include"Point.h"
//线类型,用于解决线问题,T为点类型
template<typename T>
struct Line{
	//成员变量
	T p0,p1;//线段的两端
	//构造/析构函数
	Line(const T &p0=T(),const T &p1=T()):p0(p0),p1(p1){}
};

template<typename T>//2维线
struct Line2D:public Line<Point2D<T> >{};
template<typename T>//3维线
struct Line3D:public Line<Point3D<T> >{};

#endif