#ifndef PRINTF_H
#define PRINTF_H

#if 1//有些环境没有命令行输出,可能就不需要本类(比如某些单片机,早期游戏机等)
//负责命令行输出的模块
#include"typedef.h"

struct PrintF{
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
		//前景色
		Fore_Black=30,
		Fore_Red=31,
		Fore_Green=32,
		Fore_Yellow=33,
		Fore_Blue=34,
		Fore_Purple=35,
		Fore_Cyan=36,
		Fore_White=37,
		Fore_Default_Underline=38,
		Fore_Default_Non_Underline=38,
		//背景色
		Back_Black=40,
		Back_Red=41,
		Back_Green=42,
		Back_Yellow=43,
		Back_Blue=44,
		Back_Purple=45,
		Back_Cyan=46,
		Back_White=47,
		Back_Default_Black=49
		//background color
	};
	//输出10进制数字
	int printInt(int32 value);//输出有符号数
	int printUint(uint32 value);//输出无符号数
	//打印16进制
	int printHex(uint8 value);
	int printHex(uint16 value);
	int printHex(uint32 value);
	//输出字符串
	int printString(const void *addr,int maxLen=0);
	int printString(uint8 value);
	int printString(uint16 value);
	int printString(uint32 value);
	//输出字节
	int printByte(const void *addr,int maxLen=0);
	int printByte(uint8 value);
	int printByte(uint16 value);
	int printByte(uint32 value);
	//输出调试信息
	static int setColor(int foreground,int background=Back_Black);//设置颜色,以提示不同信息
	static int colorPrint(int foreColor,bool nextLine,const char *format,...);
};

#define PRINT_LINE(Color,...) PrintF::colorPrint(PrintF::Fore_##Color,true,__VA_ARGS__);
//打印带颜色的换行
#define PRINT_RED(...) PRINT_LINE(Red,__VA_ARGS__)
#define PRINT_GREEN(...) PRINT_LINE(Green,__VA_ARGS__)
#define PRINT_BLUE(...) PRINT_LINE(Blue,__VA_ARGS__)
#define PRINT_YELLOW(...) PRINT_LINE(Yellow,__VA_ARGS__)
#define PRINT_PURPLE(...) PRINT_LINE(Purple,__VA_ARGS__)
#define PRINT_CYAN(...) PRINT_LINE(Cyan,__VA_ARGS__)
#define PRINT_WHITE(...) PRINT_LINE(White,__VA_ARGS__)
//打印特定意义的换行
#define PRINT_INFO(...) PRINT_LINE(Blue,__VA_ARGS__)
#define PRINT_OK(...) PRINT_LINE(Green,__VA_ARGS__)
#define PRINT_WARN(...) PRINT_LINE(Yellow,__VA_ARGS__)
#define PRINT_ERROR(...) PRINT_LINE(Red,__VA_ARGS__)

#else
#define PRINT_RED(...)
#define PRINT_GREEN(...)
#define PRINT_BLUE(...)
#define PRINT_YELLOW(...)
#define PRINT_PURPLE(...)
#define PRINT_CYAN(...)
#define PRINT_WHITE(...)

#define PRINT_INFO(...)
#define PRINT_OK(...)
#define PRINT_WARN(...)
#define PRINT_ERROR(...)

#endif

#endif
