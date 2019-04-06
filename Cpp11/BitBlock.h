#ifndef BITBLOCK_H
#define BITBLOCK_H

#include"DataBlock.h"

/*uintXX数的读写方法
offset数据的位偏移
length数据的位长度
value读/写的数据
reverse读写的数据是否需要反向处理
*/
#define BITBLOCK_BITS(num)\
bool get_Bits##num(uint offset,uint8 length,uint##num &value,bool reverse=false)const;\
bool set_Bits##num(uint offset,uint8 length,uint##num value,bool reverse=false);

//此方法用于BitBlock的子类源码中,提供访问位的方法
#define BITBLOCK_CUSTOM_BOOL_CPP(ClassName,Name,offset)\
bool ClassName::get##Name(bool &value)const{return getBit(offset,value);}\
bool ClassName::set##Name(bool value){return setBit(offset,value);}

//BitBlock的子类会用到的方法,可以获取某些跨多个连续字节的数据
#define BITBLOCK_CUSTOM_BITS(Name,size)\
bool get##Name(uint##size &value)const;\
bool set##Name(const uint##size &value);

//此方法用于BitBlock的子类源码中,提供访问跨多个连续字节的数据的方法
#define BITBLOCK_CUSTOM_BITS_CPP(ClassName,Name,size,offset,length)\
bool ClassName::get##Name(uint##size &value)const{return get_Bits##size(offset,length,value);}\
bool ClassName::set##Name(const uint##size &value){return set_Bits##size(offset,length,value);}

/*Bit块,和DataBlock不同的是,BitBlock用来指明数据是以bit来作为主要的存储方式,当然也需要提供基于bit的访问方式
*/
class BitBlock:public DataBlock{
public:
	BitBlock();
	BitBlock(const BitBlock &bitBlock);

	virtual void set_DataLength(uint length);//此数据不仅记录字节数,还记录bit数

	//指向dataPointer[0]的第几位,我们可以用d0~d7来描述一个字节的第几位
	SizeType bitPointer;
	//指出有多少个有效的bit数据可供访问
	SizeType bitLength;

	//获取子BitBlock
	bool subBitBlock(uint bitOff,uint bitLen,BitBlock &subBlock)const;

	//主要成员函数
	void normalizing();//让0<=bitPointer<=7,并修改dataPointer
	bool getBit(uint offset,bool &value)const;
	bool setBit(uint offset,bool value);

	BITBLOCK_BITS(8)
	BITBLOCK_BITS(16)
	BITBLOCK_BITS(32)
	BITBLOCK_BITS(64)
};
#endif