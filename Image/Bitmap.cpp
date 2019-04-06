#include "Bitmap.h"

Bitmap_32bit::Bitmap_32bit():coordinateType(CoordinateType_Math),width(0),height(0){}
Bitmap_32bit::~Bitmap_32bit(){deleteBitmap();}

bool Bitmap_32bit::newBitmap(uint w,uint h){
	if(w==0||h==0)return false;
	width=w;
	height=h;
	return newDataPointer(w*h*4);
}
bool Bitmap_32bit::deleteBitmap(){
	width=0;height=0;
	return deleteDataPointer();
}
uint Bitmap_32bit::getWidth()const{return width;}
uint Bitmap_32bit::getHeight()const{return height;}

bool Bitmap_32bit::getColor(uint x,uint y,uint32 &color)const{
	if(x>=width||y>=height)return false;
	//get position
	uint pos=0;
	if(coordinateType==CoordinateType_Math){
		pos=(y*width+x)*4;
	}else if(coordinateType==CoordinateType_Screen){
		pos=((height-1-y)*width+x)*4;
	}else return false;
	//get color
	return get_uint32(pos,color);
}
bool Bitmap_32bit::setColor(uint x,uint y,uint32 &color){
	if(x>=width||y>=height)return false;
	//get position
	uint pos=0;
	if(coordinateType==CoordinateType_Math){
		pos=(y*width+x)*4;
	}else if(coordinateType==CoordinateType_Screen){
		pos=((height-1-y)*width+x)*4;
	}else return false;
	//set color
	return set_uint32(pos,color);
}
void Bitmap_32bit::getColorsList(List<uint32> &colorsList)const{
	colorsList.clear();
	uint32 color;
	for(SizeType pos=0;pos<dataLength;pos+=4){
		if(get_uint32(pos,color) && !colorsList.contain(color)){//找到新颜色,添加
			colorsList.push_back(color);
		}
	}
}