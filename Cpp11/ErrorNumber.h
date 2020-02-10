#ifndef ERRORNUMBER_H
#define ERRORNUMBER_H

//错误号,用于获取错误描述(中文)
class ErrorNumber{
public:
	static void init();//在调用getErrorString前应调用此函数进行初始化
	static const char* getErrorString(int errorNumber);//根据errorNumber来获取错误文字(中文)
};
#endif