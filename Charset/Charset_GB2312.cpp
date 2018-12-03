#include"Charset_GB2312.h"
#include<stdio.h>

uint16 Charset_GB2312::spCode_gbCode(uint16 code){return code+0x2020;}
uint16 Charset_GB2312::spCode_internalCode(uint16 code){return code+0xA0A0;}
uint16 Charset_GB2312::gbCode_spCode(uint16 code){return code-0x2020;}
uint16 Charset_GB2312::gbCode_internalCode(uint16 code){return code+0x8080;}
uint16 Charset_GB2312::internalCode_spCode(uint16 code){return code-0xA0A0;}
uint16 Charset_GB2312::internalCode_gbCode(uint16 code){return code-0x8080;}

bool Charset_GB2312::charIndex(uint16 character,uint8 minSec,uint8 maxSec,uint8 maxPosOfLastSec,uint16 &index){
	uint16 spCode=internalCode_spCode(character);
	uint8 secCode = spCode & 0xFF;
	if(secCode>=minSec && secCode<=maxSec){
		uint8 posCode=spCode>>8;
		if(posCode>=1 && posCode<=94){
			if(secCode==maxSec && posCode>maxPosOfLastSec){//处理最后一行的特殊情况
				return false;
			}
			index=(secCode-minSec)*94;//直接根据规律计算
			index+=posCode-1;
			return true;
		}
	}
	return false;
}
bool Charset_GB2312::charIndex_Ascii(uint8 character,uint8 &index){
	if(character>=0x20 && character<0x7F){
		index=character-0x20;
		return true;
	}
	return false;
}
bool Charset_GB2312::charIndex_Symbol(uint16 character,uint16 &index){
	return charIndex(character,1,9,94,index);
}
bool Charset_GB2312::charIndex_lv1Chinese(uint16 character,uint16 &index){
	return charIndex(character,16,55,89,index);
}
bool Charset_GB2312::charIndex_lv2Chinese(uint16 character,uint16 &index){
	return charIndex(character,56,87,94,index);
}
