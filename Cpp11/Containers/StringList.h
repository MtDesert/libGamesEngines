#ifndef STRINGLIST_H
#define STRINGLIST_H

#include<string>
#include<list>
using namespace std;

//字符串列表,目前提供排序功能
class StringList:public list<string>{
public:
	void sortByName(bool ascending=true);//按名字排序
	void sortByLength(bool ascending=true);//按长度排序
	void sortBySuffix(bool ascending=true);//按后缀排序,默认为'.',主要支持扩展名排序
	void splitString(const string &str,const string &splt,bool clearBeforeDo=true);//分离字符串str,分隔符为splt,保存在自身中
};

#endif
