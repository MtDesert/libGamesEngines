#ifndef PIXMAP_H
#define PIXMAP_H

#include"Bitmap.h"
#include"Array.h"
#include"Rectangle.h"
#include"Map.h"

/*Pixmap是以像素值为单位来保存一张具体的二维图的
Pixmap的pix全称是pixel即像素,每个像素其实就是颜色值,比如RGB(A)或YUV等
但并不限于刚提到的颜色,具体的类型由模板类型T决定

比如你要画游戏的方格地图的时候(假设是画地形),可以写
Pixmap<Terrain> myMap;//Terrain为用户自己定义的地形枚举或结构
Pixmap主要用于支持Painter工作,具体请看Painter.h

注意:如果T是自定义的RGB(A)结构体,并直接传递给opengl的话可能会导致数据错误
具体原因不明,猜测可能是结构体除了成员变量占用还有其它东西(比如有虚函数的情况下)
最保险的做法是用连续的字节来存各个像素颜色值再传递给opengl(毕竟opengl是c写的)
*/
template<typename T>
struct Pixmap:public Array2D_LV1_Pointer<T>{
	//读取像素值的接口
	inline bool getColor(uint x,uint y,T &color)const{return this->getValue(x,y,color);}
	inline bool getColor(const Point2D<uint> &p,T &color)const{return this->getValue(p,color);}
	inline bool getColor(const Point2D<int> &p,T &color)const{return this->getValue(p.x,p.y,color);}
	//写入像素值的接口
	inline bool setColor(uint x,uint y,const T &color){return this->setValue(x,y,color);}
	inline bool setColor(const Point2D<uint> &p,const T &color){return this->setValue(p,color);}
	inline bool setColor(const Point2D<int> &p,const T &color){return this->setValue(p.x,p.y,color);}

	//转换表,转换成Bitmap的时候用
	Map<T,uint32> colorMap;
	//转换到bitmap
	bool toBitmap(Bitmap_32bit &bitmap)const{
		if(colorMap.size()==0)return false;
		if(!bitmap.newBitmap(this->width,this->height))return false;
		T color;
		uint32 *val=nullptr;
		for(uint y=0;y<this->height;++y){
			for(uint x=0;x<this->width;++x){
				if(getColor(x,y,color)){
					val=colorMap.value(color);
					if(val){
						bitmap.setColor(x,y,*val);
					}
				}
			}
		}
		return true;
	}
};
#endif
