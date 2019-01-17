#ifndef DATABLOCK_H
#define DATABLOCK_H

#include"typedef.h"
#include<string>
using namespace std;

//基础数据类型的get/set函数声明(基于字节操作),也可以用于枚举,返回是否get/set成功
//Type是基础类型(枚举亦可),offset代表偏移量,用于获取不同位置的数据
#define DATABLOCK_TYPE(Type)\
bool get_##Type(SizeType offset,Type &value)const;\
bool set_##Type(SizeType offset,const Type &value);

//基础数据类型的get/set函数的定义,这个宏用在DataBlock.cpp中(也可以用于子类的源文件中)
#define DATABLOCK_TYPE_CPP(Type)\
bool DataBlock::get_##Type(SizeType offset,Type &value)const{\
	if(offset+sizeof(Type)>dataLength)return false;\
	if(dataFile){\
		auto off=dataOffset+offset;\
		if((SizeType)::ftell(dataFile)!=off)::fseek(dataFile,off,SEEK_SET);\
		return ::fread(&value,1,sizeof(Type),dataFile)==sizeof(Type);\
	}else{\
		value=*((Type*)(&dataPointer[offset]));\
	}\
	return true;\
}\
bool DataBlock::set_##Type(SizeType offset,const Type &value){\
	if(dataFile){\
		auto off=dataOffset+offset;\
		if(off+sizeof(Type)>dataLength)return false;\
		if((SizeType)::ftell(dataFile)!=off)::fseek(dataFile,off,SEEK_SET);\
		return ::fwrite(&value,1,sizeof(Type),dataFile)==sizeof(Type);\
	}else{\
		if(offset+sizeof(Type)>dataLength)return false;\
		*((Type*)(&dataPointer[offset]))=value;\
	}\
	return true;\
}

//用户自定义的字段声明,用于DataBlock的子类,返回是否get/set成功
//Name代表属性名(建议首字母大写),Type代表基础类型或者枚举
#define DATABLOCK_CUSTOM_TYPE(Name,Type)\
bool get##Name(Type &value)const;\
bool set##Name(const Type &value);\

//用户自定义的字段定义,用于DataBlock的子类的源文件中
//ClassName子类名
//Name属性名
//Type属性类型
//offset属性的字节偏移位置
//littleEndian数据是否为小尾序列
#define DATABLOCK_CUSTOM_TYPE_CPP(ClassName,Name,Type,offset,littleEndian)\
bool ClassName::get##Name(Type &value)const{\
	if(get_##Type(offset,value)){\
		if(!littleEndian)reverseByte(value);\
		return true;\
	}else {return false;}\
}\
bool ClassName::set##Name(const Type &value){\
	if(littleEndian)return set_##Type(offset,value);\
	else{\
		Type v=value;\
		reverseByte(v);\
		return set_##Type(offset,v);\
	}\
}

//用户自定义的bool属性字段,用于DataBlock的子类,返回是否get/set成功
#define DATABLOCK_CUSTOM_BOOL(Name)\
bool get##Name(bool &value)const;\
bool set##Name(bool value);

//用户自定义的bool属性字段,用于DataBlock的子类的源文件中
//ClassName子类名
//Name属性名
//byteOffset属性的字节偏移位置
//bitOffset属性的位偏移位置
#define DATABLOCK_CUSTOM_BOOL_CPP(ClassName,Name,byteOffset,bitOffset)\
bool ClassName::get##Name(bool &value)const{return getBool(byteOffset,bitOffset,value);}\
bool ClassName::set##Name(bool value){return setBool(byteOffset,bitOffset,value);}

