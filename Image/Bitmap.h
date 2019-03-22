#ifndef BITMAP_H
#define BITMAP_H

#include"DataBlock.h"
#include"List.h"

/**
32位图,可以是RGBA,BGRA,ARGB等格式,但本类不存储具体是什么格式,使用者根据需求自定义需要的格式即可
 */
class Bitmap_32bit:public DataBlock{
public:
	Bitmap_32bit();
	~Bitmap_32bit();
	//new & delete
	bool newBitmap(uint width,uint height);
	bool deleteBitmap();
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
	bool setColor(uint x,uint y,uint32 &color);
	//色表,获取所有用到的颜色
	void getColorsList(List<uint32> &colorsList)const;
private:
	uint width,height;
};
#endif