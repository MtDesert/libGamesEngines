#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include"Array.h"

//枚举类型,用于从其他语言的枚举转换成类似C语言的枚举,相比C语言的枚举,本类可以为每个枚举值存个字符串
class EnumType{
	Array<string> enumValues;
public:
	EnumType();
	~EnumType();
	//类型
	typedef SizeType AmountType;

	//枚举数
	void setAmount(AmountType amount);//设置枚举数(这会清除所有空间)
	AmountType getAmount()const;//获取枚举数,可用于判断越界
	void clear();
	//枚举值
	bool setEnumName(AmountType index,const char* name);//给枚举值index设置名称name
	bool setEnumName(AmountType index,const string &name);//给枚举值index设置名称name
	bool getEnumName(AmountType index,string &name)const;//获取枚举值index的名称name
	bool getEnumIndex(const string &name,AmountType &index);//根据name获取枚举值
};
#endif