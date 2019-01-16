#include"ShapeRenderer.h"
#include"GL/gl.h"

ShapeRenderer::ShapeRenderer():hasEdge(true),hasFill(true),edgeColor(0xFFFFFFFF),fillColor(0xFFFFFFFF),texture(0){}
ShapeRenderer::~ShapeRenderer(){}

void ShapeRenderer::setColor(const ColorRGBA &color){
	glColor4ub(color.red,color.green,color.blue,color.alpha);
}

//画点
void ShapeRenderer::drawPoint(numType x,numType y)const{
	numType vertex[2];
	vertex[0]=x;
	vertex[1]=y;
	drawPoints(vertex,1);
}
void ShapeRenderer::drawPoint(const Point2D<numType> &p)const{
	drawPoints(p.data,1);
}
void ShapeRenderer::drawPoints(const list<Point2D<numType> > &points)const{
	numType vertex[points.size()*2];
	size_t i=0;
	for(auto &p:points){
		vertex[i]=p.x();
		vertex[i+1]=p.y();
		i+=2;
	}
	drawPoints(vertex,points.size());
}
void ShapeRenderer::drawPoints(const numType vertex[],int n)const{
	glBindTexture(GL_TEXTURE_2D,0);
	setColor(edgeColor);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	glDrawArrays(GL_POINTS,0,n);
}

//画线
#define SHAPERENDERER_DRAWLINE(x0,y0,x1,y1) \
numType vertex[4];\
vertex[0]=x0;\
vertex[1]=y0;\
vertex[2]=x1;\
vertex[3]=y1;\
drawLines(vertex,1);

void ShapeRenderer::drawLine(numType x0,numType y0,numType x1,numType y1)const{
	SHAPERENDERER_DRAWLINE(x0,y0,x1,y1)
}
void ShapeRenderer::drawLine(const Point2D<numType> p0,const Point2D<numType> p1)const{
	SHAPERENDERER_DRAWLINE(p0.x(),p0.y(),p1.x(),p1.y())
}
void ShapeRenderer::drawLine(const Line2D<numType> &line)const{
	SHAPERENDERER_DRAWLINE(line.p0.data[0],line.p0.data[1],line.p1.data[0],line.p1.data[1])
}

void ShapeRenderer::drawLines(const numType vertex[],int n)const{
	glBindTexture(GL_TEXTURE_2D,0);
	setColor(edgeColor);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	glDrawArrays(GL_LINES,0,n*2);
}

//折线
void ShapeRenderer::drawBrokenLine(const list<Point2D<numType> > &points)const{
	numType vertex[points.size()*2];
	size_t i=0;
	for(auto &p:points){
		vertex[i]=p.x();
		vertex[i+1]=p.y();
		i+=2;
	}
	glBindTexture(GL_TEXTURE_2D,0);
	setColor(edgeColor);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	glDrawArrays(GL_LINE_STRIP,0,points.size()*2);
}

//三角形
#define SHAPERENDERER_DRAW_TRIANGLE(x0,y0,x1,y1,x2,y2) \
numType vertex[6];\
vertex[0]=x0;\
vertex[1]=y0;\
vertex[2]=x1;\
vertex[3]=y1;\
vertex[4]=x2;\
vertex[5]=y2;\
drawTriangle(vertex);

void ShapeRenderer::drawTriangle(numType x0,numType y0,numType x1,numType y1,numType x2,numType y2)const{
	SHAPERENDERER_DRAW_TRIANGLE(x0,y0,x1,y1,x2,y2)
}
void ShapeRenderer::drawTriangle(const Point2D<numType> p0,const Point2D<numType> p1,const Point2D<numType> p2)const{
	SHAPERENDERER_DRAW_TRIANGLE(p0.x(),p0.y(),p1.x(),p1.y(),p2.x(),p2.y())
}
void ShapeRenderer::drawTriangle(const Triangle2D<numType> &triangle)const{
	SHAPERENDERER_DRAW_TRIANGLE(
		triangle.p0.data[0],triangle.p0.data[1],
		triangle.p1.data[0],triangle.p1.data[1],
		triangle.p2.data[0],triangle.p2.data[1])
}
void ShapeRenderer::drawTriangle(const numType vertex[])const{
	drawPolygen(vertex,3);
}

//画矩形
#define SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)\
numType vertex[8];\
vertex[0]=vertex[6]=x0;\
vertex[1]=vertex[3]=y0;\
vertex[2]=vertex[4]=x1;\
vertex[5]=vertex[7]=y1;\
drawRectangle(vertex);

void ShapeRenderer::drawRectangle(numType x0,numType y0,numType x1,numType y1)const{
	SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)
}
void ShapeRenderer::drawRectangle(const Point2D<numType> &p0,const Point2D<numType> &p1)const{
	SHAPERENDERER_DRAW_RECTANGLE(p0.x(),p0.y(),p1.x(),p1.y())
}
void ShapeRenderer::drawRectangle(const Rectangle2D<numType> &rect)const{
	SHAPERENDERER_DRAW_RECTANGLE(rect.p0.x(),rect.p0.y(),rect.p1.x(),rect.p1.y())
}
void ShapeRenderer::drawRectangle(const numType vertex[])const{
	drawPolygen(vertex,4);
}
//多边形
void ShapeRenderer::drawPolygen(const numType vertex[],int n)const{
	glBindTexture(GL_TEXTURE_2D,texture);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	if(hasFill){//绘制底色或纹理
		setColor(fillColor);
		glDrawArrays(GL_TRIANGLE_FAN,0,n);
	}
	if(hasEdge){//绘制边框线
		setColor(edgeColor);
		glDrawArrays(GL_LINE_LOOP,0,n);
	}
}

/*
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
*/
