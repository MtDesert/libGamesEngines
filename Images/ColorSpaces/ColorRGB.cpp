#include"ColorRGB.h"
#include"Number.h"

#define COLOR_MAX 0xFF

//constructors
ColorRGB::ColorRGB():ColorRGB(0,0,0){}
ColorRGB::ColorRGB(uint8 r,uint8 g,uint8 b):red(r),green(g),blue(b){}
ColorRGB::ColorRGB(unsigned value){fromRGB(value);}

ColorRGBA::ColorRGBA():ColorRGBA(0,0,0,0){}
ColorRGBA::ColorRGBA(uint8 r, uint8 g, uint8 b, uint8 a):ColorRGB(r,g,b),alpha(a){}
ColorRGBA::ColorRGBA(unsigned value){fromRGBA(value);}

uint8 ColorRGB::grayInt()const{return round(gray());}
COLOR_DECIMAL ColorRGB::gray()const{
	//CIE 1931,HDTV,ATSC
	//return red*0.2126 + green*0.7152 + blue*0.0722;

	//return (red*76+green*151+blue*28)>>8;//bit shift
	//return (red+green+blue)/3;//average
	//return green;//only green
	return red*grayR + green*grayG + blue*grayB;//SDTV with BT.601,PAL & NTSC
}
void ColorRGB::toGray(){red=green=blue=grayInt();}
const COLOR_DECIMAL ColorRGB::grayR(0.299),ColorRGB::grayG(0.587),ColorRGB::grayB(0.114);

#define COLOR_DARK(name) ret.name = Number::divideRound((int)name*color.name,COLOR_MAX);
ColorRGB ColorRGB::darkColor(const ColorRGB &color)const{
	ColorRGB ret;
	COLOR_DARK(red)
	COLOR_DARK(green)
	COLOR_DARK(blue)
	return ret;
}
#define COLOR_LIGHT(name) ret.name = name + Number::divideRound((COLOR_MAX-name)*color.name,COLOR_MAX);
ColorRGB ColorRGB::lightColor(const ColorRGB &color)const{
	ColorRGB ret;
	COLOR_LIGHT(red)
	COLOR_LIGHT(green)
	COLOR_LIGHT(blue)
	return ret;
}

#define COLOR_ALPHA_BLEND(name) ret.name = Number::divideRound((int)name*(COLOR_MAX-alpha) + (int)rgb.name*alpha,COLOR_MAX);
ColorRGB ColorRGB::alphaBlend(const ColorRGB &rgb,uint8 alpha)const{
	ColorRGB ret;
	COLOR_ALPHA_BLEND(red)
	COLOR_ALPHA_BLEND(green)
	COLOR_ALPHA_BLEND(blue)
	return ret;
}
#undef COLOR_ALPHA_BLEND

#define COLOR_DELTA(name) name>rgb.name ? name-rgb.name : rgb.name-name;
int ColorRGB::deltaSum(const ColorRGB &rgb)const{
	auto ret=COLOR_DELTA(red);
	ret+=COLOR_DELTA(green);
	ret+=COLOR_DELTA(blue);
	return ret;
}
#define COLOR_POW2(name) pow((int)name-(int)rgb.name,2)
double ColorRGB::distance(const ColorRGB &rgb)const{
	return sqrt(COLOR_POW2(red)+COLOR_POW2(green)+COLOR_POW2(blue));
}

ColorRGBA ColorRGBA::darkColor(const ColorRGBA &color)const{
	ColorRGBA ret;
	COLOR_DARK(red)
	COLOR_DARK(green)
	COLOR_DARK(blue)
	COLOR_DARK(alpha)
	return ret;
}
ColorRGBA ColorRGBA::lightColor(const ColorRGBA &color)const{
	ColorRGBA ret;
	COLOR_LIGHT(red)
	COLOR_LIGHT(green)
	COLOR_LIGHT(blue)
	COLOR_LIGHT(alpha)
	return ret;
}

#define COLOR_ALPHA_BLEND(name) ret.name = Number::divideRound((int)name*alpha + (int)rgba.name*rgba.alpha,COLOR_MAX);
ColorRGBA ColorRGBA::alphaBlend(const ColorRGBA &rgba)const{
	ColorRGBA ret;
	COLOR_ALPHA_BLEND(red)
	COLOR_ALPHA_BLEND(green)
	COLOR_ALPHA_BLEND(blue)
	return ret;
}

