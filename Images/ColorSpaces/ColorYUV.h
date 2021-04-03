#ifndef COLORYUV_H
#define COLORYUV_H

#include"ColorRGB.h"
//YUV是一种颜色编码格式,用于照片或视频处理中,产生于黑白电视过渡到彩色电视的时期
struct ColorYUV{
	ColorYUV(uint8 y=0,uint8 u=0,uint8 v=0);

	uint8 y,u,v;//Y为亮度(灰度),uv为颜色
	void fromRGB(const ColorRGB &rgb);
	void toRGB(ColorRGB &rgb)const;
};
struct ColorYCbCr{//Y-ComponentBlue-ComponentRed,ITU-R BT.601 recommendation,用于数字视频,计算机显示器
	ColorYCbCr(COLOR_DECIMAL y=0,COLOR_DECIMAL cb=0,COLOR_DECIMAL cr=0);
	
	void fromRGB(const ColorRGB &rgb);
	void toRGB(ColorRGB &rgb)const;
	COLOR_DECIMAL y,cb,cr;
};
#endif