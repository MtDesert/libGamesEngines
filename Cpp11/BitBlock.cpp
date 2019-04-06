#include"BitBlock.h"
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<iostream>

BitBlock::BitBlock():DataBlock(),bitPointer(0),bitLength(0){}
BitBlock::BitBlock(const BitBlock &bitBlock){*this=bitBlock;}

void BitBlock::set_DataLength(uint length){
	DataBlock::set_DataLength(length);
	bitLength=dataLength*8;
}

bool BitBlock::subBitBlock(uint bitOff, uint bitLen, BitBlock &subBlock)const{
	uint byteOff=bitOff/8;
	uint byteLen=(bitOff+bitLen+7)/8;
	bool ret=subDataBlock(byteOff,byteLen,subBlock);
	if(ret){
		subBlock.bitPointer=bitOff%8;
		subBlock.bitLength=bitLen;
	}
	return ret;
}

void BitBlock::normalizing(){
	SizeType byte=0,bit=bitPointer;
	normalizedOffset(byte,bit);
	bitPointer=bit;
	dataPointer+=byte;
}
bool BitBlock::getBit(uint offset,bool &value)const{
	return getBool(bitPointer+offset,value);
}
bool BitBlock::setBit(uint offset,bool val){
	return setBool(bitPointer+offset,val);
}

#define BITBLOCK_BITS_CPP(num)\
bool BitBlock::get_Bits##num(uint offset,uint8 length,uint##num &value,bool reverse)const{\
	if(!length)return false;\
	if(length>num)length=num;\
	if(offset+length>bitLength)return false;\
	SizeType startByte=0,startBit=bitPointer+offset;\
	normalizedOffset(startByte,startBit);\
	uint8 byteCount=(startBit+length-1)/8+1;\
	uint8 byte[byteCount];\
	if(dataFile){\
		fseek(dataFile,startByte,SEEK_SET);\
		if(fread(byte,1,byteCount,dataFile)!=byteCount)return false;\
	}else{\
		::memcpy(byte,&dataPointer[startByte],byteCount);\
	}\
	value=0;\
	for(uint8 i=0;i<byteCount;++i){\
		if(i==0)value|=(byte[i]>>startBit);\
		else value|=(((uint##num)byte[i])<<(i*8-startBit));\
	}\
	if(length<num){\
		value&=(((uint##num)1)<<length)-1;\
	}\
	if(reverse){\
		reverseBit(value,0,length);\
	}\
	return true;\
}\
bool BitBlock::set_Bits##num(uint offset,uint8 length,uint##num value,bool reverse){\
	if(!length)return false;\
	if(length>num)length=num;\
	if(offset+length>bitLength)return false;\
	SizeType startByte=0,startBit=bitPointer+offset;\
	DataBlock::normalizedOffset(startByte,startBit);\
	uint##num mask=(length<num?(((uint##num)1)<<length)-1:((uint##num)0)-1);\
	if(reverse){\
		reverseBit(value,0,length);\
	}\
	uint8 temp;\
	uint8 byteCount=(startBit+length-1)/8+1;\
	for(uint i=0;i<byteCount;++i){\
		if(i==0){\
			if(!setByte(startByte+i,value<<startBit,mask<<startBit))return false;\
		}else{\
			temp=(i*8-startBit);\
			if(!setByte(startByte+1,value>>temp,mask>>temp))return false;\
		}\
	}\
	return true;\
}

BITBLOCK_BITS_CPP(8)
BITBLOCK_BITS_CPP(16)
BITBLOCK_BITS_CPP(32)
BITBLOCK_BITS_CPP(64)