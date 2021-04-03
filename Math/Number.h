#ifndef NUMBER_H
#define NUMBER_H

#include"typedef.h"

#include<list>
using namespace std;
#include<math.h>
#include<stdlib.h>
#include<limits.h>

//这是数字特征判定用的类
class Number{
public:
	static const float PI;//圆周率
	static const float PI_2;//PI*2
	//数的符号
	template<typename T>
	static T sgn(T num){
		if(num>0)return 1;
		if(num<0)return -1;
		return 0;
	}

	//整数除法(被除数,除数)
	static int divideRound(int devidend,int devisor);//返回四舍五入结果
	static int divideCeil(int devidend,int devisor);//向上取整数
	static int divideFloor(int devidend,int devisor);//向上取整数
	//数字转字符串
	static string toString(int num,int base=10);
	static string double2String(double num);
	static string double2PercentString(double num);
	//随机数(游戏术语中有'乱数'之称)
	static int randomInt(int minimum,int maximum);//返回随机整数(最小值<=返回值<最大值)
	static float randomFloat(float minimum,float maximum);
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

/*整数的取值范围*/
struct IntRange{
	IntRange(int min=0,int max=0);
	int minimun,maximun;//最小值,最大值
};
#endif