#include "EnumType.h"

EnumType::EnumType(){}
EnumType::~EnumType(){clear();}

void EnumType::setAmount(AmountType amount){enumValues.setArraySize(amount);}
EnumType::AmountType EnumType::getAmount()const{return enumValues.arraySize();}
void EnumType::clear(){enumValues.clear();}

bool EnumType::setEnumName(AmountType index,const char *name){
	if(!name)return false;
	auto data=enumValues.data(index);
	if(data)*data=name;
	return data;
}
bool EnumType::setEnumName(AmountType index,const string &name){
	auto data=enumValues.data(index);
	if(data)*data=name;
	return data;
}
bool EnumType::getEnumName(AmountType index,string &name)const{
	auto data=enumValues.data(index);
	if(data)name=*data;
	return data;
}
bool EnumType::getEnumIndex(const string &name,AmountType &index){
	for(AmountType i=0;i<enumValues.size();++i){
		auto data=enumValues.data(i);
		if(data && *data==name){
			index=i;
			return true;
		}
	}
	return false;
}