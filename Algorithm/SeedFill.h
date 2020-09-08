#ifndef SEEDFILL_H
#define SEEDFILL_H

#include"Cpp11/Containers/Array.h"

//种子填充算法
class SeedFill{
	template<typename SrcType,typename DestType,typename CoordType>
	static void seedFill(const Array2D<SrcType> &srcArray2D,const Array2D<DestType> &destArray2D,const Point2D<CoordType> startP,Array< Point2D<CoordType> > &pointList,int distance,function<void(const Point2D<CoordType> &p,const SrcType &value)> isConnected,function<void(const Point2D<CoordType> &p,const SrcType &value)> writeFun){
		pointList.clear();
		pointList.push_back(startP);
		//运算用变量
		Point2D<CoordType> currentP,nearP,vecP;
		SrcType srcValue;
		const Point2D<int> directions[4]={Point2D<int>(-1,0),Point2D<int>(1,0),Point2D<int>(0,-1),Point2D<int>(0,1)};
		for(SizeType index=0;index<pointList.size();++index){//开始扫描,直到无法填充
			currentP.setP(*pointList.data(index));
			for(int idx=0;idx<4;++idx){
				//判断点是否在范围内
				nearP = currentP + directions[idx];//得到相邻点
				vecP = nearP - startP;//得到原点为起点的向量
				if(vecP.manhattanLength() > distance)return;//判断向量的曼哈顿距离,超出则过滤
				//判断点是否需要处理
				if(srcArray2D.getValue(nearP,srcValue)){
					if(!isConnected || isConnected(nearP,srcValue)){
						pointList.push_back(nearP);//添加,以便再次扫描
						writeFun(nearP,srcValue);//用户自行处理
					}
				}
			}
		}
	}
};
#endif //SEEDFILL_H