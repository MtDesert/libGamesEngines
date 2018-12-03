#ifndef CHARSET_H
#define CHARSET_H

#include"DataBlock.h"
//字符集,根据需要处理的种数来自行添加
#define CHARSET_ALL(MACRO)\
	MACRO(UNICODE)\
	MACRO(GB2312)\
	MACRO(GBK)\
	MACRO(GB18030)\
	MACRO(UTF8)

//字符集,用于支持字体处理程序的功能
class Charset{
public:
#define CHARSET_NAME(name) name,
	//字符集枚举
	enum EnumCharset{
		CHARSET_ALL(CHARSET_NAME)
		AmountOf_EnumCharset
	};
	//统计字符串str的字符数(charset为str的字符集)
	static size_t charAmount(const char *str,EnumCharset charset);
	//转换
	static DataBlock newString(const char *str,EnumCharset fromCharset,EnumCharset toCharset);
};

#endif
