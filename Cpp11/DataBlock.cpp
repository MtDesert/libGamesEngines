#include"DataBlock.h"
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

DataBlock::DataBlock(){set();}
DataBlock::DataBlock(const DataBlock &dataBlock){*this=dataBlock;}
DataBlock::~DataBlock(){}

bool DataBlock::openFileWrite(const string &filename,const string &mode)const{
	FILE *file=::fopen(filename.data(),mode.data());
	if(file){
		::fwrite(dataPointer,dataLength,1,file);
		::fflush(file);
		return ::fclose(file)==0;
	}
	return false;
}

bool DataBlock::loadFile(const string &filename){
	auto dataFile=::fopen(filename.data(),"rb");
	if(dataFile){
		//获取内容大小
		if(::fseek(dataFile,0,SEEK_END))return false;
		set_DataLength(::ftell(dataFile));//得到大小
		if(::fseek(dataFile,0,SEEK_SET))return false;
		//申请内存,存储读取的数据
		if(newDataPointer(dataLength)){//申请存储空间
			if(::fread(dataPointer,dataLength,1,dataFile)!=1);
		}
		::fclose(dataFile);
	}
	return false;
}
bool DataBlock::saveFile(const string &filename)const{
	return openFileWrite(filename,"wb");
}
bool DataBlock::appendFile(const string &filename)const{
	return openFileWrite(filename,"ab");
}
bool DataBlock::fileWrite(FILE *file)const{return ::fwrite(dataPointer,dataLength,1,file);}

//stdlib.h
bool DataBlock::memoryCAllocate(size_t n,size_t size){
	if(dataPointer)return false;
	dataPointer=(uchar*)::calloc(n,size);
	set_DataLength(dataPointer?n*size:0);
	return dataPointer;
}
bool DataBlock::memoryAllocate(size_t size){
	if(dataPointer)return false;
	dataPointer=(uchar*)::malloc(size);
	set_DataLength(dataPointer?size:0);
	return dataPointer;
}
bool DataBlock::memoryReallocate(size_t size){
	dataPointer=(uchar*)realloc(dataPointer,size);
	set_DataLength(size);
	return dataPointer;
}
void DataBlock::memoryFree(){
	if(dataPointer)free(dataPointer);
	dataPointer=nullptr;
	set_DataLength(0);
}

//string.h
void* DataBlock::memchr(int chr,size_t num)const{
	if(num>dataLength)num=dataLength;
	return ::memchr(dataPointer,chr,num?num:dataLength);
}
int DataBlock::memcmp(const void *ptr,size_t num)const{
	if(num>dataLength)num=dataLength;
	return ::memcmp(dataPointer,ptr,num);
}
int DataBlock::memcmp(const DataBlock &block)const{return memcmp(block.dataPointer,block.dataLength);}
void* DataBlock::memcpyTo(void *dest,size_t num)const{
	if(num>dataLength)num=dataLength;
	return ::memcpy(dest,dataPointer,num);
}
void *DataBlock::memcpyFrom(const void *src,size_t num){
	if(num>dataLength)num=dataLength;
	return ::memcpy(dataPointer,src,num);
}
void* DataBlock::memmoveTo(void *dest,size_t num) const{
	if(num>dataLength)num=dataLength;
	return ::memmove(dest,dataPointer,num);
}
void *DataBlock::memmoveFrom(void *src,size_t num){
	if(num>dataLength)num=dataLength;
	return ::memmove(dataPointer,src,num);
}
void* DataBlock::memset(int val,size_t len){return ::memset(dataPointer,val,len?len:dataLength);}

string DataBlock::toString()const{
	return dataPointer ? string((char*)dataPointer,dataLength):string();
}

#define DATABLOK_NEWDATAPOINTER_NOTHROW
bool DataBlock::newDataPointer(size_t size){
	if(dataPointer)return false;//指针必须是空的
#ifdef DATABLOK_NEWDATAPOINTER_NOTHROW //非异常版本
	dataPointer=new(std::nothrow) uchar[size];
#else //扔异常版本
	try{
		dataPointer=new uchar[size];
	}catch(const bad_alloc& e){
		::printf("Error DataBlock::newDataPointer(): %s",e.what());
	}
#endif
	set_DataLength(size);
	return dataPointer;
}
bool DataBlock::deleteDataPointer(){
	bool ret=dataPointer;
	if(dataPointer)delete []dataPointer;
	dataPointer=nullptr;
	return ret;
}

void DataBlock::set(const void *ptr, SizeType length){
	dataPointer=(decltype(dataPointer))ptr;
	dataLength=length;
}
void DataBlock::set(const DataBlock &block){set(block.dataPointer,block.dataLength);}

