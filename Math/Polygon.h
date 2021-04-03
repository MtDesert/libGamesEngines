#ifndef POLYGON_H
#define POLYGON_H

#include"Point.h"

//可变多边形,可设置分段数(边数)
template<typename T>
struct PolygonVar{
	SizeType segment;//分段,用于画圆
	T *vertex;//多边形

	PolygonVar():segment(0),vertex(NULL){}
	//设置分段,根据分段去创建点数组
	void setSegment(SizeType seg){
		//调整精度
		if(segment==seg)return;//无变化
		if(vertex){//删除旧的多边形
			delete []vertex;
			vertex=NULL;
			segment=0;
		}
		//创建新的多边形
		if(seg<=0)return;//限制段数
		segment=seg;
		vertex=new T[segment];
	}
};

//2维可变多边形
template<typename T>
struct PolygonVar2D:public PolygonVar<Point2D<T> >{};
//3维可变多边形
template<typename T>
struct PolygonVar3D:public PolygonVar<Point3D<T> >{};

//不可变多边形
template<typename T,int n>
struct Polygon{
	T vertex[n];//多边形的顶点
	SizeType vertexAmount()const{return n;}//顶点数
	//周长(注:T不一定是点类型,也有可能是指针等特殊类型)
	virtual double perimeter()const{return 0;}
};

//不可变2维多边形
template<typename T,int n>
struct Polygon2D:public Polygon<Point2D<T>,n>{};
//不可变3维多边形
template<typename T,int n>
struct Polygon3D:public Polygon<Point3D<T>,n>{};

//4边形
template<typename T>
struct Polygon4:public Polygon<T,4>{
	Polygon4(const T &p0,const T &p1,const T &p2,const T &p3){
		this->vertex[0]=p0;
		this->vertex[1]=p1;
		this->vertex[2]=p2;
		this->vertex[3]=p3;
	}
};
#endif