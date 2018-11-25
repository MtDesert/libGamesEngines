#include "EnumType.h"

EnumType::EnumType():enumArray(nullptr),enumAmount(0){}
EnumType::~EnumType(){clear();}

void EnumType::setAmount(amountType amount){
	clear();
	enumArray=new string[amount];
	if(enumArray)enumAmount=amount;
}
EnumType::amountType EnumType::getAmount()const{return enumAmount;}
void EnumType::clear(){
	delete []enumArray;
	enumAmount=0;
}

bool EnumType::setEnumName(amountType index,const string &name){
	if(index>=enumAmount)return false;
	enumArray[index]=name;
	return true;
}
bool EnumType::getEnumName(amountType index,string &name)const{
	if(index>=enumAmount)return false;
	name=enumArray[index];
	return true;
}
bool EnumType::getEnumIndex(const string &name,amountType &index){
	for(amountType i=0;i<enumAmount;++i){
		if(enumArray[i]==name){
			index=i;
			return true;
		}
	}
	return false;
}