/**
*此类名为"数据块",用来描述一段内存中连续的字节,也可以描述文件中的一段连续的数据字节
*设计初衷：用来分析一段原始数据(定长或不定长)的结构内容,主要提供通用的读写支持
数据可能来源于文件,网络套接字,其他程序提供,甚至某些不完整的数据(比如被截断的文件)
有些数据的结构是根据一些控制数据来动态变化的,我们可以在分析的时候用各种不同的数据块子类来指向不同的位置,这样主要用来避免多次大量复制数据块内容(有些未经分析的数据块可能都有好几M)

我们可以对一些特定的数据块进行读写,这样可以用于读取文件后进行动态变更特定参数后,再进行使用,也可以保存改写后的数据,或者局部的数据
(比如图片,我们可以动态分析出调色板的位置,然后改变调色板,然后再取像素,达到动态变色的目的.
早期很多游戏机的游戏就是通过调色版来进行调色,使得一个精灵可以显示成不同的颜色,以表达出敌我、自身状态等各种用意)

本类的复制构造函数不进行深拷贝(除非子类做了这个事情),也就是说此类可以当作普通变量来传递
本类支持通过文件访问,可以打开后慢慢读取,也可以一次性读取到内存中操作,也可以用映射的方式(某些编译环境可能不支持映射)打开文件访问
本类简单扩展了一些C语言下的stdlib.h和string.h中的内存操作函数,用于支持类似的操作,C++的new和delete也进行了封装,用于申请和释放内存
本类的大量函数的返回值类型为bool,如果true表示成功,因为我们有可能进行越界操作,或者故意进行一些不稳定的操作
*/
class DataBlock
{
public:
	//构造/析构函数
	DataBlock();
	DataBlock(const DataBlock &dataBlock);
	virtual ~DataBlock();

	//尺寸类型,这是用来衡量文件大小的数据类型,某些场合下可以用无符号类型来兼容内存操作函数,也可以用有符号的类型来兼容文件操作函数
	//请根据实际情况进行调整
	//typedef int SizeType;
	//typedef uint SizeType;
	//typedef long SizeType;
	//typedef ulong SizeType;
	typedef size_t SizeType;

	//打开/关闭文件
	bool openFile(const string &filename,const string &mode);//封装fopen(),打开成功的时候获取大小,返回是否成功
	bool openFileRead(const string &filename);//二进制只读方式调用openFile(),返回openFile()的返回值
	bool openFileReadUpdate(const string &filename);//二进制读写方式调用openFile(),返回openFile()的返回值
	bool openFileWrite(const string &filename,const string &mode)const;//二进制只写方式调用fopen(),mode决定写入方式,返回是否成功
	bool closeFile();//关闭文件,返回是否关闭成功
	//保存/加载/附加文件
	bool loadFile(const string &filename);//从文件中读取所有数据到内存中,返回是否成功(读大文件的时候很吃内存)
	bool saveFile(const string &filename)const;//从内存中写入所有数据到文件中,返回是否成功
	bool appendFile(const string &filename)const;//从内存中写入所有数据到文件中,从文件尾添加,返回是否成功
	//内存映射(某些编译环境不支持内存映射,会使以下两个函数返回false)
	bool memoryMap(const string &name);//以内存映射方式访问文件,返回是否成功
	bool memoryUnmap();//解除内存映射,返回是否成功

	//stdlib.h的部分封装
	bool memoryCAllocate(size_t n,size_t size);//calloc()
	bool memoryAllocate(size_t size);//malloc()
	bool memoryReallocate(size_t size);//realloc()
	void memoryFree();//free()
	//string.h的部分封装
	void* memchr(int chr,size_t num=0)const;
	int memcmp(const void *ptr,size_t num)const;
	int memcmp(const DataBlock &block)const;
	void* memcpyTo(void *dest,size_t num)const;//memcpy()
	void* memcpyFrom(const void *src,size_t num);//memcpy()
	void* memmoveTo(void *dest,size_t num)const;//memmove()
	void* memmoveFrom(void *src,size_t num);//memmove()
	void* memset(int val,size_t len=0);
	//string类相关
	string toString()const;
	//c++ new & delete
	bool newDataPointer(size_t size);//封装new关键字
	bool deleteDataPointer();//封装delete关键字
	//调试
	//int debug()const;//输出成员变量信息

	//数据分析
	/** @brief parseData 自行分析,子类可以重写此函数,主要目的是根据分析结果重新调整数据,比如调整能识别的数据长度,或调整子类的其他成员变量
		@return 默认为0,子类可以用于表示有多少数据是可识别的或者合法的,不同的子类可以定义不同的返回值*/
	virtual SizeType parseData(){return 0;}
	virtual void set_DataLength(SizeType length){dataLength=length;}//设定长度,支持子类的函数,比如BitBlock,会重新计算bitLength
	/** @brief reset 重置数据块,这会让数据块的成员变量变回初值
		如果调用了newDataPointer()等申请内存的函数,或者调用了openFile()等打开文件的函数时候,请不要调用此函数,请调用对应的清理函数
		否则可能造成内存泄漏,除非你知道你在干什么
		@return 默认返回0,可以在子类中对此值的意义进行重新定义*/
	virtual SizeType reset();

