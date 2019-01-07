#include"Charset.h"
#include<iconv.h>
#include<string.h>

#define STR_NAME(name) #name,
static const char* strCharset[]={//静态字符表
	CHARSET_ALL(STR_NAME)
};

size_t Charset::charAmount(const char *str,EnumCharset charset){
	size_t ret=0,pos=0,len=0,length=strlen(str);
	if(str){
		uchar byte=str[pos];
		while(byte){
			len=(byte<0x80);//测字符长度
			if(len!=1){//非ASCII的符号需要另外转换
				switch(charset){
					case GB2312:case GBK:len=2;break;//GB系列的汉字默认为2字节
					case GB18030:
						if(byte>=0x81 && byte<=0x82)len=4;//可能有4个字节的情况
						else len=2;
					break;
					case UTF8://UTF8要从首字节的最高位开始检测
						len=0;
						while(byte & 0x80){++len;byte<<=1;}//检测1的连续个数
					break;
					default:len=1;//未知的就先按1
				}
			}
			if(pos+len>length)break;//保证字符完整
			++ret;//统计
			pos+=len;//下一个
			byte=str[pos];
		}
	}
	return ret;
}

DataBlock Charset::newString(const char *str,EnumCharset fromCharset,EnumCharset toCharset){
	DataBlock ret;
	if(!str || fromCharset>=AmountOf_EnumCharset || toCharset>=AmountOf_EnumCharset)return ret;//防非法输入
	size_t prefixLen=0,bytesPerChar=1;//前缀字符个数和每个字符可能的个数
	switch(toCharset){//根据目标字符集确定每个字可能的最长长度
		case UNICODE:prefixLen=2;bytesPerChar=2;break;//iconv转换后自带2字节的头
		case GB2312:case GBK:bytesPerChar=2;break;
		case UTF8:bytesPerChar=3;break;
		case GB18030:bytesPerChar=4;break;
		default:;
	}
	size_t toLen=prefixLen+charAmount(str,fromCharset)*bytesPerChar;//测字符数,确定需要的转换空间
	if(ret.memoryAllocate(toLen)){//申请空间,不保证空间一定能用完
		ret.memset(0);
		size_t fromLen=strlen(str);//测字节数
		char *fromStr=(char*)str,*toStr=(char*)ret.dataPointer;//iconv用到的变量
		//iconv转换的完整过程
		iconv_t convert=iconv_open(strCharset[toCharset],strCharset[fromCharset]);
		iconv(convert,&fromStr,&fromLen,&toStr,&toLen);
		iconv_close(convert);
		//清除没用到的空间
		if(toLen){
			ret.memoryReallocate(ret.dataLength-toLen);
		}
	}
	return ret;
}