#include"PrintF.h"
#include<stdio.h>
#include<stdarg.h>

//打印10进制
int PrintF::printInt(int32 value){
	setColor(Fore_Green);
	return ::printf("%d",value);
}
int PrintF::printUint(uint32 value){
	setColor(Fore_Green);
	return ::printf("%u",value);
}
//打印16进制
int PrintF::printHex(uint8 value){
	setColor(Fore_Cyan);
	return ::printf("%.2X",value);
}
int PrintF::printHex(uint16 value){
	setColor(Fore_Cyan);
	return ::printf("%.4X",value);
}
int PrintF::printHex(uint32 value){
	setColor(Fore_Cyan);
	return ::printf("%.8X",value);
}
//打印字符串
int PrintF::printString(const void *addr,int maxLen){
	setColor(Fore_Purple);
	const char *str=(const char*)addr;
	if(maxLen>0){//限制长度
		for(int i=0;i<maxLen;++i){
			::printf("%c",str[i]);
		}
	}else{//不限制长度
		return ::printf("%s",str);
	}
	return 0;
}
int PrintF::printString(uint8 value){return printString(&value,1);}
int PrintF::printString(uint16 value){return printString(&value,2);}
int PrintF::printString(uint32 value){return printString(&value,4);}
//打印字节
int PrintF::printByte(const void *addr,int maxLen){
	setColor(Fore_Blue);
	const uint8 *str=(const uint8*)addr;
	for(int i=0;i<maxLen || maxLen<=0;++i){
		if(i)::printf(" ");
		::printf("%.2X",str[i]);
	}
	return 0;
}
int PrintF::printByte(uint8 value){return printByte(&value,1);}
int PrintF::printByte(uint16 value){return printByte(&value,2);}
int PrintF::printByte(uint32 value){return printByte(&value,4);}

int PrintF::setColor(int foreground,int background){
	return ::printf("\033[%d;%dm",foreground,background);
}
int PrintF::colorPrint(int foreColor,bool nextLine,const char *format,...){
	setColor(foreColor);
	va_list args;
	va_start(args,format);
	int ret=::vprintf(format,args);
	va_end(args);
	::printf("\033[0m");
	if(nextLine)::printf("\n");
	::fflush(stdout);
	return ret;
}
