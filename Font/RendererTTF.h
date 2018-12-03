#ifndef RENDERERTTF_H
#define RENDERERTTF_H

#include"FileTTF.h"
#include"FileTTF_cmap.h"
#include"FileTTF_glyf.h"
#include"Pixmap.h"
#include"Point.h"

#include<vector>

/*RendererTTF是一个字体渲染器,关联一个字体文件后,只要传递文字的编码就可以绘制字体
大多数.ttf文件可能都用unicode编码来查询,也有可能不用(当然我不能每个字体文件都去剖析)
文本文件中存储文字用的字符集基本上是非unicode的(纯ASCII的我就不管了)
比如Windows下的默认字体,中国的gb2312,gbk,gb18030,日本的Shift-J,Linux及其派生系统下默认用的utf8等
通过iconv可以进行转换,具体的命令行用法和编程用法请自行查资料
绘制出来的文字是"黑底白字"(实际上就是黑白矩阵),可以根据需要自行转换成需要的颜色,以便实现各种效果
*/
class RendererTTF{
	//主要用到的表结构(用于查询)
	FileTTF_cmap::SubTable *cmapSubTable;//charCode -> glyphIndex
	FileTTF_loca loca;//glyphIndex -> glyfOff,glyfLen
	FileTTF_hmtx hmtx;//glyphIndex -> leftBearing,advanceWidth
	FileTTF_glyf glyf;//glyfOff,glyfLen -> glyfData
	//主要变量,控制绘制流程的关键
	uint16 unitsPerEm;
	int16 numCoutours,xMin,yMin,xMax,yMax;
	DataBlock glyfFlags,glyfXcoordinate,glyfYcoordinate;
	vector<uint8> flags;
	vector<Point2D<int32> > xyCoordinate;
	//外边框的范围
	Point2D<int> boxMin,boxMax;
	void outlineBox();
	//绘制字体
	void drawGlyph(int16 startPtr,int16 endPtr);//画轮廓
	void fillGlyphs();//填充字体
public:
	RendererTTF();
	
	bool initialize(const FileTTF &fileTTF);//根据字体文件进行初始化
	bool uninitialize();//清空
	
	int charWidth,charHeight;//ASCII字符的宽高(汉字应该用此宽度*2)
	bool renderCharCode(uint16 charCode);//渲染字符(charCode为unicode)
	//文字的最终绘制结果
	Pixmap<uint8> charPixmap;
};
#endif
