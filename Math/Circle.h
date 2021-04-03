#ifndef CIRCLE_H
#define CIRCLE_H

#include"Polygon.h"
#include"Number.h"

//圆形
template<typename T>
struct Circle:public PolygonVar2D<T>{
	Point2D<T> center;//圆心
	T radius;//半径

	Circle():radius(0){}
	//设置分段,根据分段去创建点数组
	void setSegment(SizeType seg){
		PolygonVar2D<T>::setSegment(seg);
		//生成各个顶点
		double archer=0;//弧度
		for(SizeType i=0;i<this->segment;++i){
			archer = Number::PI_2 * i / this->segment;
			this->vertex[i].setXY(center.x + radius * ::cos(archer),center.y + radius * ::sin(archer));
		}
	}
};
#endif