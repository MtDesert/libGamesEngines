#ifndef CUBE_H
#define CUBE_H

#include"Point.h"

//立方体
template<typename T>
struct Cube{
	enum Plane{//所有的平面
		PlaneLeft,//左
		PlaneRight,//右
		PlaneDown,//下
		PlaneUp,//上
		PlaneBottom,//底
		PlaneTop,//顶
		AmountOfPlane
	};
	static const int AmountOfLine=12;//边线数量
	Point3D<T> p0,p1;//对角点
};

//浮点立方体
struct CubeF:public Cube<float>{
	static void setCubeF(const CubeF &cubeF);
	float* getLineVertexs(int line)const;
	float* getPlaneVertexs(int plane)const;//获取平面的所有点的集合
};
#endif