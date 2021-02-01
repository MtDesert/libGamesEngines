#ifndef PRINTF_H
#define PRINTF_H

#if 1//有些环境没有命令行输出,可能就不需要本类(比如某些单片机,早期游戏机等)
//负责命令行输出的模块
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
	//输出调试信息
	static int setColor(int foreground,int background);//设置颜色,以提示不同信息
	#define PRINT_FUNC(name) static int print##name(const char *format,...);
	PRINT_FUNC(Info)//输出调试信息,以告知用户当前执行到了哪步
	PRINT_FUNC(OK)//输出成功,表示顺利完成某任务
	PRINT_FUNC(Warn)//输出警告,表示出现了需要注意的问题
	PRINT_FUNC(Error)//输出错误,表示出现了致命错误
	PRINT_FUNC(Purple)//紫色信息
	PRINT_FUNC(Cyan)//青色信息
	PRINT_FUNC(White)//白色信息
};

#define PRINT_INFO(...) PrintF::printInfo(__VA_ARGS__);
#define PRINT_OK(...) PrintF::printOK(__VA_ARGS__);
#define PRINT_WARN(...) PrintF::printWarn(__VA_ARGS__);
#define PRINT_ERROR(...) PrintF::printError(__VA_ARGS__);
#define PRINT_PURPLE(...) PrintF::printPurple(__VA_ARGS__);
#define PRINT_CYAN(...) PrintF::printCyan(__VA_ARGS__);
#define PRINT_WHITE(...) PrintF::printWhite(__VA_ARGS__);

#else
#define PRINT_INFO(...)
#define PRINT_OK(...)
#define PRINT_WARN(...)
#define PRINT_ERROR(...)
#endif

#endif