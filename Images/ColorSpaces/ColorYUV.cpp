#include"ColorYUV.h"
#include<math.h>

static const int16 minU=-RGB_MAX*(ColorRGB::grayR+ColorRGB::grayG);
static const int16 minV=-RGB_MAX*(ColorRGB::grayG+ColorRGB::grayB);
//YUV
ColorYUV::ColorYUV(uint8 y,uint8 u,uint8 v):y(y),u(u),v(v){}
void ColorYUV::fromRGB(const ColorRGB &rgb){
	auto &r(rgb.red),&b(rgb.blue);
	y=rgb.grayInt();//计算亮度
	u=(b-y)-minU;
	v=(r-y)-minV;
}
void ColorYUV::toRGB(ColorRGB &rgb)const{
	auto &r(rgb.red),&g(rgb.green),&b(rgb.blue);
	r = minV+v+y;
	b = minU+u+y;
	auto gg = round((y-r*ColorRGB::grayR-b*ColorRGB::grayB)/ColorRGB::grayG);
	if(gg>RGB_MAX){g=RGB_MAX;}
	else if(gg<0){g=0;}
	else g=gg;
}

ColorYCbCr::ColorYCbCr(COLOR_DECIMAL y, COLOR_DECIMAL cb, COLOR_DECIMAL cr):y(y),cb(cb),cr(cr){}
void ColorYCbCr::fromRGB(const ColorRGB &rgb){
	auto &r(rgb.red),&b(rgb.blue);
	y=rgb.gray();//计算亮度
	cb=b-y;
	cr=r-y;
}
void ColorYCbCr::toRGB(ColorRGB &rgb)const{
	auto &r(rgb.red),&g(rgb.green),&b(rgb.blue);
	b=round(y+cb);
	r=round(y+cr);
	g=round((y-r*ColorRGB::grayR-b*ColorRGB::grayB)/ColorRGB::grayG);
}