DATABLOCK_TYPE_CPP(int8)
DATABLOCK_TYPE_CPP(int16)
DATABLOCK_TYPE_CPP(int32)
DATABLOCK_TYPE_CPP(int64)

DATABLOCK_TYPE_CPP(uint8)
DATABLOCK_TYPE_CPP(uint16)
DATABLOCK_TYPE_CPP(uint32)
DATABLOCK_TYPE_CPP(uint64)

DATABLOCK_TYPE_CPP(wchar_t)
DATABLOCK_TYPE_CPP(char16_t)
DATABLOCK_TYPE_CPP(char32_t)
DATABLOCK_TYPE_CPP(float)
DATABLOCK_TYPE_CPP(double)

bool DataBlock::get_string(SizeType offset,string &value)const{
	if(dataPointer==nullptr || offset>=dataLength)return false;
	value=(char*)&dataPointer[offset];
	return true;
}
bool DataBlock::set_string(SizeType offset,const string &value){
	if(dataPointer==nullptr || offset>=dataLength)return false;
	SizeType sz=min(dataLength-offset,value.size());
	return memcpy(&dataPointer[offset],value.data(),sz);
}

DataBlock DataBlock::subDataBlock(SizeType offset, SizeType length)const{
	DataBlock block;
	subDataBlock(offset,length,block);
	return block;
}
bool DataBlock::subDataBlock(SizeType offset, SizeType length, DataBlock &subBlock, bool copy)const{
	if(offset+length>dataLength)return false;
	if(copy){
		return ::memcpy(subBlock.dataPointer,&dataPointer[offset],length);
	}else{
		subBlock.dataPointer=&dataPointer[offset];
		subBlock.set_DataLength(length);
	}
	return true;
}

void DataBlock::reverseBit(uint8 &byte){
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
}
void DataBlock::reverseBit(uint16 &u16){
	u16 = (u16 & 0xAAAA) >> 1 | (u16 & 0x5555) << 1;
	u16 = (u16 & 0xCCCC) >> 2 | (u16 & 0x3333) << 2;
	u16 = (u16 & 0xF0F0) >> 4 | (u16 & 0x0F0F) << 4;
	u16 = (u16 & 0xFF00) >> 8 | (u16 & 0x00FF) << 8;
}
void DataBlock::reverseBit(uint32 &u32){
	u32 = (u32 & 0xAAAAAAAA) >> 1 | (u32 & 0x55555555) << 1;
	u32 = (u32 & 0xCCCCCCCC) >> 2 | (u32 & 0x33333333) << 2;
	u32 = (u32 & 0xF0F0F0F0) >> 4 | (u32 & 0x0F0F0F0F) << 4;
	u32 = (u32 & 0xFF00FF00) >> 8 | (u32 & 0x00FF00FF) << 8;
	u32 = (u32 & 0xFFFF0000) >>16 | (u32 & 0x0000FFFF) << 16;
}
void DataBlock::reverseBit(uint64 &u64){
	u64 = (u64 & 0xAAAAAAAAAAAAAAAA) >> 1 | (u64 & 0x5555555555555555) << 1;
	u64 = (u64 & 0xCCCCCCCCCCCCCCCC) >> 2 | (u64 & 0x3333333333333333) << 2;
	u64 = (u64 & 0xF0F0F0F0F0F0F0F0) >> 4 | (u64 & 0x0F0F0F0F0F0F0F0F) << 4;
	u64 = (u64 & 0xFF00FF00FF00FF00) >> 8 | (u64 & 0x00FF00FF00FF00FF) << 8;
	u64 = (u64 & 0xFFFF0000FFFF0000) >>16 | (u64 & 0x0000FFFF0000FFFF) << 16;
	u64 = (u64 & 0xFFFFFFFF00000000) >>32 | (u64 & 0x00000000FFFFFFFF) << 32;
}

#define DATABLOCK_REVERSE_BIT(size)\
void DataBlock::reverseBit(uint##size &value,uint8 low,uint8 high){\
	if(low>size-1)low=size-1;\
	if(high>size-1)high=size-1;\
	if(low>high){\
		uint##size tmp=low;low=high;high=tmp;\
	}\
	uint##size a1,a2,b1,b2;\
	for(;low<high;++low,--high){\
		a1=(1<<low);a2=value&a1;\
		b1=(1<<high);b2=value&b1;\
		if((a2&&!b2)||(!a2&&b2)){\
			value=value^(a1|b1);\
		}\
	}\
}

DATABLOCK_REVERSE_BIT(8)
DATABLOCK_REVERSE_BIT(16)
DATABLOCK_REVERSE_BIT(32)
DATABLOCK_REVERSE_BIT(64)

