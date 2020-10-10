#ifndef POINT_H
#define POINT_H

#include<cmath>
#include"typedef.h"

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

	void setXY(T x,T y){this->x=x;this->y=y;}
	void setP(const Point2D<T> &p){setXY(p.x,p.y);}
	//算术运算符
	POINT_RETURN_P(Point2D,+)
	POINT_RETURN_P(Point2D,-)
	POINT_RETURN_P(Point2D,*)
	POINT_RETURN_P(Point2D,/)
#undef POINT_RETURN_P
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
#define POINT_RETURN_P(PointType,symbol)\
PointType operator symbol(T val)const{return PointType(this->x symbol val,this->y symbol val,z symbol val);}\
PointType operator symbol(const PointType &p)const{return PointType(this->x symbol p.x,this->y symbol p.y,z symbol p.z);}

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
	//算术运算符
	POINT_RETURN_P(Point3D,+)
	POINT_RETURN_P(Point3D,-)
	POINT_RETURN_P(Point3D,*)
	POINT_RETURN_P(Point3D,/)

	void setXYZ(T x,T y,T z){this->setXY(x,y);this->z=z;}
};

#define POINT_OPERATOR_EXPRESSION(Type1,symbol,Type2,ReturnType)\
ReturnType operator symbol(const Point2D<Type1> &p1,const Point2D<Type2> &p2);

#define POINT_OPERATOR_2(Type1,symbol,Type2,ReturnType)\
POINT_OPERATOR_EXPRESSION(Type1,symbol,Type2,ReturnType)\
POINT_OPERATOR_EXPRESSION(Type2,symbol,Type1,ReturnType)

POINT_OPERATOR_2(int,==,float,bool)
POINT_OPERATOR_2(int,+,float,Point2D<int>)
POINT_OPERATOR_2(int,-,float,Point2D<int>)
POINT_OPERATOR_2(float,+,SizeType,Point2D<float>)

#endif