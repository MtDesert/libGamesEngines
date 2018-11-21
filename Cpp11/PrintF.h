#ifndef PRINTF_H
#define PRINTF_H

#include"typedef.h"
#include<set>
#include<string>
using namespace std;
#include<stdio.h>

struct PrintF{
	PrintF();
	enum Code{
		Default=0,
		Highlight=1,
		HalfBrightness=2,
		Underline=4,
		Twinkle=5,
		Reverse=7,
		NormalDensity=22,
		Non_Underline=24,
		Non_Twinkle=25,
		Non_Reverse=27,
		//foreground color
		Fore_Black=30,
		Fore_Red=31,
		Fore_Green=32,
		Fore_Brown=33,
		Fore_Blue=34,
		Fore_Purple=35,
		Fore_Cyan=36,
		Fore_White=37,
		Fore_Default_Underline=38,
		Fore_Default_Non_Underline=38,
		//background color
		Back_Black=40,
		Back_Red=41,
		Back_Green=42,
		Back_Brown=43,
		Back_Blue=44,
		Back_Purple=45,
		Back_Cyan=46,
		Back_White=47,
		Back_Default_Black=49
		//background color
	};

	set<uchar> codes;
	bool turnDefaultAfterPrintf;
	//code string
	string codesString()const;
	static string codesString_Default();
	string codesStringFormat(const char *format)const;

	//printf....
	int printf(const char *format,...)const;
	int vprintf(const char *format,va_list args)const;

	int sprintf(char *buffer,const char *format,...)const;
	int vsprintf(char *buffer,const char *format,va_list args)const;

	int fprintf(FILE *file,const char *format,...)const;
	int vfprintf(FILE *file,const char *format,va_list args)const;

	int snprintf(char *buffer,size_t maxlen,const char *format,...)const;
	int vsnprintf(char *buffer,size_t maxlen,const char *format,va_list args)const;

	//std::string
	int snprintf(string &str,const char *format,...)const;
	string format(const char *format,...)const;

	//print Hex(Diff highlight)
#define PRINTF_PRINTHEX(size)\
	void printHex(uint##size num)const;\
	void printHexDiff(uint##size num,uint##size correct,const PrintF &mismatch)const;
	
	PRINTF_PRINTHEX(8)
	PRINTF_PRINTHEX(16)
	PRINTF_PRINTHEX(32)
	PRINTF_PRINTHEX(64)
	
	void printHex8(void *ptr,uint len,uint start=0,uint countPerLine=16)const;//print a block of hex, describe by ptr & len
	void printHex8(void *ptr,void *correct,uint len,const PrintF &mismatch)const;//print a block of hex, describe by ptr & len, if diff from correct, use mismatch to print the diff part
};
#endif
