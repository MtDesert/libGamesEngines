#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include"Triangle.h"
#include"Rectangle.h"
#include"ColorRGB.h"

#include<list>
using namespace std;

//图形渲染器,利用现有的绘图引擎(比如OpenGL)来绘制图形
//不同的绘图引擎可能会有差异
class ShapeRenderer{
public:
	ShapeRenderer();
	~ShapeRenderer();
	
	//typedef int numType;
	typedef float numType;
	//成员变量
	bool hasEdge;//是否绘制几何图形的边缘
	bool hasFill;//是否填充图形
	ColorRGBA edgeColor;//这是线的颜色
	ColorRGBA fillColor;//这是闭合图形的填充色
	int texture;//OpenGL的当前纹理,可混合填充色
	
	static void setColor(const ColorRGBA &color);//设置当前绘制的颜色(OpenGL的话则是传给状态机)

	//画点
	void drawPoint(numType x,numType y)const;
	void drawPoint(const Point2D<numType> &p)const;
	void drawPoints(const list<Point2D<numType> > &points)const;
	void drawPoints(const numType vertex[],int n)const;//注意n是点数,vertex[2*n]才是数组
	//画直线
	void drawLine(numType x0,numType y0,numType x1,numType y1)const;
	void drawLine(const Point2D<numType> p0,const Point2D<numType> p1)const;
	void drawLine(const Line2D<numType> &line)const;
	void drawLines(const list<Line2D<numType> > &lines);
	void drawLines(const numType vertex[],int n)const;
	//画折线
	void drawBrokenLine(const list<Point2D<numType> > &points)const;

	//画三角形(填充为fillColor色)
	void drawTriangle(numType x0,numType y0,numType x1,numType y1,numType x2,numType y2)const;
	void drawTriangle(const Point2D<numType> p0,const Point2D<numType> p1,const Point2D<numType> p2)const;
	void drawTriangle(const Triangle2D<numType> &triangle)const;
	void drawTriangle(const numType vertex[])const;
	//画矩形
	void drawRectangle(numType x0,numType y0,numType x1,numType y1)const;
	void drawRectangle(const Point2D<numType> &p0,const Point2D<numType> &p1)const;
	void drawRectangle(const Rectangle2D<numType> &rect)const;
	void drawRectangle(const numType vertex[])const;
	//画多边形(顶点数组,顶点数量)
	void drawPolygen(const numType vertex[],int n)const;//注意:n是顶点数,不是数组大小
};

#endif
