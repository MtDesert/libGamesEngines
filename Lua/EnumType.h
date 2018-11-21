#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include"typedef.h"
#include<string>
using namespace std;
//枚举,用于从其他语言的枚举转换成类似C语言的枚举,相比C语言的枚举,本类可以为每个枚举值存个字符串
class EnumType{
	//成员变量
	string *enumArray;
	uint enumAmount;//枚举总数
public:
	EnumType();
	~EnumType();
	//类型
	typedef decltype(enumAmount) amountType;

	//枚举数
	void setAmount(amountType amount);//设置枚举数(这会清除所有空间)
	amountType getAmount()const;//获取枚举数,可用于判断越界
	void clear();
	//枚举值
	bool setEnumName(amountType index,const string &name);//给枚举值index设置名称name
	bool getEnumName(amountType index,string &name)const;//获取枚举值index的名称name
	bool getEnumIndex(const string &name,amountType &index);//根据name获取枚举值
};

#endif //ENUM_H