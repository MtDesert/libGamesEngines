#include "TranslationMap.h"

TranslationMap::TranslationMap():strMapArray(nullptr),mapAmount(0){}
TranslationMap::~TranslationMap(){clear();}

void TranslationMap::setAmount(amountType amount){
	clear();
	strMapArray=new StrMap[amount];
	if(strMapArray)mapAmount=amount;
	current=0;
}
TranslationMap::amountType TranslationMap::getAmount()const{return mapAmount;}
void TranslationMap::clear(){
	delete []strMapArray;
	mapAmount=0;
}

void TranslationMap::addStringMap(const string &key, const string &value){
	if(current<mapAmount){
		strMapArray[current].src=key;
		strMapArray[current].dest=value;
		++current;
	}
}