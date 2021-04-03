#ifndef BITMAP_H
#define BITMAP_H

#include"DataBlock.h"
#include"List.h"

/**
32位图,可以是RGBA,BGRA,ARGB等格式,但本类不存储具体是什么格式,使用者根据需求自定义需要的格式即可
 */
class Bitmap_32bit:public DataBlock{
public:
	//构造/析构函数
	Bitmap_32bit();
	~Bitmap_32bit();

	//申请/释放空间
	bool newBitmap(uint width,uint height);
	void deleteBitmap();
	//尺寸
	uint getWidth()const;
	uint getHeight()const;

	//坐标类型
	enum CoordinateType{
		CoordinateType_Math,
		CoordinateType_Screen
	};
	CoordinateType coordinateType;

	//颜色读写,返回读写是否成功
	bool getColor(uint x,uint y,uint32 &color)const;
	bool setColor(uint x,uint y,const uint32 &color);
	void fillColor(const uint32 &color);
	//色表,获取所有用到的颜色保存到colorsList,maxAmount为colorsList的上线,0为无限制
	void getColorsList(List<uint32> &colorsList,uint maxAmount=0)const;

	//局部图像
	Bitmap_32bit subBitmap(uint x,uint y,uint width,uint height)const;
private:
	uint width,height;
};

//Bitmap32扫描线,用于编码解码数据
struct Bitmap32_Scanline:public DataBlock{
	Bitmap32_Scanline();
	~Bitmap32_Scanline();

	uint32 width,height;//图像宽度
	uint8 bitDepth;//位深
	uint8 channelAmount;//通道数
	uint32 lineSize;

	List<uint32> *colorsList;//颜色表,索引图需要
	void createBuffer();
protected:
	//缓冲变量
	uint64 precision;//精度,用于四舍五入
	uint8 value2color(decltype(precision) value)const;
	decltype(precision) color2value(uint8 color)const;

	uint8 leastUint;//缓冲区的字节数
	//编码解码
	void encodeLineData();
	void decodeLineData()const;
	//读写
	uint64 getBufferValue(uint x,uint8 channel)const;
	void setBufferValue(uint x,uint8 channel,uint64 value);
	//uint指针
	DataBlock buffer;
	uint8 *buffer8;
	uint16 *buffer16;
};
#endif