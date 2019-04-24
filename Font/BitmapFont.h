#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include"Pixmap.h"

/*点阵字体,游戏机中常用这种字体(毕竟是机能限制,字体应该尽量小到人还能看得出字的内容)
本类的点阵字体目前只处理2色

点阵字体和矢量字体的区别:
点阵字体是直接用绘制好的图来显示,矢量字体是根据每个轮廓的关键点来进行画线推算的
点阵字体放大后会看到马赛克(像素放大),矢量字体无论怎么放大都几乎看不出马赛克(动态绘制)
字体尺寸比较小的情况下,点阵字体生成比较快(已经是生成好的结果,数据量少),矢量字体需要进行计算(费时间绘制)
字体尺寸比较大的情况下,点阵字体占空间较大(存储空间换绘制时间),矢量字体几乎没有变化(绘制时间换存储空间)
根据以上区别
点阵字体大多用于小尺寸,或者运算速度不是很快,存储空间比较小的嵌入式芯片中(比如早期的游戏机)
矢量字体大多用于任意尺寸,存储空间足够大(至少要能装得下字体渲染程序),运算速度够快的设备上(电脑和智能手机都是)
请根据实际情况选择合适的方案
*/
class BitmapFont{
public:
	BitmapFont();
	virtual ~BitmapFont();

	Pixmap<bool> charPixmap;//字符的像素图,用于转化
};

class BitmapFont_Ascii:public BitmapFont{
public:
	BitmapFont_Ascii();
	~BitmapFont_Ascii();

	bool renderCharCode(uint8 charCode);//charCode是ASCII字符
	DataBlock charBlock;//字形数据
};

//GB2312的点阵字体
class BitmapFont_GB2312:public BitmapFont{
public:
	BitmapFont_GB2312();
	~BitmapFont_GB2312();

	bool renderCharCode(uint16 charCode);//渲染文字(charCode是gb2312而不是unicode),返回是否渲染成功
	//数据源
	DataBlock symbolBlock;//符号区
	DataBlock lv1Chinese;//一级汉字
	DataBlock lv2Chinese;//二级汉字
};

#endif
