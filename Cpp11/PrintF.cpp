#include"PrintF.h"
#include<sstream>
using namespace std;
#include<stdio.h>
#include<stdarg.h>

PrintF::PrintF():turnDefaultAfterPrintf(true){}
string PrintF::codesString()const{
	stringstream ss;
	if(codes.size()){
		ss<<"\033[";
		//first
		auto itr=codes.begin();
		ss<<(int)(*itr);
		++itr;
		//next
		for(;itr!=codes.end();++itr){
			ss<<";"<<(int)(*itr);
		}
		ss<<"m";
	}
	return ss.str();
}
string PrintF::codesString_Default(){return "\033[0m";}
string PrintF::codesStringFormat(const char *format)const{
	return codesString()+format+(turnDefaultAfterPrintf?codesString_Default():"");
}

int PrintF::printf(const char *format,...)const{
	va_list args;
	va_start(args,format);
	int ret=vprintf(format,args);
	va_end(args);
	return ret;
}
int PrintF::vprintf(const char *format,va_list args)const{
	return ::vprintf(codesStringFormat(format).data(),args);
}

int PrintF::sprintf(char *buffer,const char *format,...)const{
	va_list args;
	va_start(args,format);
	int ret=vsprintf(buffer,format,args);
	va_end(args);
	return ret;
}
int PrintF::vsprintf(char *buffer,const char *format,va_list args)const{
	return ::vsprintf(buffer,codesStringFormat(format).data(),args);
}

int PrintF::fprintf(FILE *file,const char *format,...)const{
	va_list args;
	va_start(args,format);
	int ret=vfprintf(file,format,args);
	va_end(args);
	return ret;
}
int PrintF::vfprintf(FILE *file,const char *format,va_list args)const{
	return ::vfprintf(file,codesStringFormat(format).data(),args);
}

int PrintF::snprintf(char *buffer, size_t maxlen, const char *format,...)const{
	va_list args;
	va_start(args,format);
	int ret=vsnprintf(buffer,maxlen,format,args);
	va_end(args);
	return ret;
}
int PrintF::vsnprintf(char *buffer,size_t maxlen,const char *format,va_list args)const{
	return ::vsnprintf(buffer,maxlen,codesStringFormat(format).data(),args);
}

#define PRINTF_SNPRINTF\
	str.clear();\
	va_list args;\
	va_start(args,format);\
	int size=vsnprintf(nullptr,0,format,args);\
	va_end(args);\
	if(size){\
	str.resize(size+1);\
	va_start(args,format);\
	vsnprintf((char*)str.data(),str.size(),format,args);\
	va_end(args);\
	}

int PrintF::snprintf(string &str,const char *format,...)const{
	PRINTF_SNPRINTF
	return size;
}
string PrintF::format(const char *format,...)const{
	string str;
	PRINTF_SNPRINTF
	return str;
}

void PrintF::printHex(uint8 num)const{printf("%.2X",num);}
void PrintF::printHex(uint16 num)const{printf("%.4X",num);}
void PrintF::printHex(uint32 num)const{printf("%.8X",num);}
void PrintF::printHex(uint64 num)const{printf("%.16X",num);}

void PrintF::printHexDiff(uint8 num,uint8 correct,const PrintF &mismatch)const{
	((num&0xF0)==(correct&0xF0)?*this:mismatch).printf("%.1X",num>>4);
	((num&0x0F)==(correct&0x0F)?*this:mismatch).printf("%.1X",num&0x0F);
}
void PrintF::printHexDiff(uint16 num,uint16 correct,const PrintF &mismatch)const{
	uint8 *u1=(uint8*)&num,*u2=(uint8*)&correct;
	printHexDiff(u1[1],u2[1],mismatch);
	printHexDiff(u1[0],u2[0],mismatch);
}
void PrintF::printHexDiff(uint32 num,uint32 correct,const PrintF &mismatch)const{
	uint16 *u1=(uint16*)&num,*u2=(uint16*)&correct;
	printHexDiff(u1[1],u2[1],mismatch);
	printHexDiff(u1[0],u2[0],mismatch);
}
void PrintF::printHexDiff(uint64 num,uint64 correct,const PrintF &mismatch)const{
	uint32 *u1=(uint32*)&num,*u2=(uint32*)&correct;
	printHexDiff(u1[1],u2[1],mismatch);
	printHexDiff(u1[0],u2[0],mismatch);
}

void PrintF::printHex8(void *ptr,uint len,uint start,uint countPerLine)const{
	if(!ptr || countPerLine==0)return;
	for(uint i=0;i<len;++i){
		if(i%countPerLine==0){//new line,output address range
			if(i/countPerLine)printf("\n");
			printf("%.8X-%.8X: ",start+i,
				i+countPerLine<=len?
				start+i+countPerLine-1:
				start+len-1);
		}
		printHex(((uint8*)ptr)[i]);
		if(i<len-1)::printf(" ");
	}
	printf("\n");
}
void PrintF::printHex8(void *ptr, void *correct, uint len, const PrintF &mismatch)const{
	if(!ptr||!correct)return;
	for(uint i=0;i<len;++i){
		printHexDiff(((uint8*)ptr)[i],((uint8*)correct)[i],mismatch);
		if(i<len-1)printf(" ");
	}
}