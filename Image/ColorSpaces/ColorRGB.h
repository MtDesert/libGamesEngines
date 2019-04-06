#ifndef COLORRGB_H
#define COLORRGB_H

#include"typedef.h"

#define COLOR_UNSIGNED(Name)\
	uint32 to##Name()const;\
	void from##Name(unsigned val);

#define COLOR_UNSIGNED24(Name) COLOR_UNSIGNED(Name)
#define COLOR_UNSIGNED32(Name) COLOR_UNSIGNED(Name)

struct ColorRGB{//Red-Green-Blue
	//constructor
	ColorRGB();
	ColorRGB(uint8 r,uint8 g,uint8 b);
	ColorRGB(unsigned value);//0x00RRGGBB

	//operator
	bool operator==(const ColorRGB &color)const;

	//to unsigned 24 bit
	COLOR_UNSIGNED24(RGB)
	COLOR_UNSIGNED24(RBG)
	COLOR_UNSIGNED24(GRB)
	COLOR_UNSIGNED24(GBR)
	COLOR_UNSIGNED24(BRG)
	COLOR_UNSIGNED24(BGR)

	//feature
	uint8 gray()const;
	void toGray();

	//member variable
	uint8 red,green,blue;
};

struct ColorRGBA:public ColorRGB{//RGBA(Red-Green-Blue-Alpha)
	//constructor
	ColorRGBA();
	ColorRGBA(uint8 r,uint8 g,uint8 b,uint8 a=255);
	explicit ColorRGBA(unsigned value);//0xAARRGGBB

	//operator
	bool operator==(const ColorRGBA &color)const;
	ColorRGBA& operator=(const ColorRGB &color);

	//to unsigned 32 bit
	COLOR_UNSIGNED32(RGBA)
	COLOR_UNSIGNED32(RBGA)
	COLOR_UNSIGNED32(GRBA)
	COLOR_UNSIGNED32(GBRA)
	COLOR_UNSIGNED32(BRGA)
	COLOR_UNSIGNED32(BGRA)

	COLOR_UNSIGNED32(RGAB)
	COLOR_UNSIGNED32(RBAG)
	COLOR_UNSIGNED32(GRAB)
	COLOR_UNSIGNED32(GBAR)
	COLOR_UNSIGNED32(BRAG)
	COLOR_UNSIGNED32(BGAR)

	COLOR_UNSIGNED32(RAGB)
	COLOR_UNSIGNED32(RABG)
	COLOR_UNSIGNED32(GARB)
	COLOR_UNSIGNED32(GABR)
	COLOR_UNSIGNED32(BARG)
	COLOR_UNSIGNED32(BAGR)

	COLOR_UNSIGNED32(ARGB)
	COLOR_UNSIGNED32(ARBG)
	COLOR_UNSIGNED32(AGRB)
	COLOR_UNSIGNED32(AGBR)
	COLOR_UNSIGNED32(ABRG)
	COLOR_UNSIGNED32(ABGR)

	//member variable
	uint8 alpha;
};
#endif