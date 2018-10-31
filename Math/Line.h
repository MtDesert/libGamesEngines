#ifndef LINE_H
#define LINE_H

#include"Point.h"

//返回Line的函数的通用宏
#define LINE_RETURN_LINE(symbol)\
Line operator symbol(T val)const{\
	Line line;\
	line.p0=p0 symbol val;line.p1=p1 symbol val;\
	return line;\
}\
Line operator symbol(const Point<T,d> &p)const{\
	Line line;\
	line.p0=p0 symbol p;line.p1=p1 symbol p;\
	return line;\
}\
Line operator symbol(const Line &l)const{\
	Line line;\
	line.p0=p0 symbol l.p0;line.p1=p1 symbol l.p1;\
	return line;\
}

//返回bool的函数的通用宏
#define LINE_RETURN_BOOL(symbol)\
bool operator symbol(T val)const{return (p0 symbol val)&&(p1 symbol val);}\
bool operator symbol(const Point<T,d> &p)const{return (p0 symbol p)&&(p1 symbol p);}\
bool operator symbol(const Line &l)const{return (p0 symbol l.p0)&&(p1 symbol l.p1);}

//返回Line&的函数的通用宏
#define LINE_RETURN_SELF(symbol)\
Line& operator symbol(T val)const{p0 symbol val;p1 symbol val;return *this;}\
Line& operator symbol(const Point<T,d> &p)const{p0 symbol p;p1 symbol p;return *this;}\
Line& operator symbol(const Line &l)const{p0 symbol l.p0;p1 symbol l.p1;return *this;}

//线通用模板,T表示数据类型(int,uint,float等),d表示维度
template<typename T,size_t d>
struct Line{
	//构造/析构函数
	Line(){}
	Line(const Point<T,d> &p0,const Point<T,d> &p1):p0(p0),p1(p1){}

	//逻辑运算符
	bool operator!()const{return (*this==0);}
	//自增减运算符
	Line& operator++(){return operator+=(1);}//前置++
	Line& operator--(){return operator-=(1);}//前置--
	Line operator++(int){Line p=*this;++(*this);return p;}//后置++
	Line operator--(int){Line p=*this;--(*this);return p;}//后置--
	//负号
	Line operator-()const{
		return Line(-p0,-p1);
	}
	//算术运算符
	LINE_RETURN_LINE(+)
	LINE_RETURN_LINE(-)
	LINE_RETURN_LINE(*)
	LINE_RETURN_LINE(/)
	//关系运算符
	LINE_RETURN_BOOL(==)
	bool operator!=(T val)const{return !(*this==val);}
	bool operator!=(const Point<T,d> &p)const{return !(*this==p);}
	bool operator!=(const Line &l)const{return !(*this==l);}
	LINE_RETURN_BOOL(<)
	LINE_RETURN_BOOL(>)
	LINE_RETURN_BOOL(<=)
	LINE_RETURN_BOOL(>=)
	//位运算符
	Line operator~()const{
		Line line;
		line.p0=~p0;line.p1=~p1;
		return line;
	}
	LINE_RETURN_LINE(<<)
	LINE_RETURN_LINE(>>)
	LINE_RETURN_LINE(|)
	LINE_RETURN_LINE(&)
	LINE_RETURN_LINE(^)
	//自返运算符
	LINE_RETURN_SELF(+=)
	LINE_RETURN_SELF(-=)
	LINE_RETURN_SELF(*=)
	LINE_RETURN_SELF(/=)
	LINE_RETURN_SELF(>>=)
	LINE_RETURN_SELF(<<=)
	LINE_RETURN_SELF(&=)
	LINE_RETURN_SELF(|=)
	LINE_RETURN_SELF(^=)

	//特征
	size_t dimension()const{return d;}//维度
	bool isLine()const{return p0!=p1;}//是否线,两点重合则非线
	T manhattanLength()const{//曼哈顿路径
		return (p1-p0).manhattanLength();
	}
	T length2()const{return (p1-p0).distance2();}//直线距离的平方
	double length()const{return (p1-p0).distance();}//直线距离

	//转换
	Point<T,d> toVec()const{return p1-p0;}
	//成员变量
	Point<T,d> p0,p1;
};

#define LINE_INLINE(name,index)\
T name()const{return this->p1.data[index]-this->p0.data[index];}

template<typename T>
struct Line2D:public Line<T,2>{
	LINE_INLINE(x,0)
	LINE_INLINE(y,1)
	Point2D<T> toPoint2D()const{return this->p1-this->p0;}
};

template<typename T>
struct Line3D:public Line<T,3>{
	LINE_INLINE(x,0)
	LINE_INLINE(y,1)
	LINE_INLINE(z,2)
	Point3D<T> toPoint3D()const{return this->p1-this->p0;}
};

#endif
