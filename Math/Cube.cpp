#include"Cube.h"
#include"Polygon.h"
#include"Line.h"

#include<stdio.h>

static CubeF staticCube;
typedef Point3D<float*> vertexType;
typedef Line< vertexType* > lineType;
typedef Polygon4<vertexType*> planeType;
//8个顶点
#define XYZ(A,B) &staticCube.p##A.B
#define VTX(A,B,C) vertexType(XYZ(A,x),XYZ(B,y),XYZ(C,z))
static vertexType allVertexs[]={
	VTX(0,0,0),VTX(1,0,0),VTX(1,1,0),VTX(0,1,0),
	VTX(0,0,1),VTX(1,0,1),VTX(1,1,1),VTX(0,1,1),
};
//12条边
#define LN(A,B) lineType(&allVertexs[A],&allVertexs[B])
static lineType allLines[]={
	LN(0,1),LN(2,3),LN(4,5),LN(6,7),
	LN(0,3),LN(1,2),LN(4,7),LN(5,6),
	LN(0,4),LN(1,5),LN(2,6),LN(3,7),
};
//6个面
#define PLANE(A,B,C,D) planeType(&allVertexs[A],&allVertexs[B],&allVertexs[C],&allVertexs[D])
static planeType allPlanes[]={
	PLANE(0,4,7,3),//左部
	PLANE(1,2,6,5),//右部
	PLANE(0,1,5,4),//下部
	PLANE(2,3,7,6),//上部
	PLANE(0,1,2,3),//底部
	PLANE(4,5,6,7) //顶部
};
#undef PLANE
#undef LN
#undef VTX
#undef XYZ

void CubeF::setCubeF(const CubeF &cubeF){staticCube=cubeF;}

static float vertexOfLine[6]={0};
static float vertexOfPlane[12]={0};

float* CubeF::getLineVertexs(int id)const{
	if(0<=id && id<AmountOfLine){
		auto &ln=allLines[id];
#define LN(index,idx,xyz) vertexOfLine[index]=*(ln.p##idx->xyz);
		LN(0,0,x)LN(1,0,y)LN(2,0,z)
		LN(3,1,x)LN(4,1,y)LN(5,1,z)
#undef LN
	}
	return vertexOfLine;
}
float* CubeF::getPlaneVertexs(int id)const{
	if(0<=id && id<AmountOfPlane){
		auto &vtx=allPlanes[id].vertex;
#define VTX(index,idx,xyz) vertexOfPlane[index]=*(vtx[idx]->xyz);
		VTX(0,0,x)VTX(1,0,y)VTX(2,0,z)
		VTX(3,1,x)VTX(4,1,y)VTX(5,1,z)
		VTX(6,2,x)VTX(7,2,y)VTX(8,2,z)
		VTX(9,3,x)VTX(10,3,y)VTX(11,3,z)
#undef VTX
	}
	return vertexOfPlane;
}