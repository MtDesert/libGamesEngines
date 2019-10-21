#ifndef POINT_H
#define POINT_H

#include<cmath>

//二维点
#define POINT_RETURN_P(PointType,symbol)\
PointType operator symbol(T val)const{return PointType(x symbol val,y symbol val);}\
PointType operator symbol(const PointType &p)const{return PointType(x symbol p.x,y symbol p.y);}

template<typename T>
struct Point2D{
	T x,y;
	//构造函数
	Point2D(T x=0,T y=0){
		this->x=x;
		this->y=y;
	}
	Point2D(const Point2D &p):Point2D(p.x,p.y){}
	//算术运算符
	POINT_RETURN_P(Point2D,+)
	POINT_RETURN_P(Point2D,-)
	POINT_RETURN_P(Point2D,*)
	POINT_RETURN_P(Point2D,/)
	//关系运算符
	bool operator==(const Point2D &p)const{return this->x==p.x&&this->y==p.y;}
	bool operator!=(T val)const{return !(*this==val);}
	bool operator!=(const Point2D &p)const{return !(*this==p);}
	//特征
	T manhattanLength()const{//曼哈顿路径
		return abs(x)+abs(y);
	}
	T distance2()const{//直线距离的平方
		return x*x+y*y;
	}
	double distance()const{return ::sqrt(distance2());}//直线距离
};
//三维点
template<typename T>
struct Point3D:public Point2D<T>{
	T z;
	//构造函数
	Point3D(T x=0,T y=0,T z=0){
		this->x=x;
		this->y=y;
		this->z=z;
	}
	Point3D(const Point3D &p):Point3D(p.x,p.y,p.z){}
};
#endif