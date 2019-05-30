#ifndef CURLEASY_H
#define CURLEASY_H

#include<curl/curl.h>
#include<string>
#include<list>
using namespace std;

//curl的c++封装版本,主要提供下载和一些分析功能
//主要用于爬虫类工具
class CurlEasy{
	CURL *curl;//下载用
public:
	CurlEasy();
	~CurlEasy();

	/**下载url的内容保存成filename文件*/
	int downloadFile(const string &url,const string &filename)const;
	//辅助分析模块
	/**解析内容(源数据行,开始标志,结束标志),返回开始标志到结束标志之间的内容,解析不出返回nullptr*/
	static string parseContent(char *line,const char *startMark,const char *finMark);
};
#endif