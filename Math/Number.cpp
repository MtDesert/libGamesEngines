#include"Number.h"

IntRange::IntRange(int min,int max):minimun(min),maximun(max){}

#define DIVIDE_CONDITION(condition) \
if(devisor==0)return INT_MAX;\
int ret=devidend/devisor;\
if(condition)++ret;\
return ret;

int Number::divideRound(int devidend,int devisor){
	DIVIDE_CONDITION(devidend%devisor*2>=devisor)
}
int Number::divideCeil(int devidend,int devisor){
	DIVIDE_CONDITION(devidend%devisor)
}
#undef DIVIDE_CONDITION
string Number::toString(int num,int base){
	string ret;
	if(base>=2 && base<=36){
		auto devidend=num;
		char buf[2]={0,0};
		do{//被除数为0时停止
			auto remain = devidend % base;//计算余数
			buf[0] = base<=10 ? '0'+remain : 'A'+(remain-10);
			ret.insert(0,buf);
			//下一个
			devidend/=base;
		}while(devidend);
	}
	return ret;
}
string Number::double2String(double num){
	char str[20];
	sprintf(str,"%lf",num);
	return str;
}
string Number::double2PercentString(double num){
	char str[20];
	sprintf(str,"%4.2f%%",num*100);
	return str;
}

int Number::randomInt(int minimum,int maximum){
	if(minimum==maximum)return minimum;
	//开始计算
	auto delta = maximum-minimum;
	return minimum + rand()%delta;
}

void Number::makePrimeNumbersList(ulonglong range){
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
bool Number::isPrimeNumber(ulonglong num){
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