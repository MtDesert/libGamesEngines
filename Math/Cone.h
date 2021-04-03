#ifndef CONE_H
#define CONE_H

//本文件定义各种锥体
#include"Polygon.h"
#include"Point.h"

//锥体特点,底面是个多边形,只有一个不在多边形面内的顶点
template<typename T,int n>
struct Cone{
	Polygon2D<T,n> polygen;//多边形底面
	Point3D<T> peak;//顶峰
};

//3棱锥
template<typename T>
struct TriangleCone:public Cone<T,3>{};

#endif