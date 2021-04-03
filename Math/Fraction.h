#ifndef FRACTION_H
#define FRACTION_H

#include"Number.h"
#include<string>
#include<sstream>
using namespace std;

#define FRACTION_RETURN_BOOL(symbol)\
bool operator symbol(const Fraction &f)const{\
	return (*this-f).denominatorPositive().numerator symbol 0;\
}
#define FRACTION_RETURN_SELF(symbol)\
Fraction& operator symbol##=(const Fraction &f){*this=*this symbol f;return *this;}

//分数,有分子和分母
template<typename T>
struct Fraction{
	//构造/析构函数
	Fraction():Fraction(0,0){}
	Fraction(T numerator):Fraction(numerator,1){}
	Fraction(T numerator,T denominator):numerator(numerator),denominator(denominator){}

	//特征
	bool isValid()const{return denominator!=0;}//有效性(分母不可为0)
	bool isProperFraction()const{return abs(numerator)<abs(denominator);}//是否真分数
	bool isSimplest()const{return Number::isRelativelyPrime(abs(numerator),abs(denominator));}//是否最简分数
	T sgn()const{//正负号,返回正(1)负(-1)零(0)
		return Number::sgn(numerator)*Number::sgn(denominator);
	}
	
	//值
	T quotient()const{return numerator/denominator;}//商
	T remainder()const{return numerator%denominator;}//余数
	//转化
	Fraction reduction()const{//约分
		auto gcd=Number::greatestCommonDivisor(numerator,denominator);
		return Fraction(numerator/gcd,denominator/gcd);
	}
	Fraction reciprocal()const{//倒数
		return Fraction(denominator,numerator);
	}
	Fraction denominatorPositive()const{//分母正数化
		return Fraction(abs(numerator)*sgn(),abs(denominator));
	}
	Fraction formal()const{//规范化
		return reduction().denominatorPositive();
	}

	//逻辑运算符
	bool operator!()const{return (numerator==0);}
	//负号
	Fraction operator-()const{
		return Fraction(-numerator,denominator);
	}
	//算数运算符
	Fraction operator+(const Fraction &f)const{
		Fraction ret;
		T lcm=Number::leastCommonMultiple(denominator,f.denominator);//先通分
		ret.denominator=lcm;
		ret.numerator =  numerator*(lcm/denominator);//后加减
		ret.numerator+=f.numerator*(lcm/f.denominator);
		return ret;
	}
	Fraction operator-(const Fraction &f)const{
		Fraction ret;
		T lcm=Number::leastCommonMultiple(abs(denominator),abs(f.denominator));//先通分
		ret.denominator=lcm;
		ret.numerator =  numerator*(lcm/denominator);//后加减
		ret.numerator-=f.numerator*(lcm/f.denominator);
		return ret;
	}
	Fraction operator*(const Fraction &f)const{
		return Fraction(numerator*f.numerator,denominator*f.denominator);
	}
	Fraction operator/(const Fraction &f)const{
		return (*this)*f.reciprocal();
	}
	Fraction operator%(const Fraction &f)const{//余数,这是一种很特别的运算
		return Fraction( (numerator*f.denominator)%(denominator*f.numerator) , denominator*f.denominator );
	}
	//关系运算符
	FRACTION_RETURN_BOOL(==)
	bool operator!=(const Fraction &f)const{return !(*this==f);}
	FRACTION_RETURN_BOOL(<)
	FRACTION_RETURN_BOOL(>)
	FRACTION_RETURN_BOOL(<=)
	FRACTION_RETURN_BOOL(>=)

	//自返运算符
	FRACTION_RETURN_SELF(+)
	FRACTION_RETURN_SELF(-)
	FRACTION_RETURN_SELF(*)
	FRACTION_RETURN_SELF(/)
	FRACTION_RETURN_SELF(%)

	//格式化
	string toString()const{
		stringstream strm;
		strm<<numerator<<"/"<<denominator;
		return strm.str();
	}
	//成员变量
	T numerator;//分子
	T denominator;//分母
};
#endif //FRACTION_H