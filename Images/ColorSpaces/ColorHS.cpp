#include"ColorHS.h"
#include<math.h>

const COLOR_DECIMAL ColorHS::maxHue=360;//角度
const COLOR_DECIMAL ColorHS::maxSaturation=255;
const COLOR_DECIMAL ColorHSL::maxLightness=255;

ColorHS::ColorHS(COLOR_DECIMAL h, COLOR_DECIMAL s):hue(h),saturation(s){}
ColorHSL::ColorHSL(COLOR_DECIMAL h, COLOR_DECIMAL s, COLOR_DECIMAL l):ColorHS(h,s),lightness(l){}
ColorHSV::ColorHSV(uint16 h, uint8 s, uint8 v):ColorHS(h,s),value(v){}

uint8 ColorHS::minRGB(const ColorRGB &rgb){return min(rgb.red,min(rgb.green,rgb.blue));}
uint8 ColorHS::maxRGB(const ColorRGB &rgb){return max(rgb.red,max(rgb.green,rgb.blue));}
void ColorHS::caculateHue(const ColorRGB &rgb,uint8 minRGB,uint8 maxRGB){
	if(minRGB==maxRGB){
		hue=0;
	}else{//判断色相区间
		auto &r(rgb.red),&g(rgb.green),&b(rgb.blue);
		int delta=maxRGB-minRGB;
		if(maxRGB==r){
			hue=60*((COLOR_DECIMAL)g-b)/delta;
		}else if(maxRGB==g){
			hue=60*((COLOR_DECIMAL)b-r)/delta+120;
		}else if(maxRGB==b){
			hue=60*((COLOR_DECIMAL)r-g)/delta+240;
		}
		if(hue<0)hue+=maxHue;
		if(hue>=maxHue)hue-=maxHue;
	}
}
void ColorHS::toRGB(ColorRGB &rgb,uint8 minRGB,uint8 maxRGB)const{
	uint8 &r(rgb.red),&g(rgb.green),&b(rgb.blue);
	if(minRGB==maxRGB){
		r=g=b=minRGB;
	}else{
		uint16 low=0,high=0;
		uint8 *maxGrp[6]={&r,&g,&g,&b,&b,&r};
		uint8 *minGrp[6]={&b,&b,&r,&r,&g,&g};
		uint8 *midGrp[6]={&g,&r,&b,&g,&r,&b};
		COLOR_DECIMAL offset[6]={0,120,120,240,240,360};
		for(int i=0;i<6;++i){
			low=high,high=maxHue*(i+1)/6;
			if(low<=hue && hue<high){
				*maxGrp[i]=maxRGB;
				*minGrp[i]=minRGB;
				auto negative=(i%2==0)?1:-1;
				*midGrp[i]=round((hue-offset[i])*(maxRGB-minRGB)*6/maxHue*negative + minRGB);
				break;
			}
		}
	}
}
void ColorHSL::fromRGB(const ColorRGB &rgb){
	int min=minRGB(rgb),max=maxRGB(rgb);
	caculateHue(rgb,min,max);//计算色相
	lightness=(max+min)/2;//计算亮度
	//计算饱和度
	auto delta=max-min;
	if(delta==0){
		saturation=0;
	}else if(lightness<=maxLightness/2){
		saturation=delta*maxSaturation/(max+min);
	}else{
		saturation=delta*maxSaturation/(maxLightness*2-(max+min));
	}
}
void ColorHSL::toRGB(ColorRGB &rgb)const{
	auto delta=lightness<=maxLightness/2?
		lightness*2*saturation/maxSaturation:
		(maxLightness-lightness)*2*saturation/maxSaturation;
	uint8 min=round(lightness-delta/2),max=round(lightness+delta/2);
	ColorHS::toRGB(rgb,min,max);
}
void ColorHSV::fromRGB(const ColorRGB &rgb){
	auto min=minRGB(rgb),max=maxRGB(rgb);
	caculateHue(rgb,min,max);//计算色相
	saturation=max?(max-min)*maxSaturation/max:0;//计算饱和度
	value=max;//计算明度
}
void ColorHSV::toRGB(ColorRGB &rgb)const{
	auto max=value;
	auto min=max?max-round(max*saturation/maxSaturation):0;
	ColorHS::toRGB(rgb,min,max);
}