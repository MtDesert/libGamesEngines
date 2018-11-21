#include"ColorYUV.h"

const uint16 ColorYUV::weightR=299;
const uint16 ColorYUV::weightG=587;
const uint16 ColorYUV::weightB=114;
const uint16 ColorYUV::maxU=436;
const uint16 ColorYUV::maxV=615;
uint16 ColorYUV::weightRGB(){return weightR+weightG+weightB;}

//YUV
ColorYUV::ColorYUV(uint8 y,uint8 u,uint8 v):y(y),u(u),v(v){}
void ColorYUV::fromRGB(const ColorRGB &color){
	const uint8 &r(color.red),&g(color.green),&b(color.blue);
	uint16 rgb=weightRGB();
	y=(r*weightR+g*weightG+b*weightB)/rgb;//SDTV with BT.601
	//y=(r*2126+g*7152+b*722)/10000;//HDTV with BT.709
	u=maxU*(b-y)/(rgb-weightB)+128;
	v=maxV*(r-y)/(rgb-weightR)+128;
}
void ColorYUV::toRGB(ColorRGB &color)const{
	uint8 &r(color.red),&g(color.green),&b(color.blue);
	uint16 rgb=weightRGB();
	b=(u-128)*(rgb-weightB)/maxU+y;
	r=(v-128)*(rgb-weightR)/maxV+y;
	g=(y*rgb-r*weightR-b*weightB)/weightG;
}

ColorYCbCr::ColorYCbCr(uint8 y, uint8 cb, uint8 cr):y(y),cb(cb),cr(cr){}
void ColorYCbCr::fromRGB(const ColorRGB &color){
	const uint8 &r(color.red),&g(color.green),&b(color.blue);
	y=(r*299+g*587+b*114)/1000;
	cr=(-r*1687-g*3313+b*5000)/10000+128;
	cb=(r*5000-g*4187-b*813)/10000+128;
}
void ColorYCbCr::toRGB(ColorRGB &color)const{
	uint8 &r(color.red),&g(color.green),&b(color.blue);
	r=y+(cb-128)*1402/1000;
	g=y-((cb-128)*34414+(cr-128)*71414)/100000;
	b=y+(cb-128)*1772/1000;
}

ColorYIQ::ColorYIQ(uint8 y, uint8 i, uint8 q):y(y),i(i),q(q){}
void ColorYIQ::fromRGB(const ColorRGB &color){
	const uint8 &r(color.red),&g(color.green),&b(color.blue);
	y=(r*299+g*587+b*114)/1000;
	i=(r*596-g*274-b*322)/1000;
	q=(r*211-g*523+b*312)/1000;
}
void ColorYIQ::toRGB(ColorRGB &color)const{
	uint8 &r(color.red),&g(color.green),&b(color.blue);
	r=y+(i*956+q*621)/1000;
	g=y+(-i*272-q*647)/1000;
	b=y+(-i*1106+q*1703)/1000;
}