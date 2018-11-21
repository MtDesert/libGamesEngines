#ifndef PIXMAP_H
#define PIXMAP_H

#include"FileStructs/FileBMP.h"
#include"ColorRGB.h"
#include"Array.h"
#include<stdio.h>

template<typename T>
struct Pixmap:public Array2D_LV1_Pointer<T>
{
	inline bool getColor(uint x,uint y,T &color)const{return this->getValue(x,y,color);}
	inline bool setColor(uint x,uint y,const T &color){return this->setValue(x,y,color);}
	const T* dataPointer()const{
		return this->data;
	}
};
#endif