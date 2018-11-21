#ifndef COLORYUV_H
#define COLORYUV_H

#include"ColorRGB.h"
//Luma plus chroma/chrominance
struct ColorYUV{
	ColorYUV(uint8 y=0,uint8 u=0,uint8 v=0);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 y,u,v;

	//const
	static uint16 weightRGB();
	static const uint16 weightR,weightG,weightB;
	static const uint16 maxU,maxV;
};
struct ColorYCbCr{//Y-ComponentBlue-ComponentRed
	ColorYCbCr(uint8 y=0,uint8 cb=0,uint8 cr=0);
	
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 y,cb,cr;
};
struct ColorYIQ{//Y-InParse-QuadraturePhase
	ColorYIQ(uint8 y=0,uint8 i=0,uint8 q=0);

	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
	uint8 y,i,q;
};

#endif