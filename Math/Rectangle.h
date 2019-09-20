#ifndef RECTANGLE_H
#define RECTANGLE_H

#include"Point.h"

template<typename T>
//2维矩形
struct Rectangle2D{
	Rectangle2D(){}
	Rectangle2D(const Point2D<T> &p0,const Point2D<T> &p1):p0(p0),p1(p1){}
	Rectangle2D(const T &x0,const T &y0,const T &x1,const T &y1):p0(x0,y0),p1(x1,y1){}

	//特征
	inline T width()const{return abs(p1.x-p0.x);}
	inline T height()const{return abs(p1.y-p0.y);}
	inline T perimeter()const{return (width()+height())*2;}//周长
	inline T area()const{return width()*height();}//面积
	bool isRectangle()const{return area()>0;}

	//坐标访问
#define RECTANGLE_INLINE_T_REF(name,xORy,gtORlt) \
inline T& name(){return (p0.xORy gtORlt p1.xORy?p0.xORy:p1.xORy);}\
inline const T& name()const{return (p0.xORy gtORlt p1.xORy?p0.xORy:p1.xORy);}

	RECTANGLE_INLINE_T_REF(left,x,<)
	RECTANGLE_INLINE_T_REF(right,x,>)
	RECTANGLE_INLINE_T_REF(bottom,y,<)
	RECTANGLE_INLINE_T_REF(top,y,>)

	Rectangle2D normalized()const{//坐标标准化
		Rectangle2D ret;
		ret.p0.x=min(p0.x,p1.x);
		ret.p1.x=max(p0.x,p1.x);
		ret.p0.y=min(p0.y,p1.y);
		ret.p1.y=max(p0.y,p1.y);
		return ret;
	}
	//是否包含点
	bool containPoint(const T &x,const T &y)const{
		return left()<x && x<right() && bottom()<y && y<top();
	}
	bool containPoint(const Point2D<T> &p)const{
		return containPoint(p.x,p.y);
	}

	//特征点
#define RECTANGLE_INLINE_POINT2D_CENTER_X left()+(right()-left())/2
#define RECTANGLE_INLINE_POINT2D_CENTER_Y bottom()+(top()-bottom())/2
#define RECTANGLE_INLINE_POINT2D(name,x,y)\
inline Point2D<T> name()const{return Point2D<T>(x,y);}

	RECTANGLE_INLINE_POINT2D(topLeft,left(),top())
	RECTANGLE_INLINE_POINT2D(topRight,right(),top())
	RECTANGLE_INLINE_POINT2D(bottomLeft,this->left(),this->bottom())
	RECTANGLE_INLINE_POINT2D(bottomRight,right(),bottom())
	RECTANGLE_INLINE_POINT2D(leftCenter,left(),RECTANGLE_INLINE_POINT2D_CENTER_Y)
	RECTANGLE_INLINE_POINT2D(rightCenter,right(),RECTANGLE_INLINE_POINT2D_CENTER_Y)
	RECTANGLE_INLINE_POINT2D(bottomCenter,RECTANGLE_INLINE_POINT2D_CENTER_X,bottom())
	RECTANGLE_INLINE_POINT2D(topCenter,RECTANGLE_INLINE_POINT2D_CENTER_X,top())
	RECTANGLE_INLINE_POINT2D(center,RECTANGLE_INLINE_POINT2D_CENTER_X,RECTANGLE_INLINE_POINT2D_CENTER_Y)

	//平移
	void translate(const T x,const T y){
		p0.x+=x;
		p0.y+=y;
		p1.x+=x;
		p1.y+=y;
	}
	//成员变量
	Point2D<T> p0,p1;//p0和p1是对角关系
};

#endif
