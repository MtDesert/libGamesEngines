#ifndef CURLEASY_H
#define CURLEASY_H

#include"curl/curl.h"
#include<string>
#include<list>
using namespace std;

class CurlEasy{
public:
	CurlEasy();
	~CurlEasy();

	CURL *curl;//下载用
	int downloadFile(const string &url,const string &filename,int minSize=0)const;//下载文件
	int parseHtml(const string &filename);//解析html文件
	virtual void clearCache();//清除缓冲
	virtual void parseHtmlLine(char *buffer);//解析行,解析方法子类实现

	//解析过程的函数
	//解析内容(源数据行,开始标志,结束标志),返回解析结果,解析不出返回默认string
	static string parseContent(const char *line,const char *startMark,const char *finMark,bool reserveMark);
	static string getUrlFilename(const string &url);//根据url获取文件名
	static string getSuffix(const string &url);

	//错误处理
};
#endif