//operator
bool ColorRGB::operator==(const ColorRGB &color)const{
	return red==color.red
		&& green==color.green
		&& blue==color.blue;
}
bool ColorRGBA::operator==(const ColorRGBA &color)const{
	return ColorRGB::operator==(color)
		&& alpha==color.alpha;
}
ColorRGBA& ColorRGBA::operator=(const ColorRGB &color){
	red=color.red;
	green=color.green;
	blue=color.blue;
	return *this;
}

#define COLOR_CPP_FROMTO24(Name,Color0,Color1,Color2) \
unsigned ColorRGB::to##Name()const{return Color0|(Color1<<8)|(Color2<<16);} \
void ColorRGB::from##Name(unsigned val){ \
	Color0=val;\
	Color1=val>>8;\
	Color2=val>>16;\
}

COLOR_CPP_FROMTO24(RGB,red,green,blue)
COLOR_CPP_FROMTO24(RBG,red,blue,green)
COLOR_CPP_FROMTO24(GRB,green,red,blue)
COLOR_CPP_FROMTO24(GBR,green,blue,red)
COLOR_CPP_FROMTO24(BRG,blue,red,green)
COLOR_CPP_FROMTO24(BGR,blue,green,red)

#define COLOR_CPP_FROMTO32(Name,Color0,Color1,Color2,Color3) \
uint32 ColorRGBA::to##Name()const{return Color0|(Color1<<8)|(Color2<<16)|(Color3<<24);} \
void ColorRGBA::from##Name(unsigned val){ \
	Color0=val;\
	Color1=val>>8;\
	Color2=val>>16;\
	Color3=val>>24;\
}

COLOR_CPP_FROMTO32(RGBA,red,green,blue,alpha)
COLOR_CPP_FROMTO32(RBGA,red,blue,green,alpha)
COLOR_CPP_FROMTO32(GRBA,green,red,blue,alpha)
COLOR_CPP_FROMTO32(GBRA,green,blue,red,alpha)
COLOR_CPP_FROMTO32(BRGA,blue,red,green,alpha)
COLOR_CPP_FROMTO32(BGRA,blue,green,red,alpha)

COLOR_CPP_FROMTO32(RGAB,red,green,alpha,blue)
COLOR_CPP_FROMTO32(RBAG,red,blue,alpha,green)
COLOR_CPP_FROMTO32(GRAB,green,red,alpha,blue)
COLOR_CPP_FROMTO32(GBAR,green,blue,alpha,red)
COLOR_CPP_FROMTO32(BRAG,blue,red,alpha,green)
COLOR_CPP_FROMTO32(BGAR,blue,green,alpha,red)

COLOR_CPP_FROMTO32(RAGB,red,alpha,green,blue)
COLOR_CPP_FROMTO32(RABG,red,alpha,blue,green)
COLOR_CPP_FROMTO32(GARB,green,alpha,red,blue)
COLOR_CPP_FROMTO32(GABR,green,alpha,blue,red)
COLOR_CPP_FROMTO32(BARG,blue,alpha,red,green)
COLOR_CPP_FROMTO32(BAGR,blue,alpha,green,red)

COLOR_CPP_FROMTO32(ARGB,alpha,red,green,blue)
COLOR_CPP_FROMTO32(ARBG,alpha,red,blue,green)
COLOR_CPP_FROMTO32(AGRB,alpha,green,red,blue)
COLOR_CPP_FROMTO32(AGBR,alpha,green,blue,red)
COLOR_CPP_FROMTO32(ABRG,alpha,blue,red,green)
COLOR_CPP_FROMTO32(ABGR,alpha,blue,green,red)

const ColorRGBA
ColorRGBA::Transparent(0),
ColorRGBA::Black(0xFF000000),
ColorRGBA::Red(0xFF0000FF),
ColorRGBA::Green(0xFF00FF00),
ColorRGBA::Blue(0xFFFF0000),
ColorRGBA::Gray(0xFF808080),
ColorRGBA::Yellow(0xFF00FFFF),
ColorRGBA::Purple(0xFFFF00FF),
ColorRGBA::Cyan(0xFFFFFF00),
ColorRGBA::White(0xFFFFFFFF);