void DataBlock::normalizedOffset(SizeType &byte,SizeType &bit){
	if(bit>=8){
		byte += bit / 8;
		bit %= 8;
	}
}
bool DataBlock::getBool(SizeType offset,bool &value)const{
	return getBool(offset/8,offset%8,value);
}
bool DataBlock::getBool(SizeType byteOffset, SizeType bitOffset, bool &value)const{
	normalizedOffset(byteOffset,bitOffset);
	uint8 byte;
	if(getByte(byteOffset,byte,1<<bitOffset)){
		value=byte;
		return true;
	}else return false;
}
bool DataBlock::setBool(SizeType offset,bool value){
	return setBool(offset/8,offset%8,value);
}
bool DataBlock::setBool(SizeType byteOffset,SizeType bitOffset,bool value){
	normalizedOffset(byteOffset,bitOffset);
	uint8 bit=1<<bitOffset;
	return setByte(byteOffset,value?bit:0,bit);
}

bool DataBlock::getByte(SizeType offset,uint8 &value,uint8 mask)const{
	if(get_uint8(offset,value)){
		value&=mask;
		return true;
	}else return false;
}
bool DataBlock::setByte(SizeType offset,const uint8 value,uint8 mask){
	uint8 oldValue;
	if(get_uint8(offset,oldValue)){
		uint8 maskXor=0xFF-mask;
		uint8 constValue = oldValue & maskXor;//不需要变化的部分
		uint8 variableValue = value & mask;//需要变化的部分
		return set_uint8(offset,constValue|variableValue);
	}else return false;
}

uint DataBlock::leastUintToStoreBit(uint8 bitLen){
	uint8 ret=0;
	if(0<bitLen && bitLen<=8)ret=1;
	else if(bitLen<=16)ret=2;
	else if(bitLen<=32)ret=4;
	else if(bitLen<=64)ret=8;
	return ret;
}

#define DATABLOCK_CPP_TO_ARRAY(bit)\
bool DataBlock::toUint##bit##Array(uint8 bitLen,SizeType amount,bool littleEndian,uint##bit uintArray[])const{\
	/*检查参数合法性*/\
	if(bitLen==0||bitLen>64)return false;\
	if(amount*bitLen>dataLength*8)return false;\
	/*开始解数据*/\
	SizeType offset=0;\
	uint##bit value;\
	uint8 byte=0,remainBits=0,valueSize=0;\
	for(decltype(amount) i=0;i<amount;++i){\
		value=valueSize=0;\
		while(valueSize<bitLen){/*生成需要的数值*/\
			/*补充数据*/\
			if(remainBits==0){\
				getByte(offset,byte);\
				remainBits=8;\
				++offset;\
			}\
			/*根据大尾或小尾来取数据*/\
			if(littleEndian){\
				value|=((uint##bit)(byte&0x01)<<valueSize);\
				byte>>=1;\
			}else{\
				value<<=1;\
				value|=((byte&0x80)>>7);\
				byte<<=1;\
			}\
			/*更新状态*/\
			++valueSize;\
			--remainBits;\
		}\
		/*写入数组*/\
		uintArray[i]=value;\
	}\
	return true;\
}\
bool DataBlock::fromUint##bit##Array(uint##bit uintArray[],SizeType amount,uint8 bitLen,bool littleEndian){\
	/*检查参数合法性*/\
	if(bitLen==0||bitLen>64)return false;\
	if(amount*bitLen>dataLength*8)return false;\
	/*开始编数据*/\
	SizeType offset=0;\
	uint##bit value;\
	uint8 byte=0,usedBits=0,valueSize=0;\
	for(decltype(amount) i=0;i<amount;++i){\
		value=uintArray[i];\
		valueSize=bitLen;\
		while(valueSize>0){/*生成需要的数值*/\
			--valueSize;\
			/*根据大尾或小尾来取数据*/\
			if(littleEndian){\
				byte|=((value&0x01)<<usedBits);\
				value>>=1;\
			}else{\
				byte|=(((value>>valueSize)&0x01)<<(7-usedBits));\
			}\
			/*更新状态*/\
			++usedBits;\
			/*写入数据*/\
			if(usedBits>=8){\
				setByte(offset,byte);\
				byte=0;\
				usedBits=0;\
				++offset;\
			}\
		}\
	}\
	setByte(offset,byte);/*确保写入最后一个字节*/\
	return true;\
}

DATABLOCK_CPP_TO_ARRAY(8)
DATABLOCK_CPP_TO_ARRAY(16)
DATABLOCK_CPP_TO_ARRAY(32)
DATABLOCK_CPP_TO_ARRAY(64)

/*void DataBlock::debug()const{
	printf("DataBlock:%p %lu\n",dataPointer,dataLength);
	if(!dataPointer)return;
	for(SizeType i=0;i<dataLength;++i){
		printf("%.2X ",dataPointer[i]);
	}
	printf("\n");
}*/