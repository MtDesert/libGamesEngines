#include"ColorCMYK.h"
#include<math.h>
//CMY
ColorCMY::ColorCMY(uint8 c,uint8 m,uint8 y):cyan(c),magenta(m),yellow(y){}
ColorCMY::ColorCMY(const ColorRGB &color){fromRGB(color);}

void ColorCMY::fromRGB(const ColorRGB &color){
	cyan=RGB_MAX-color.red;
	magenta=RGB_MAX-color.green;
	yellow=RGB_MAX-color.blue;
}
void ColorCMY::toRGB(ColorRGB &color) const{
	color.red=RGB_MAX-cyan;
	color.green=RGB_MAX-magenta;
	color.blue=RGB_MAX-yellow;
}
//CMYK
ColorCMYK::ColorCMYK(COLOR_DECIMAL c,COLOR_DECIMAL m,COLOR_DECIMAL y,COLOR_DECIMAL k):cyan(c),magenta(m),yellow(y),black(k){}
ColorCMYK::ColorCMYK(const ColorRGB &rgb){fromRGB(rgb);}

void ColorCMYK::fromRGB(const ColorRGB &rgb){
#define DECL(var,name) COLOR_DECIMAL var=(COLOR_DECIMAL)rgb.name/RGB_MAX;
	DECL(r,red)DECL(g,green)DECL(b,blue)//转换成比例形式
#undef DECL
	black=1-max(r,max(g,b));//黑色比例
	auto white=1-black;
	if(white==0)cyan=magenta=yellow=0;
	else{//CMY比例
		cyan=(white-r)/white;
		magenta=(white-g)/white;
		yellow=(white-b)/white;
	}
}
void ColorCMYK::toRGB(ColorRGB &rgb)const{
	auto white=1-black;
#define DECL(var,name) rgb.var=round((white-white*name)*RGB_MAX);
	DECL(red,cyan)DECL(green,magenta)DECL(blue,yellow)
#undef DECL
}