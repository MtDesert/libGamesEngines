#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include"Point.h"
#include"typedef.h"

//棋子模板,对于棋子来说,基本信息就是棋子的类型(棋子是什么,比如象棋的不同兵种),棋子的颜色(区分敌我),棋子的坐标
template<typename CorpType,typename Color>
struct ChessPiece{
	//棋子属性
	CorpType corpType;//兵种
	Color color;//颜色
	Point2D<int16> coordinate;//坐标
	//构造函数
	ChessPiece(){}
	ChessPiece(const CorpType &ct,const Color &clr,const decltype(coordinate) &p):corpType(ct),color(clr),coordinate(p){}
	//运算符重载
	bool operator==(const ChessPiece &cp)const{
		return corpType==cp.corpType && color==cp.color && coordinate==cp.coordinate;
	}
};
#endif