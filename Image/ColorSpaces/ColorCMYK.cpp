#include"ColorCMYK.h"
#include"MinMax.h"
//CMY
ColorCMY::ColorCMY(uint8 c,uint8 m,uint8 y):cyan(c),magenta(m),yellow(y){}
ColorCMY::ColorCMY(const ColorRGB &color){fromRGB(color);}

void ColorCMY::fromRGB(const ColorRGB &color){
	cyan=255-color.red;
	magenta=255-color.green;
	yellow=255-color.blue;
}
void ColorCMY::toRGB(ColorRGB &color) const{
	color.red=255-cyan;
	color.green=255-magenta;
	color.blue=255-yellow;
}
//CMYK
ColorCMYK::ColorCMYK(uint8 c,uint8 m,uint8 y,uint8 k):ColorCMY(c,m,y),keyBlack(k){}
ColorCMYK::ColorCMYK(const ColorCMY &color){fromCMY(color);}
ColorCMYK::ColorCMYK(const ColorRGB &color){fromRGB(color);}

void ColorCMYK::fromRGB(const ColorRGB &color){
	fromCMY(ColorCMY(color));
}
void ColorCMYK::toRGB(ColorRGB &color)const{
	ColorCMY cmy;
	toCMY(cmy);
	cmy.toRGB(color);
}
void ColorCMYK::fromCMY(const ColorCMY &color){
	keyBlack=minUint8(3,color.cyan,color.magenta,color.yellow);
	//new c,y,m
	if(keyBlack==255){
		cyan=magenta=yellow=0;
	}else{
		auto white=255-keyBlack;
		cyan=(color.cyan-keyBlack)*255/white;
		magenta=(color.magenta-keyBlack)*255/white;
		yellow=(color.yellow-keyBlack)*255/white;
	}
}
void ColorCMYK::toCMY(ColorCMY &color)const{
	if(keyBlack==255){
		color.cyan=color.magenta=color.yellow=255;
	}else{
		auto white=255-keyBlack;
		color.cyan=cyan*white/255+keyBlack;
		color.magenta=magenta*white/255+keyBlack;
		color.yellow=yellow*white/255+keyBlack;
	}
}