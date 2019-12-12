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
	value=*((Type*)(&dataPointer[offset]));\
	return true;\
}\
bool DataBlock::set_##Type(SizeType offset,const Type &value){\
	if(offset+sizeof(Type)>dataLength)return false;\
	*((Type*)(&dataPointer[offset]))=value;\
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
*此类名为"数据块",用来描述一段内存中连续的字节
*设计初衷：用来分析一段原始数据(定长或不定长)的结构内容,主要提供通用的读写支持
数据可能来源于文件,网络套接字,其他程序提供,甚至某些不完整的数据(比如被截断的文件)
有些数据的结构是根据一些控制数据来动态变化的,我们可以在分析的时候用各种不同的数据块子类来指向不同的位置,这样主要用来避免多次大量复制数据块内容(有些未经分析的数据块可能都有好几M)

我们可以对一些特定的数据块进行读写,这样可以用于读取文件后进行动态变更特定参数后,再进行使用,也可以保存改写后的数据,或者局部的数据
(比如图片,我们可以动态分析出调色板的位置,然后改变调色板,然后再取像素,达到动态变色的目的.
早期很多游戏机的游戏就是通过调色版来进行调色,使得一个精灵可以显示成不同的颜色,以表达出敌我、自身状态等各种用意)

本类的复制构造函数不进行深拷贝(除非子类做了这个事情),也就是说此类可以当作普通变量来传递
本类简单扩展了一些C语言下的stdlib.h和string.h中的内存操作函数,用于支持类似的操作,C++的new和delete也进行了封装,用于申请和释放内存
本类的大量函数的返回值类型为bool,如果true表示成功,因为我们有可能进行越界操作,或者故意进行一些不稳定的操作
*/
class DataBlock{
public:
	//构造/析构函数
	DataBlock(const void *ptr=nullptr,SizeType length=0);
	DataBlock(const DataBlock &dataBlock);
	virtual ~DataBlock();

	//打开/关闭文件
	bool openFileWrite(const string &filename,const string &mode)const;//二进制只写方式调用fopen(),mode决定写入方式,返回是否成功
	//保存/加载/附加文件
	DataBlock loadFile(const string &filename);//从文件filename中读取所有数据到buffer中,返回是否成功(读大文件的时候很吃内存)
	bool saveFile(const string &filename)const;//从内存中写入所有数据到文件中,返回是否成功
	bool appendFile(const string &filename)const;//从内存中写入所有数据到文件中,从文件尾添加,返回是否成功
	bool fileWrite(FILE *file)const;

	//stdlib.h的部分封装
	bool memoryAllocate(SizeType size,bool enlargeOnly=false);//malloc(),realloc(),enlargeOnly表示只扩充不减少
	void memoryFree();//free()
	//string.h的部分封装
	void* memchr(int chr,SizeType num=0)const;
	int memcmp(const void *ptr,SizeType num)const;
	int memcmp(const DataBlock &block)const;
	void* memcpyTo(void *dest,SizeType num)const;//memcpy()
	void* memcpyFrom(const void *src,SizeType num);//memcpy()
	void* memmoveTo(void *dest,SizeType num)const;//memmove()
	void* memmoveFrom(void *src,SizeType num);//memmove()
	void* memset(int val,SizeType len=0);
	//string类相关
	string toString()const;

	//数据分析
	/** @brief parseData 自行分析,子类可以重写此函数,主要目的是根据分析结果重新调整数据,比如调整能识别的数据长度,或调整子类的其他成员变量
		@return 默认为0,子类可以用于表示有多少数据是可识别的或者合法的,不同的子类可以定义不同的返回值*/
	virtual SizeType parseData(){return 0;}
	virtual void set_DataLength(SizeType length){dataLength=length;}//设定长度,支持子类的函数,比如BitBlock,会重新计算bitLength
	//设置数据块内容(请留意原来的指针是否用于申请内存)
	void set(const void *ptr=nullptr,SizeType length=0);
	void set(const DataBlock &block);

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

	DATABLOCK_TYPE(string)
	//bool操作
	static void normalizedOffset(SizeType &byte,SizeType &bit);//将byte和bit表示的位置进行规范化
	bool getBool(SizeType bitOffset,bool &value)const;//获取bitOffset位置的位(bool),返回是否读写成功
	bool getBool(SizeType byteOffset,SizeType bitOffset,bool &value)const;//获取byteOffset,bitOffset位置的位(bool),返回是否读写成功
	bool setBool(SizeType bitOffset,bool value);//设置bitOffset位置的位(bool),返回是否读写成功
	bool setBool(SizeType byteOffset,SizeType bitOffset,bool value);//设置byteOffset,bitOffset位置的位(bool),返回是否读写成功

	//字节操作(支持掩码操作)
	bool getByte(SizeType offset,uint8 &value,uint8 mask=0xFF)const;//获取offset位置的字节到value中,并用mask过滤,返回是否读写成功
	bool setByte(SizeType offset,const uint8 value,uint8 mask=0xFF);//写字节到offset位置,所写字节会先被mask过滤再写入,返回是否读写成功

	//转换
	//使用uint存储bit个位需要多少个字节(根据系统情况会返回1,2,4,8,返回0表示bit超出范围)
	static uint leastUintToStoreBit(uint8 bitLen);
	/** @brief 将原始数据转换成整数数组(DataBlock保存),在地址上是连续的,以便直接访问
	 * bitLen 原始数据中每个数据的长度(bit)
	 * amount 原始数据数量
	 * littleEndian 原始数据的排列规律是否为小尾序列
	 * uintArray 输出的数组
	*/
#define DATABLOCK_TO_ARRAY(bit)\
	bool toUint##bit##Array(uint8 bitLen,SizeType amount,bool littleEndian,uint##bit uintArray[])const;\
	bool fromUint##bit##Array(uint##bit uintArray[],SizeType amount,uint8 bitLen,bool littleEndian);

	DATABLOCK_TO_ARRAY(8)
	DATABLOCK_TO_ARRAY(16)
	DATABLOCK_TO_ARRAY(32)
	DATABLOCK_TO_ARRAY(64)
#undef DATABLOCK_TO_ARRAY
	void debug()const;

	/** @brief dataPointer 指向内存数据的指针,指向数据块的首字节
	这个指针可以任意改变,只要你知道你在干什么.和操作普通指针一样,访问野指针可能会造成程序崩溃
	一般的用法是直接调用申请内存的函数,或者让dataPointer直接指向其它模块产生的数据块地址*/
	uchar* dataPointer;
	/** @brief dataLength 数据块的长度,可自行设定.打开文件或申请内存的时候,这个变量会自动设定*/
	SizeType dataLength;
};
#endif