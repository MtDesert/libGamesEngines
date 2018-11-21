#ifndef COLORCIE_H
#define COLORCIE_H

#include"ColorRGB.h"
//CIE(Commission Internationale de L'Eclairage, French)
struct ColorCIERGB{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 red,green,blue;
};
struct ColorCIEXYZ{
	enum Type{REC601,REC709,ITU};
	ColorCIEXYZ(uint8 x=0,uint8 y=0,uint8 z=0,Type type=REC601);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;

	uint8 x,y,z;
	Type type;
};

struct ColorCIELAB{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};
struct ColorCIELCH{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};
struct ColorCIELUV{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};
struct ColorCIEUCS{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};
struct ColorCIEUVW{
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};

#endif