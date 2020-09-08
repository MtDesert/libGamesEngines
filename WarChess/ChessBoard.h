#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include"ChessPiece.h"
#include"Array.h"

/**棋盘是个容纳棋子的容器
大多数棋盘是2D,故ChessBoard的基类定为二维数组,使用二级指针的目的是为了能创建超大棋盘(比如1000*1000)
战棋游戏和棋类游戏有一些不同,战棋游戏大多数是有地形的(即考虑地形因素),而且不同的游戏中棋子所包含的数据不同,所以用模板
ChessPieceType 使用ChessPiece的子类
 */
template<typename Lattice,typename ChessPieceType>
class ChessBoard:public Array2D_LV2_Pointer<Lattice>{
public:
	Array<ChessPieceType> chessPieces;//棋子表,在此表中的棋子可以显示在棋盘上
	//构造函数
	ChessBoard(){}
	ChessBoard(uint width,uint height):Array2D_LV2_Pointer<Lattice>(width,height){}
	ChessBoard(const ChessBoard<Lattice,ChessPieceType> &another):Array2D_LV2_Pointer<Lattice>(another){
		chessPieces=another.chessPieces;
	}

	//棋子操作
	//放棋子(横坐标,纵坐标,棋子),被引用的棋子坐标会被修改,返回是否成功
	bool putChessPiece(uint x,uint y,ChessPieceType &cpt){
		if(!this->isInRange(x,y))return false;//放入失败
		cpt.coordinate.x=x;
		cpt.coordinate.y=y;
		chessPieces.push_back(cpt);
		return true;
	}
	//放棋子(坐标,棋子),被引用的棋子坐标会被修改,返回是否成功
	bool putChessPiece(const Point2D<uint> &p,ChessPieceType &cpt){return putChessPiece(p.x,p.y,cpt);}

	//移除棋子
	bool removeChessPiece(uint x,uint y){
		return true;
	}
	void removeAllChessPieces(){chessPieces.clear();}

	//获取棋子(横坐标,纵坐标,查询结果),返回是否查询成功
	bool getChessPiece(uint x,uint y,ChessPieceType &cpt)const{
		for(auto &cp:chessPieces){
			if(cp.coordinate.x==x && cp.coordinate.y==y){
				cpt=cp;
				return true;
			}
		}
		return false;//查不到
	}
	//获取棋子(坐标,查询结果),返回是否查询成功
	bool getChessPiece(const Point2D<uint> &p,ChessPieceType &cpt){return getChessPiece(p.x,p.y,cpt);}
	//获取棋子(横坐标,纵坐标,查询结果),返回查询到的个数,注意:cptList不会被清空
	SizeType getChessPieces(uint x,uint y,list<ChessPieceType> &cptList)const{
		SizeType ret=0;
		for(auto &cp:chessPieces){
			if(cp.coordinate.x==x && cp.coordinate.y==y){
				cptList.push_back(cp);
				++ret;
			}
		}
		return ret;
	}
	SizeType getChessPieces(const Point2D<uint> &p,ChessPieceType &cptList)const{
		return getChessPieces(p.x,p.y,cptList);
	}
	//格子操作
	void forEachLattice(function<void(uint,uint,Lattice&)> callback){
		this->forEach(callback);
	}
	void forEachLattice(function<void(uint,uint,const Lattice&)> callback)const{
		this->forEach(callback);
	}
};
#endif