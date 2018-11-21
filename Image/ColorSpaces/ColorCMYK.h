#ifndef COLORCMYK_H
#define COLORCMYK_H

#include"ColorRGB.h"
//CMY Serial, use for print

struct ColorCMY{//Cyan-Magenta-Yellow
	ColorCMY(uint8 c=0,uint8 m=0,uint8 y=0);
	ColorCMY(const ColorRGB &color);

	virtual void fromRGB(const ColorRGB &color);
	virtual void toRGB(ColorRGB &color)const;
	uint8 cyan,magenta,yellow;
};
struct ColorCMYK:public ColorCMY{//Cyan-Magenta-Yellow-KeyPlate
	ColorCMYK(uint8 c=0,uint8 m=0,uint8 y=0,uint8 k=0);
	ColorCMYK(const ColorCMY &color);
	ColorCMYK(const ColorRGB &color);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	void fromCMY(const ColorCMY &color);
	void toCMY(ColorCMY &color)const;
	uint8 keyBlack;
};
#endif