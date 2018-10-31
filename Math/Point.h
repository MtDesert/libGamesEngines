#ifndef POINT_H
#define POINT_H

#include<cmath>
#include<string>
#include<sstream>
using namespace std;
#include<string.h>

#define POINT_FOR(code)\
for(size_t i=0;i<d;++i){code;}

//返回Point的函数的通用宏
#define POINT_RETURN_P(symbol)\
Point operator symbol(T val)const{\
	Point p;\
	POINT_FOR(p.data[i]=data[i] symbol val)\
	return p;\
}\
Point operator symbol(const Point &p)const{\
	Point p0;\
	POINT_FOR(p0.data[i]=data[i] symbol p.data[i])\
	return p0;\
}

//返回bool的函数的通用宏
#define POINT_RETURN_BOOL(symbol)\
bool operator symbol(T val)const{\
	POINT_FOR(if(!(data[i] symbol val))return false)\
	return true;\
}\
bool operator symbol(const Point &p)const{\
	POINT_FOR(if(!(data[i] symbol p.data[i]))return false)\
	return true;\
}

//返回Point&的函数的通用宏
#define POINT_RETURN_SELF(symbol)\
Point& operator symbol(T val){\
	POINT_FOR(data[i] symbol val)\
	return *this;\
}\
Point& operator symbol(const Point &p){\
	POINT_FOR(data[i] symbol p.data[i])\
	return *this;\
}

//点通用模板,T表示数据类型(int,uint,float等),d表示维度
template<typename T,size_t d>
struct Point{
	//构造/析构函数
	explicit Point(){memset(data,0,sizeof(T)*d);}
	~Point(){}

	//逻辑运算符
	bool operator!()const{return (*this==0);}
	//自增减运算符
	Point& operator++(){return operator+=(1);}//前置++
	Point& operator--(){return operator-=(1);}//前置--
	Point operator++(int){Point p=*this;++(*this);return p;}//后置++
	Point operator--(int){Point p=*this;--(*this);return p;}//后置--
	//负号
	Point operator-()const{
		Point p;
		POINT_FOR(p.data[i]=-data[i])
		return p;
	}
	//算术运算符
	POINT_RETURN_P(+)
	POINT_RETURN_P(-)
	POINT_RETURN_P(*)
	POINT_RETURN_P(/)
	//关系运算符
	POINT_RETURN_BOOL(==)
	bool operator!=(T val)const{return !(*this==val);}
	bool operator!=(const Point &p)const{return !(*this==p);}
	POINT_RETURN_BOOL(<)
	POINT_RETURN_BOOL(>)
	POINT_RETURN_BOOL(<=)
	POINT_RETURN_BOOL(>=)
	//位运算符
	Point operator~()const{
		Point p;
		POINT_FOR(p.data[i]=~data[i])
		return p;
	}
	POINT_RETURN_P(<<)
	POINT_RETURN_P(>>)
	POINT_RETURN_P(|)
	POINT_RETURN_P(&)
	POINT_RETURN_P(^)
	//自返运算符
	POINT_RETURN_SELF(+=)
	POINT_RETURN_SELF(-=)
	POINT_RETURN_SELF(*=)
	POINT_RETURN_SELF(/=)
	POINT_RETURN_SELF(>>=)
	POINT_RETURN_SELF(<<=)
	POINT_RETURN_SELF(&=)
	POINT_RETURN_SELF(|=)
	POINT_RETURN_SELF(^=)
	//点积
	T dotProduct(const Point &p)const{
		T ret=0;
		POINT_FOR(ret+=data[i]*p.data[i])
		return ret;
	}

	//特征
	size_t dimension()const{return d;}//维度
	bool isZero()const{//是否原点
		POINT_FOR(if(data[i])return false;)
		return true;
	}
	T manhattanLength()const{//曼哈顿路径
		T ret=0;
		POINT_FOR(ret+=abs(data[i]))
		return ret;
	}
	T distance2()const{//直线距离的平方
		T ret=0;
		POINT_FOR(ret+=data[i]*data[i])
		return ret;
	}
	double distance()const{return ::sqrt(distance2());}//直线距离

	//debug
	string toString()const{
		stringstream strm;
		POINT_FOR(
		strm<<data[i];
		if(i+1<d)strm<<",";)
		return strm.str();
	}
	T data[d];//各个维的值
};

//点的内联函数,用于映射
#define POINT_INLINE(name,index)\
inline T& name(){return this->data[index];}\
inline const T& name()const{return this->data[index];}

//二维点
template<typename T>
struct Point2D:public Point<T,2>{
	//构造函数
	Point2D(){}
	Point2D(T x,T y){
		this->x()=x;
		this->y()=y;
	}
	Point2D(const Point<T,2> &p):Point2D(p.data[0],p.data[1]){}
	//维度名称
	POINT_INLINE(x,0)
	POINT_INLINE(y,1)
};
//三维点
template<typename T>
struct Point3D:public Point<T,3>{
	//构造函数
	Point3D(){}
	Point3D(T x,T y,T z){
		this->x()=x;
		this->y()=y;
		this->z()=z;
	}
	Point3D(const Point<T,3> &p):Point3D(p.data[0],p.data[1],p.data[2]){}
	//维度名称
	POINT_INLINE(x,0)
	POINT_INLINE(y,1)
	POINT_INLINE(z,2)
};

#endif
