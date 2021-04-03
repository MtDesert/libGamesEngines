#ifndef COLORHS_H
#define COLORHS_H

#include"ColorRGB.h"
//HSL,HSV色彩模式,主要用于色彩分析,选择,调整等

struct ColorHS{//Hue and Saturation
	ColorHS(COLOR_DECIMAL h=0,COLOR_DECIMAL s=0);

	COLOR_DECIMAL hue;//色调
	COLOR_DECIMAL saturation;//饱和度

	static const COLOR_DECIMAL maxHue;
	static const COLOR_DECIMAL maxSaturation;

	static uint8 minRGB(const ColorRGB &rgb);//rgb分量中的最小值
	static uint8 maxRGB(const ColorRGB &rgb);//rgb分量中的最小值
	void caculateHue(const ColorRGB &rgb,uint8 minRGB,uint8 maxRGB);//计算色相
	void toRGB(ColorRGB &rgb,uint8 minRGB,uint8 maxRGB)const;
	//RGB互转
	virtual void fromRGB(const ColorRGB &color)=0;
	virtual void toRGB(ColorRGB &color)const=0;
};
struct ColorHSL:public ColorHS{//Hue-Saturation-Lightness, equal HSI(Hue-Saturation-Intensity)
	ColorHSL(COLOR_DECIMAL h=0,COLOR_DECIMAL s=0,COLOR_DECIMAL l=0);

	COLOR_DECIMAL lightness;//亮度
	static const COLOR_DECIMAL maxLightness;
	//RGB互转
	void fromRGB(const ColorRGB &rgb);
	void toRGB(ColorRGB &rgb)const;
};
struct ColorHSV:public ColorHS{//Hue-Saturation-Value, equal HSB(Hue-Saturation-Brightness)
	ColorHSV(uint16 h=0,uint8 s=0,uint8 v=0);

	uint8 value;//明度
	//RGB互转
	void fromRGB(const ColorRGB &rgb);
	void toRGB(ColorRGB &rgb)const;
};
#endif