#ifndef COLORRGB_H
#define COLORRGB_H

#include"typedef.h"
#define RGB_MAX 0xFF
#define COLOR_DECIMAL float

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
	//红绿蓝
	uint8 red,green,blue;
	//operator
	bool operator==(const ColorRGB &color)const;

	//to unsigned 24 bit
	COLOR_UNSIGNED24(RGB)
	COLOR_UNSIGNED24(RBG)
	COLOR_UNSIGNED24(GRB)
	COLOR_UNSIGNED24(GBR)
	COLOR_UNSIGNED24(BRG)
	COLOR_UNSIGNED24(BGR)

	//灰度,即此颜色对应的灰色(RGB值都相等),根据不同的标准,灰度的算法也有差异
	uint8 grayInt()const;//获取灰度
	COLOR_DECIMAL gray()const;//获取灰度(小数)
	void toGray();//转换成灰度颜色
	static const COLOR_DECIMAL grayR,grayG,grayB;

	//颜色变换
	ColorRGB darkColor(const ColorRGB &color)const;//暗色,根据color来计算本颜色的暗色
	ColorRGB lightColor(const ColorRGB &color)const;//亮色,根据color来计算本颜色的亮色
	ColorRGB alphaBlend(const ColorRGB &rgb,uint8 alpha)const;//alpha混合色,根据color及其alpha值来计算混合后的颜色

	//差异度,计算此颜色和rgb的差异程度
	int deltaSum(const ColorRGB &rgb)const;//计算rgb的各个分量的差值之和
	double distance(const ColorRGB &rgb)const;//计算此颜色和rgb在RGB坐标系中对应点的距离
};

struct ColorRGBA:public ColorRGB{//RGBA(Red-Green-Blue-Alpha)
	//constructor
	ColorRGBA();
	ColorRGBA(uint8 r,uint8 g,uint8 b,uint8 a=255);
	explicit ColorRGBA(unsigned value);//0xAABBGGRR

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

	//明暗
	ColorRGBA darkColor(const ColorRGBA &color)const;//暗色,根据color来计算本颜色的暗色
	ColorRGBA lightColor(const ColorRGBA &color)const;//亮色,根据color来计算本颜色的亮色
	//Alpha混合(rgba为一起混合的颜色),返回混合结果
	ColorRGBA alphaBlend(const ColorRGBA &rgba)const;

	uint8 alpha;
	//默认颜色
	static const ColorRGBA Transparent,Black,Red,Green,Blue,Gray,Yellow,Purple,Cyan,White;
};
#endif