#ifndef NUMBER_H
#define NUMBER_H

#include"typedef.h"
#include<math.h>
#include<stdlib.h>
#include<limits.h>
#include<list>
using namespace std;

//这是数字特征判定用的类
class Number{
public:
	//数的符号
	template<typename T>
	static T sgn(T num){
		if(num>0)return 1;
		if(num<0)return -1;
		return 0;
	}

	//整数除法,返回四舍五入结果
	static int divideRound(int devidend,int devisor);
	//整数除法,向上取整
	static int divideCeil(int devidend,int devisor);

	static string toString(int num,int base=10);
	//质数
	list<ulonglong> primeNumbersList;//质数表
	void makePrimeNumbersList(ulonglong range);//质数表,2~range为质数表范围
	bool isPrimeNumber(ulonglong num);//判断num是否为质数

	//最大公约数
	template<typename T>
	static T greatestCommonDivisor(T a,T b){
		if(a==0||b==0)return 0;
		T c=a;
		if(abs(a)<abs(b)){//交换,确保abs(a)>=abs(b)
			c=a;a=b;b=c;
		}
		do{//辗转相除法
			c=a%b;
			if(c==0)break;
			a=b;b=c;
		}while(c);
		return b;
	}
	//最小公倍数
	template<typename T>
	static T leastCommonMultiple(T a,T b){
		if(a==0||b==0)return 0;
		return a/greatestCommonDivisor(a,b)*b;
	}
	//互质数
	template<typename T>
	static bool isRelativelyPrime(T a,T b){
		return greatestCommonDivisor(a,b)==1;
	}
};
#endif
