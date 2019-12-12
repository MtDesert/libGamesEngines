#include"StringList.h"
#include"typedef.h"

//升降序的判断函数(函数名,先行代码,最终左代码,最终右代码)
#define STRINGLIST_COMPARE(name,code,codeL,codeR)\
static bool a##name(const string &a,const string &b){code return codeL<codeR;}\
static bool d##name(const string &a,const string &b){code return codeL>codeR;}

STRINGLIST_COMPARE(Length,,a.length(),b.length())
STRINGLIST_COMPARE(Name,,a.compare(b),0)
STRINGLIST_COMPARE(Suffix,
	auto aa=a.substr(a.find_last_of("."));auto bb=b.substr(b.find_last_of("."));,
	aa.compare(bb),0)

#define STRINGLIST_SORT(name)\
void StringList::sortBy##name(bool ascending){\
	sort(ascending?a##name:d##name);\
}

STRINGLIST_SORT(Name)
STRINGLIST_SORT(Length)
STRINGLIST_SORT(Suffix)

void StringList::splitString(const string &str,const string &splt,bool clearBeforeDo){
	if(str.length()==0)return;
	if(clearBeforeDo)clear();
	SizeType start=0,fin=0;
	do{
		fin=str.find(splt,start);
		push_back(str.substr(start,fin));
		start=fin + splt.length();
	}while(fin!=string::npos);
}