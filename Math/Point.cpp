#include"Point.h"

//宏
#define POINT_OPERATOR_2_CPP(TypeA,symbol,TypeB,ReturnType) \
Point2D<ReturnType> operator symbol(const Point2D<TypeA> &pA,const Point2D<TypeB> &pB){return Point2D<ReturnType>(pA.x symbol pB.x,pA.y symbol pB.y);}

#define POINT_OPERATOR_AB_CPP(TypeA,symbol,TypeB,ReturnType) \
POINT_OPERATOR_2_CPP(TypeA,symbol,TypeB,ReturnType) \
POINT_OPERATOR_2_CPP(TypeB,symbol,TypeA,ReturnType) \

//定义
bool operator==(const Point2D<int> &p1,const Point2D<float> &p2){return p1.x==p2.x && p1.y==p2.y;}
bool operator==(const Point2D<float> &p1,const Point2D<int> &p2){return p1.x==p2.x && p1.y==p2.y;}

POINT_OPERATOR_AB_CPP(float,+,SizeType,float)
POINT_OPERATOR_AB_CPP(int,+,float,int)
POINT_OPERATOR_AB_CPP(int,-,float,int)