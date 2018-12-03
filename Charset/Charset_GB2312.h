#ifndef CHARSET_GB2312_H
#define CHARSET_GB2312_H

#include"typedef.h"

/*国标2312,中国国家标准总局1980年发布
区位码,2字节,由区码(section-code,首字节)和位码(position-code,次字节)组成
区位码的划分:各类符号(区码1~9),一级汉字(区码16~55),二级汉字(56~87)
国标码 = 区位码+0x2020
机内码 = 国标码+0x8080 = 区位码+0xA0A0
我们在Windows下存储的文本文件中的中文默认就是机内码(即gb2312,除非设置成其它字符集)
*/
class Charset_GB2312{
public:
	//转换(spCode区位码,gbCode国标码,internalCode机内码)
	static uint16 spCode_gbCode(uint16 spCode);
	static uint16 spCode_internalCode(uint16 spCode);
	static uint16 gbCode_spCode(uint16 gbCode);
	static uint16 gbCode_internalCode(uint16 gbCode);
	static uint16 internalCode_spCode(uint16 iCode);
	static uint16 internalCode_gbCode(uint16 iCode);
	
	//查询(输入character以及区码范围以及末区最大的位码,输出index),返回是否成功(这可以用于游戏机的点阵字体查询)
	static bool charIndex(uint16 character,uint8 minSec,uint8 maxSec,uint8 maxPosOfLastSec,uint16 &index);
	//查询(输入character,输出index),返回是否成功
	static bool charIndex_Ascii(uint8 character,uint8 &index);
	static bool charIndex_Symbol(uint16 character,uint16 &index);
	static bool charIndex_lv1Chinese(uint16 character,uint16 &index);
	static bool charIndex_lv2Chinese(uint16 character,uint16 &index);
};

#endif