	//子数据块操作
	DataBlock subDataBlock(SizeType offset,SizeType length)const;//根据offset和length获取特定位置的数据块,返回子数据块
	/**
	 * @brief subDataBlock() 根据offset和length获取特定位置的数据块
	 * @param offset 数据在块中的偏移地址
	 * @param length 数据的长度(单位字节)
	 * @param subBlock 用于返回,获取成功的时候,新数据块会写到此对象中(该数据块需要事先申请好内存)
	 * @param copy 是否需要复制,如果true则subBlock会开辟一段内存,并从本对象中复制特定区块的数据
	 * @return 返回是否成功
	 */
	bool subDataBlock(SizeType offset,SizeType length,DataBlock &subBlock,bool copy=false)const;

	//以字节为单位反置任意类型的数据
	template<typename T>
	static void reverseByte(T &value)
	{
		int size=sizeof(T);
		char *data=(char*)&value;
		int front=0,back=size-1;
		char tmp;
		for(;front<back;++front,--back)
		{
			tmp=data[front];
			data[front]=data[back];
			data[back]=tmp;
		}
	}
	//以位为单位反置任意uintX型的数据
	static void reverseBit(uint8 &byte);
	static void reverseBit(uint16 &u16);
	static void reverseBit(uint32 &u32);
	static void reverseBit(uint64 &u64);
	//以位为单位反置从low位到high位的任意uintX型数据
	static void reverseBit(uint8 &byte,uint8 low,uint8 high);
	static void reverseBit(uint16 &u16,uint8 low,uint8 high);
	static void reverseBit(uint32 &u32,uint8 low,uint8 high);
	static void reverseBit(uint64 &u64,uint8 low,uint8 high);

	//get/set基础数据类型,返回是否读写成功
	DATABLOCK_TYPE(int8)
	DATABLOCK_TYPE(int16)
	DATABLOCK_TYPE(int32)
	DATABLOCK_TYPE(int64)

	DATABLOCK_TYPE(uint8)
	DATABLOCK_TYPE(uint16)
	DATABLOCK_TYPE(uint32)
	DATABLOCK_TYPE(uint64)
	
	DATABLOCK_TYPE(wchar_t)
	DATABLOCK_TYPE(char16_t)
	DATABLOCK_TYPE(char32_t)
	DATABLOCK_TYPE(float)
	DATABLOCK_TYPE(double)

	//bool操作
	static void normalizedOffset(SizeType &byte,SizeType &bit);//将byte和bit表示的位置进行规范化
	bool getBool(SizeType bitOffset,bool &value)const;//获取bitOffset位置的位(bool),返回是否读写成功
	bool getBool(SizeType byteOffset,SizeType bitOffset,bool &value)const;//获取byteOffset,bitOffset位置的位(bool),返回是否读写成功
	bool setBool(SizeType bitOffset,bool value);//设置bitOffset位置的位(bool),返回是否读写成功
	bool setBool(SizeType byteOffset,SizeType bitOffset,bool value);//设置byteOffset,bitOffset位置的位(bool),返回是否读写成功

	//字节操作(支持掩码操作)
	bool getByte(SizeType offset,uint8 &value,uint8 mask=0xFF)const;//获取offset位置的字节到value中,并用mask过滤,返回是否读写成功
	bool setByte(SizeType offset,const uint8 value,uint8 mask=0xFF);//写字节到offset位置,所写字节会先被mask过滤再写入,返回是否读写成功

	/** @brief dataPointer 指向内存数据的指针,指向数据块的首字节
	这个指针可以任意改变,只要你知道你在干什么.和操作普通指针一样,访问野指针可能会造成程序崩溃
	一般的用法是直接调用申请内存的函数,或者让dataPointer直接指向其它模块产生的数据块地址*/
	uchar* dataPointer;
	/** @brief dataLength 数据块的长度,可自行设定.打开文件或申请内存的时候,这个变量会自动设定*/
	SizeType dataLength;
	/** @brief dataFile 数据块所关联的源文件,如果数据块是内存数据块的时候,此值应该为空.打开文件成功的时候会设置此变量,不建议手动修改此变量
	此变量会影响各种get/set的操作,具体请看本文件开头定义的宏*/
	FILE *dataFile;
	/** @brief dataOffset 数据块在文件中的位置,和dataPointer的作用很相似,可以指定从特定位置开始作为首数据的地址
	此变量会影响各种get/set的操作,具体请看本文件开头定义的宏*/
	SizeType dataOffset;
};

#endif
