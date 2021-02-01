#include"PrintF.h"
#include<stdio.h>
#include<stdarg.h>

int PrintF::setColor(int foreground,int background){
	return ::printf("\033[%d;%dm",foreground,background);
}
#define PRINT_VAR_LIST(name,fore) \
int PrintF::print##name(const char *format,...){\
	setColor(Fore_##fore,Back_Black);\
	va_list args;\
	va_start(args,format);\
	int ret=::vprintf(format,args);\
	va_end(args);\
	::printf("\033[0m\n");\
	::fflush(stdout);\
	return ret;\
}

PRINT_VAR_LIST(Info,Blue)
PRINT_VAR_LIST(OK,Green)
PRINT_VAR_LIST(Warn,Yellow)
PRINT_VAR_LIST(Error,Red)
PRINT_VAR_LIST(Purple,Purple)
PRINT_VAR_LIST(Cyan,Cyan)
PRINT_VAR_LIST(White,White)