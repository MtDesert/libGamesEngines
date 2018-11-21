#ifndef COLORHS_H
#define COLORHS_H

#include"ColorRGB.h"
struct ColorHS{//Hue and Saturation
	ColorHS(uint16 h=0,uint8 s=0);
	virtual void fromRGB(const ColorRGB &color)=0;
	virtual void toRGB(ColorRGB &color)const=0;
	uint16 hue;
	uint8 saturation;

	static const uint16 maxHue;
	static const uint8 maxSaturation;
};
struct ColorHSL:public ColorHS{//Hue-Saturation-Lightness, equal HSI(Hue-Saturation-Intensity)
	ColorHSL(uint16 h=0,uint8 s=0,uint8 l=0);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 lightness;

	static const uint8 maxLightness;
};
struct ColorHSV:public ColorHS{//Hue-Saturation-Value, equal HSB(Hue-Saturation-Brightness)
	ColorHSV(uint16 h=0,uint8 s=0,uint8 v=0);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 value;

	static const uint8 maxValue;
};
#endif