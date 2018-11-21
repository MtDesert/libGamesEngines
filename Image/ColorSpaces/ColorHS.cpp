#include"ColorHS.h"
#include"MinMax.h"

const uint16 ColorHS::maxHue=360;//degree
const uint8 ColorHS::maxSaturation=255;
const uint8 ColorHSL::maxLightness=255;
const uint8 ColorHSV::maxValue=255;

#define COLOR_CACULATE_HUE \
const uint8 &r(color.red),&g(color.green),&b(color.blue);\
uint8 max=maxUint8(3,r,g,b);\
uint8 min=minUint8(3,r,g,b);\
if(max==min){\
	hue=0;\
}else if(max==r){\
	if(g>=b){\
		hue=maxHue*(g-b)/(max-min)/6;\
	}else{\
		hue=maxHue*(g-b)/(max-min)/6+maxHue;\
	}\
}else if(max==g){\
	hue=maxHue*(b-r)/(max-min)/6+maxHue/3;\
}else if(max==b){\
	hue=maxHue*(r-g)/(max-min)/6+maxHue*2/3;\
}

#define COLOR_CACULATE_RGB \
uint8 &r(color.red),&g(color.green),&b(color.blue);\
if(min==max){\
	r=g=b=min;\
}else{\
	uint16 low=0,high=0;\
	uint8 *maxRGB[6]={&r,&g,&g,&b,&b,&r};\
	uint8 *minRGB[6]={&b,&b,&r,&r,&g,&g};\
	uint8 *mmmRGB[6]={&g,&r,&b,&g,&r,&b};\
	for(int i=0;i<6;++i){\
		low=high,high=maxHue*(i+1)/6;\
		if(low<=hue && hue<high){\
			*maxRGB[i]=max;\
			*minRGB[i]=min;\
			*mmmRGB[i]=hue*(max-min)*6/maxHue + min - high;\
			break;\
		}\
	}\
}

ColorHS::ColorHS(uint16 h, uint8 s):hue(h),saturation(s){}
ColorHSL::ColorHSL(uint16 h, uint8 s, uint8 l):ColorHS(h,s),lightness(l){}
ColorHSV::ColorHSV(uint16 h, uint8 s, uint8 v):ColorHS(h,s),value(v){}

void ColorHSL::fromRGB(const ColorRGB &color){
	COLOR_CACULATE_HUE
	//lightness
	lightness=(max+min)/2;
	//saturation
	if(lightness==0||min==max){
		saturation=0;
	}else if(lightness>0 && lightness<=maxLightness/2){
		saturation=(max-min)*maxSaturation/(max+min);
	}else{
		saturation=(max-min)*maxSaturation/(maxLightness*2-(max+min));
	}
}
void ColorHSL::toRGB(ColorRGB &color)const{
	//caculate min,max
	uint8 min,max;
	if(lightness<=255/2){
		auto delta=lightness*saturation/255;
		min=lightness-delta;
		max=lightness+delta;
	}else{
		auto delta=(255-lightness)*saturation/255;
		min=lightness-delta;
		max=lightness+delta;
	}
	//caculate r,g,b
	COLOR_CACULATE_RGB
}
void ColorHSV::fromRGB(const ColorRGB &color){
	COLOR_CACULATE_HUE
	//saturation
	saturation=max?(max-min)*maxSaturation/max:0;
	//value
	value=max*maxValue/255;
}
void ColorHSV::toRGB(ColorRGB &color)const{
	auto max=value*255/maxValue;
	auto min=max?max-max*saturation/maxSaturation:0;
	COLOR_CACULATE_RGB
}