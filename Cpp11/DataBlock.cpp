#include"DataBlock.h"
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

#ifndef __MINGW32__
#include<sys/mman.h>
#endif

DataBlock::DataBlock():dataPointer(nullptr),dataLength(0),dataFile(nullptr),dataOffset(0){}
DataBlock::DataBlock(const DataBlock &dataBlock){*this=dataBlock;}
DataBlock::~DataBlock(){}

bool DataBlock::openFile(const string &filename,const string &mode){
	dataFile=::fopen(filename.data(),mode.data());
	if(dataFile){//打开成功,那就获取文件大小先存起来
		if(::fseek(dataFile,0,SEEK_END))return false;
		set_DataLength(::ftell(dataFile));//得到大小
		if(::fseek(dataFile,0,SEEK_SET))return false;
	}else{
		::perror(("DataBlock::openFile(\""+filename+"\"):").data());//输出系统错误信息
	}
	return dataFile;
}
bool DataBlock::openFileRead(const string &filename){
	return openFile(filename,"rb");
}
bool DataBlock::openFileReadUpdate(const string &filename){
	return openFile(filename,"r+b");
}
bool DataBlock::openFileWrite(const string &filename,const string &mode)const{
	FILE *file=::fopen(filename.data(),mode.data());
	if(!file){
		::perror(("DataBlock::openFileWrite(\""+filename+"\")--fopen():").data());
		return false;
	}
	if(::fwrite(dataPointer,dataLength,1,file)!=1){
		::perror(("DataBlock::openFileWrite(\""+filename+"\")--fwrite():").data());
	}
	if(::fflush(file)==EOF){
		::perror(("DataBlock::openFileWrite(\""+filename+"\")--fflush():").data());
		return false;
	}
	return ::fclose(file)==0;
}

bool DataBlock::closeFile(){
	if(dataFile){
		auto ret=::fclose(dataFile);//关闭文件
		dataFile=nullptr;
		if(ret==EOF){
			::perror("DataBlock::closeFile(): ");
		}
		return ret!=EOF;
	}return false;
}

bool DataBlock::loadFile(const string &filename){
	if(openFileRead(filename)){//打开文件
		if(newDataPointer(dataLength)){//申请存储空间
			if(::fread(dataPointer,dataLength,1,dataFile)!=1){
				::perror(("DataBlock::loadFile(\""+filename+"\"):").data());//读取出现问题
				return false;
			}
		}
		return closeFile();
	}return false;
}
bool DataBlock::saveFile(const string &filename)const{
	return openFileWrite(filename,"wb");
}
bool DataBlock::appendFile(const string &filename)const{
	return openFileWrite(filename,"ab");
}

bool DataBlock::memoryMap(const string &name){
#ifdef __MINGW32__
	return false;//mingw has not sys/mmap.h
#else
	struct stat theStat;
	int ret=stat(name.data(),&theStat);
	if(ret==-1){//stat()出错
		char strErrno[10];
		sprintf(strErrno,"%d",errno);
		::perror(("DataBlock::memoryMap(\""+name+"\")--stat():errno=="+strErrno).data());//出现问题
		return false;
	}

	auto fd=open(name.data(),O_RDWR);//读写模式打开
	if(fd==-1){
		char strErrno[10];
		sprintf(strErrno,"%d",errno);
		::perror(("DataBlock::memoryMap(\""+name+"\")--open():errno=="+strErrno).data());//出现问题
		return false;
	}

	dataPointer=(uchar*)::mmap(0,theStat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(dataPointer==MAP_FAILED){
		char strErrno[10];
		sprintf(strErrno,"%d",errno);
		::perror(("DataBlock::memoryMap(\""+name+"\")--mmap():errno=="+strErrno).data());//出现问题
		return false;
	}
	set_DataLength(theStat.st_size);

	return true;
#endif
}
bool DataBlock::memoryUnmap(){
#ifdef __MINGW32__
	return false;//mingw has not sys/mmap.h
#else
	return munmap(dataPointer,dataLength)!=(long)MAP_FAILED;
#endif
}

//stdlib.h
bool DataBlock::memoryCAllocate(size_t n,size_t size){
	dataPointer=(uchar*)::calloc(n,size);
	if(!dataPointer){
		::perror("DataBlock::memoryCAllocate(): ");
	}
	set_DataLength(dataPointer?n*size:0);
	return dataPointer;
}
bool DataBlock::memoryAllocate(size_t size){
	dataPointer=(uchar*)::malloc(size);
	if(!dataPointer){
		::perror("DataBlock::memoryAllocate(): ");
	}
	set_DataLength(dataPointer?size:0);
	return dataPointer;
}
bool DataBlock::memoryReallocate(size_t size){
	uchar *ptr=(uchar*)realloc(dataPointer,size);
	if(ptr){
		dataPointer=ptr;
		set_DataLength(size);
		return true;
	}else if(size==0){//free!!!!
		::perror("DataBlock::memoryReallocate(): ");
		set_DataLength(0);
	}//else do nothing(keep)
	return false;
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
	string ret;
	ret.resize(dataLength);
	DataBlock tmp;
	tmp.dataPointer=(uchar*)ret.data();//hack!!!!
	subDataBlock(0,dataLength,tmp,true);
	return ret;
}

//#define DATABLOK_NEWDATAPOINTER_NOTHROW
bool DataBlock::newDataPointer(size_t size){
	if(dataPointer)return false;//指针必须是空的
#ifdef DATABLOK_NEWDATAPOINTER_NOTHROW //非异常版本
	dataPointer=new(std::nothrow) uchar[size];
	if(!dataPointer){
		::perror("Error DataBlock::newDataPointer(): ");
	}
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

int DataBlock::debug()const{
	return printf("DataBlock: ptr=%p file=%p off=%lu len=%lu\n",dataPointer,dataFile,dataOffset,dataLength);
}

DataBlock::SizeType DataBlock::reset(){
	dataPointer=nullptr;
	dataFile=nullptr;
	dataLength=0;
	dataOffset=0;
	return 0;
}

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

DataBlock DataBlock::subDataBlock(SizeType offset, SizeType length)const{
	DataBlock block;
	subDataBlock(offset,length,block);
	return block;
}
bool DataBlock::subDataBlock(SizeType offset, SizeType length, DataBlock &subBlock, bool copy)const{
	if(offset+length>dataLength)return false;
	if(dataFile){
		subBlock.dataFile=dataFile;
		subBlock.dataOffset=dataOffset+offset;//影响fread/fwrite
		if(copy){
			::fseek(dataFile,subBlock.dataOffset,SEEK_SET);
			return ::fread(subBlock.dataPointer,1,length,dataFile)==length;
		}else{
			subBlock.dataPointer=dataPointer;//可能null
			subBlock.set_DataLength(length);
		}
	}else{
		if(copy){
			return ::memcpy(subBlock.dataPointer,&dataPointer[offset],length);
		}else{
			subBlock.dataPointer=&dataPointer[offset];
			subBlock.set_DataLength(length);
		}
		subBlock.dataFile=dataFile;//may be null
		subBlock.dataOffset=dataOffset+offset;//no affect if not use fread/fwrite
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
bool DataBlock::setByte(SizeType offset, const uint8 value, uint8 mask){
	uint8 oldValue;
	if(get_uint8(offset,oldValue)){
		uint8 maskXor=0xFF-mask;
		uint8 constValue = oldValue & maskXor;//不需要变化的部分
		uint8 variableValue = value & mask;//需要变化的部分
		return set_uint8(offset,constValue|variableValue);
	}else return false;
}
