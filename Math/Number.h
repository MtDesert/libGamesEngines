#ifndef NUMBER_H
#define NUMBER_H

#include"typedef.h"
#include<math.h>
#include<stdlib.h>
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
	//质数
	list<ulonglong> primeNumbersList;//质数表
	void makePrimeNumbersList(ulonglong range){//质数表,2~range为质数表范围
		if(range<2){//表中无项
			primeNumbersList.clear();
			return;
		}
		if(primeNumbersList.size()==0)primeNumbersList.push_back(2);//最小的质数
		ulonglong maxPrime=*primeNumbersList.rbegin();
		if(range<maxPrime){
			while(range<maxPrime){//删除多余的质数
				primeNumbersList.pop_back();//删除最后(最大)项
				maxPrime=*primeNumbersList.rbegin();
			}return;
		}
		if(range>maxPrime){//添加缺少的项
			ulonglong i=0,n=0;
			for(ulonglong num=maxPrime+1;num<=range;++num){
				n=sqrt((double)num);//以平方根为整除上限
				for(auto itr:primeNumbersList){
					i=itr;
					if(i>n)break;//超过整除上限,再算下去也无意义
					if(num%i==0)break;//可整除
				}
				if(num%i){//搜索完毕的时候,如果不可整除,则为质数
					primeNumbersList.push_back(num);
				}
			}
		}
	}
	bool isPrimeNumber(ulonglong num){//判断num是否为质数
		if(num<2)return false;
		if(primeNumbersList.size()==0)makePrimeNumbersList(2);
		auto maxPrime=*primeNumbersList.rbegin();
		if(num==maxPrime)return true;
		else if(num<maxPrime){//查表
			for(auto n:primeNumbersList){
				if(n==num)return true;
			}return false;
		}else{//num>maxPrime,扩充表
			makePrimeNumbersList(num);
			return num==*primeNumbersList.rbegin();
		}
	}

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
