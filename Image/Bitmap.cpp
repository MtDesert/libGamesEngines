#include "Bitmap.h"
#include "Number.h"

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

#define BITMAP_GET_SET_COLOR(getORset) \
if(x>=width||y>=height)return false;\
/*定位*/\
uint pos=0;\
if(coordinateType==CoordinateType_Math){\
	pos=(y*width+x)*4;\
}else if(coordinateType==CoordinateType_Screen){\
	pos=((height-1-y)*width+x)*4;\
}else return false;\
/*读写颜色*/\
return getORset##_uint32(pos,color);

bool Bitmap_32bit::getColor(uint x,uint y,uint32 &color)const{BITMAP_GET_SET_COLOR(get)}
bool Bitmap_32bit::setColor(uint x,uint y,uint32 &color){BITMAP_GET_SET_COLOR(set)}

void Bitmap_32bit::getColorsList(List<uint32> &colorsList,uint maxAmount)const{
	colorsList.clear();
	uint32 color;
	for(SizeType pos=0;pos<dataLength;pos+=4){
		if(get_uint32(pos,color) && !colorsList.contain(color)){//找到新颜色,添加
			colorsList.push_back(color);
			if(maxAmount && colorsList.size()>=maxAmount){
				break;
			}
		}
	}
}

//扫描线
Bitmap32_Scanline::Bitmap32_Scanline():
	width(0),height(0),bitDepth(0),
	channelAmount(0),lineSize(0),
	colorsList(nullptr),
	leastUint(0),
	buffer8(nullptr),buffer16(nullptr),buffer32(nullptr),buffer64(nullptr){}

void Bitmap32_Scanline::createBuffer(){
	precision=pow(2,bitDepth)-1;
	leastUint=leastUintToStoreBit(bitDepth);
	deleteBuffer();
	newBuffer(leastUint*channelAmount*width);
}

uint8 Bitmap32_Scanline::value2color(decltype(precision) value)const{
	return Number::divideRound(value*0xFF,precision);
}
decltype(Bitmap32_Scanline::precision) Bitmap32_Scanline::color2value(uint8 color)const{
	return Number::divideRound(color*precision,0xFF);
}

void Bitmap32_Scanline::newBuffer(SizeType size){
	buffer8=new uint8[size];
	buffer16=reinterpret_cast<uint16*>(buffer8);
}
void Bitmap32_Scanline::deleteBuffer(){
	delete []buffer8;
	buffer8=nullptr;
}

#define BITMAP_BYTE_BIT(MACRO) \
	MACRO(1,8)\
	MACRO(2,16)\
	MACRO(4,32)\
	MACRO(8,64)

void Bitmap32_Scanline::encodeLineData(){
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:fromUint##bit##Array(buffer##bit,channelAmount*width,bitDepth,false);break;
		BITMAP_BYTE_BIT(CASE_ARRAY)
#undef CASE_ARRAY
	}
}
void Bitmap32_Scanline::decodeLineData()const{
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:toUint##bit##Array(bitDepth,channelAmount*width,false,buffer##bit);break;
		BITMAP_BYTE_BIT(CASE_ARRAY)
#undef CASE_ARRAY
	}
}

uint64 Bitmap32_Scanline::getBufferValue(uint x,uint8 channel)const{
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:return buffer##bit[x*channelAmount+channel];break;
		BITMAP_BYTE_BIT(CASE_ARRAY)
#undef CASE_ARRAY
	}
	return 0;
}

void Bitmap32_Scanline::setBufferValue(uint x,uint8 channel,uint64 value){
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:buffer##bit[x*channelAmount+channel]=value;break;
		BITMAP_BYTE_BIT(CASE_ARRAY)
#undef CASE_ARRAY
	}
}