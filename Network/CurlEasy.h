#ifndef CURLEASY_H
#define CURLEASY_H

#include<curl/curl.h>
#include<string>
#include<list>
using namespace std;

//curl的c++封装版本
class CurlEasy{
	CURL *curl;//下载用
public:
	CurlEasy();
	~CurlEasy();

	int downloadFile(const string &url,const string &filename)const;//下载url的内容保存成filename文件
};
#endif