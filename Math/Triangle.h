#ifndef TRIANGLE_H
#define TRIANGLE_H

#include"Point.h"
#include"Line.h"

#define TRIANGLE2D_LENGTHS double a=length0();double b=length1();double c=length2();
#define TRIANGLE2D_LENGTHS_SQUARE double aa=a*a;double bb=b*b;double cc=c*c;

//三角形,可以在2维和3维中使用
template<typename T,size_t d>
struct Triangle{
	//特征
	size_t dimension()const{return d;}//维度
	//线
	Line<T,d> line0()const{return Line<T,d>(p0,p1);}
	Line<T,d> line1()const{return Line<T,d>(p1,p2);}
	Line<T,d> line2()const{return Line<T,d>(p2,p0);}
	//边长的平方
	T length0_2()const{return (p1-p0).distance2();}
	T length1_2()const{return (p2-p1).distance2();}
	T length2_2()const{return (p0-p2).distance2();}
	//边长
	double length0()const{return sqrt(length0_2());}
	double length1()const{return sqrt(length1_2());}
	double length2()const{return sqrt(length2_2());}
	//周长
	double perimeter()const{return length0()+length1()+length2();}
	//面积(海伦公式)
	double area()const{
		TRIANGLE2D_LENGTHS
		double p=(a+b+c)/2;
		return p=sqrt(p*(p-a)*(p-b)*(p-c));
	}
	bool isTriangle()const{return area()>0;}//三角形面积大于0
	bool isAcuteTriangle()const{//锐角三角形
		TRIANGLE2D_LENGTHS
		TRIANGLE2D_LENGTHS_SQUARE;
		return (aa*bb>cc)&&(aa*cc>bb)&&(bb*cc>aa);
	}
	bool isRightTriangle()const{//直角三角形
		TRIANGLE2D_LENGTHS
		TRIANGLE2D_LENGTHS_SQUARE;
		return (aa*bb==cc)||(aa*cc==bb)||(bb*cc==aa);
	}
	bool isObtuseTriangle()const{//钝角三角形
		TRIANGLE2D_LENGTHS
		TRIANGLE2D_LENGTHS_SQUARE;
		return (aa*bb<cc)||(aa*cc<bb)||(bb*cc<aa);
	}
	bool isIsoscelesTriangle()const{//等腰三角形
		TRIANGLE2D_LENGTHS
		return a==b||a==c||b==c;
	}
	bool isEquilateralTriangle()const{//等边三角形
		TRIANGLE2D_LENGTHS
		return a==b&&b==c;
	}
	//成员变量
	Point<T,d> p0,p1,p2;
};

template<typename T>
struct Triangle2D:public Triangle<T,2>{};

template<typename T>
struct Triangle3D:public Triangle<T,3>{};

#endif // TRIANGLE_H
