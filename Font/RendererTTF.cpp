#include"RendererTTF.h"
#include"Painter.h"

RendererTTF::RendererTTF():cmapSubTable(nullptr),unitsPerEm(0){}

bool RendererTTF::initialize(const FileTTF &fileTTF){
	FileTTF_head head;
	FileTTF_hhea hhea;
	FileTTF_cmap cmap;
	if(!fileTTF.table("head",head))return false;
	if(!fileTTF.table("cmap",cmap))return false;
	if(!fileTTF.table("hhea",hhea))return false;
	if(!fileTTF.table("loca",loca))return false;
	if(!fileTTF.table("hmtx",hmtx))return false;
	if(!fileTTF.table("glyf",glyf))return false;
	//寻找unicode字符集,根据字符码获取字形索引(cmap)
	cmapSubTable=cmap.newSubTable_Unicode();
	if(!cmapSubTable)return false;
	//根据字形索引获取字形位置(loca,head)
	if(!head.getUnitsPerEm(unitsPerEm))return false;
	if(!head.getIndexToLocFormat(loca.version))return false;
	loca.glyfDataLength=glyf.dataLength;
	//查询水平表信息(hhea,hmtx)
	if(!hhea.getNumberOfHMetrics(hmtx.numberOfHMetrics))return false;
	//
	charWidth=unitsPerEm;
	charHeight=unitsPerEm;
	charPixmap.newData(charWidth,charHeight);
	//设定pixmap的色表(影响输出字体的颜色)
	charPixmap.colorMap.insert(0x00,0x00000000);//底色
	charPixmap.colorMap.insert(0x01,0xFFFFFFFF);//轮廓色
	charPixmap.colorMap.insert(0x02,0xFFFFFFFF);//填充色
	//charPixmap.colorMap.insert(std::pair<uchar,uint32>(0x03,0xFF00FF00));//
	//charPixmap.colorMap.insert(std::pair<uchar,uint32>(0x04,0xFF0000FF));//
	return true;
}
bool RendererTTF::uninitialize(){
	if(cmapSubTable)delete cmapSubTable;
	charPixmap.deleteData();
	return true;
}

void RendererTTF::outlineBox(){
	boxMin.x()=boxMin.y()=boxMax.x()=boxMax.y()=0;
	if(xyCoordinate.size()){
		auto p=xyCoordinate[0];
		boxMin.x()=boxMax.x()=p.x();
		boxMin.y()=boxMax.y()=p.y();
		for(size_t i=1;i<xyCoordinate.size();++i){
			p=xyCoordinate[i];
			if(p.x()<boxMin.x())boxMin.x()=p.x();
			if(p.x()>boxMax.x())boxMax.x()=p.x();
			if(p.y()<boxMin.y())boxMin.y()=p.y();
			if(p.y()>boxMax.y())boxMax.y()=p.y();
		}
	}
}

void RendererTTF::drawGlyph(int16 startPtr,int16 endPtr){
	Painter<uchar> painter(charPixmap);
	painter.color=1;
	//在这里绘制每个轮廓,先找起点
	int16 idx=startPtr;
	Point2D<int> startP=xyCoordinate[startPtr],endP=xyCoordinate[endPtr];
	if (flags[startPtr] & FileTTF_glyf::Flag_OnCurve){//起点在线上,我们从下个点开始扫描
		++idx;
	}else{//起点不在线上,是控制点
		if (flags[endPtr] & FileTTF_glyf::Flag_OnCurve){//终点在点上,设置为起点
			startP = endP;
			--endPtr;
		}else{//终点不在线上,直接取首点和末点的平均值作为新起点
			startP=(startP+endP)/2;
		}
	}
	//开始画轮廓
	auto lastP=startP;
	while(idx<=endPtr){
		if(flags[idx] & FileTTF_glyf::Flag_OnCurve){//点在线上,是直线,直接画
			painter.drawLine(lastP,xyCoordinate[idx]);
			lastP = xyCoordinate[idx];
		}else{//点不在线上,是曲线的控制点,我们来找曲线的终点
			list< Point2D<int> > curvePoints;
			curvePoints.push_back(lastP);
			while(idx<=endPtr){
				curvePoints.push_back(xyCoordinate[idx]);
				if(flags[idx] & FileTTF_glyf::Flag_OnCurve){//点在曲线上,画线结束
					break;
				}
				//下一个
				++idx;
			}
			if(idx>endPtr){//若此条件成立,说明最后一个点不在曲线上,那么我们需要连回起点
				curvePoints.push_back(startP);
				lastP=startP;
			}else{
				lastP=xyCoordinate[idx];
			}
			painter.drawBezier(curvePoints);
		}
		++idx;
	}
	if(flags[endPtr] & FileTTF_glyf::Flag_OnCurve){//如果最后一个点在线上,我们直接和起点相连
		painter.drawLine(lastP,startP);
	}
}
void RendererTTF::fillGlyphs(){
	uchar color;
	bool isLine=false;//指示当前扫描的点是不是在轮廓上
	struct Segment{//扫描线上检测到的线段(如果是点则l和r相等)
		Segment():l(0),r(0),drawTo(0){}
		int l,r;
		char drawTo;//要往哪个方向画,-1左,0无,1右
		bool isConnectedWith(const Segment &seg)const{//邻接性判断
			int l0=l-1,r0=r+1;
			if(l0<=seg.l && seg.l<=r0)return true;
			if(l0<=seg.r && seg.r<=r0)return true;
			return false;
		}
	};
	Segment seg;//缓存扫描到的水平线段
	seg.l=seg.r=-1;
	list<Segment> segList[2],*curSegList=&segList[0],*prevSegList=&segList[1];//双缓冲
	auto w=charPixmap.getWidth();
	auto h=charPixmap.getHeight();
	for(uint y=0;y<h;++y){
		//扫描每行的线段数并保存
		seg.drawTo=0;
		for(uint x=0;x<w;++x){
			if(charPixmap.getColor(x,y,color)){
				//检测有没有碰到线段
				if(color){
					if(!isLine)seg.l=x;//线段起点
					seg.r=x;//线段终点(动态更新)
				}else{
					if(isLine){//线段结束,可以先缓存
						curSegList->push_back(seg);
					}
				}
				isLine=color;
			}
		}
		//寻找可能的空白区间
		bool needDraw=false;
		for(auto itrL=curSegList->begin();itrL!=curSegList->end();++itrL){
			auto itrR=itrL;++itrR;
			if(itrR==curSegList->end())break;
			//同一行上,我们得到了两条相邻的轮廓线段itrL,itrR,接下来需要判断是否需要填充
			needDraw=false;
			for(auto itrPrev=prevSegList->begin();itrPrev!=prevSegList->end();++itrPrev){
				//auto itrPrev=prevSegList->data(j);
				//判断是不是与上一行的同一个线段相连
				bool conL=itrPrev->isConnectedWith(*itrL);
				bool conR=itrPrev->isConnectedWith(*itrR);
				if(conL && conR){//itrL和itrR相连的情况
					if(needDraw){//往外
						itrL->drawTo-=1;
						itrR->drawTo+=1;
					}else if(itrPrev->drawTo==0){//往内
						itrL->drawTo+=1;
						itrR->drawTo-=1;
					}
				}else if(conL){
					if(itrPrev->drawTo>0)itrL->drawTo+=1;else
					if(itrPrev->drawTo<0)itrL->drawTo-=1;else
					if(itrPrev->drawTo==0){
						if(needDraw)itrL->drawTo+=1;
					}
				}else if(conR){
					if(itrPrev->drawTo<0)itrR->drawTo-=1;else
					if(itrPrev->drawTo>0)itrR->drawTo+=1;else
					if(itrPrev->drawTo==0){
						if(needDraw)itrR->drawTo-=1;
					}
				}
				//改变绘制状态
				if(itrPrev->drawTo>0)needDraw=true;else
				if(itrPrev->drawTo<0)needDraw=false;
			}
		}
		//画线
		for(auto itrL=curSegList->begin();itrL!=curSegList->end();++itrL){
			auto itrR=itrL;++itrR;
			if(itrR==curSegList->end())break;
			//尝试绘画
			if(itrL->drawTo>0){
				needDraw=true;
				for(int i=itrL->l;i<=itrL->r;++i)charPixmap.setColor(i,y,3);//debug用
			}
			if(needDraw){
				for(int i=itrL->r+1;i<itrR->l;++i){
					charPixmap.setColor(i,y,2);
				}
			}
			if(itrR->drawTo<0){
				needDraw=false;
				for(int i=itrR->l;i<=itrR->r;++i)charPixmap.setColor(i,y,4);//debug用
			}
		}
		//交换,准备扫描下一行
		prevSegList->clear();
		auto tmp=curSegList;
		curSegList=prevSegList;
		prevSegList=tmp;
	}
}

bool RendererTTF::renderCharCode(uint16 charCode){
	if(!cmapSubTable || unitsPerEm<=0 || charWidth<=0 || charHeight<=0)return false;

	//charCode -> glyphIndex
	uint32 glyphIndex;
	if(!cmapSubTable->glyphIndex(charCode,glyphIndex))return false;
	//printf("charCode==%.4X -> glyphIndex==%.4X\n",charCode,glyphIndex);

	//glyphIndex -> glyfOff,glyfLen
	uint32 glyfOff,glyfLen;
	if(!loca.glyphLocation(glyphIndex,glyfOff,glyfLen))return false;
	//printf("glyph: off==%.8X len==%d\n",glyfOff,glyfLen);

	//glyphIndex -> leftBearing,advanceWidth
	int16 leftBearing=0;uint16 advanceWidth=0;
	if(!hmtx.horizontalMetrics(glyphIndex,leftBearing,advanceWidth))return false;
	//printf("glyph: left==%d advance==%d\n",leftBearing,advanceWidth);
	
	//glyfOff,glyfLen -> glyfData
	FileTTF_glyf glyfData;
	if(!glyf.subDataBlock(glyfOff,glyfLen,glyfData))return false;
	glyfData.parseData();

	if(!glyfData.getNumberOfCoutours(numCoutours))return false;
	if(!glyfData.getXMin(xMin)||!glyfData.getYMin(yMin))return false;
	if(!glyfData.getXMax(xMax)||!glyfData.getYMax(yMax))return false;
	//printf("numContours==%d min(x,y)=(%d,%d) max(x,y)=(%d,%d)\n",numCoutours,xMin,yMin,xMax,yMax);

	//获取标志数量(也是实际坐标数量)
	uint16 flagAmount;
	if(!glyfData.flagAmount(flagAmount))return false;
	//printf("glyf.flagAmount==%d\n",flagAmount);
	if(!flagAmount)return false;

	glyfFlags=glyfData.flags(true);
	glyfXcoordinate=glyfData.xCoordinates(glyfFlags,true);
	glyfYcoordinate=glyfData.yCoordinates(glyfFlags,true);

	//缩放坐标
	uint8 flag;int32 x,y;
	xyCoordinate.clear();
	xyCoordinate.resize(flagAmount);
	flags.clear();
	flags.resize(flagAmount);
	for(int i=0;i<flagAmount;++i){
		if(glyfFlags.get_uint8(i,flag) && glyfXcoordinate.get_int32(i*4,x) && glyfYcoordinate.get_int32(i*4,y)){
			flags[i]=flag;
			xyCoordinate[i].x()=x*(int)charPixmap.getWidth()/unitsPerEm;//处理字符宽度问题
			xyCoordinate[i].y()=y*(int)charPixmap.getHeight()/unitsPerEm;
		}
	}
	//坐标集合轮廓
	outlineBox();
	//printf("BoxMin(%d,%d) BoxMax(%d,%d)\n",boxMin.x(),boxMin.y(),boxMax.x(),boxMax.y());
	//printf("pixels: %d*%d\n",boxMax.x()-boxMin.x(),boxMax.y()-boxMin.y());
	//某些字体的坐标可能会超出范围,那么我们需要自动调整
	if(boxMin.y()<0||boxMax.y()>=unitsPerEm){
		auto mid=(boxMin.y()+boxMax.y())/2;
		auto delta=charPixmap.getHeight()/2-mid;
		for(size_t i=0;i<flagAmount;++i){
			xyCoordinate[i].y()+=delta;
		}
	}
	//确定每个轮廓的end指针,并尝试绘制
	charPixmap.fill(0);//清空
	auto endPtrBlock=glyfData.endPtsOfContours();
	int16 startPtr=0,endPtr=0;
	for(auto i=0;i<numCoutours;++i){
		if(endPtrBlock.get_int16(i*2,endPtr)){
			DataBlock::reverseByte(endPtr);
			drawGlyph(startPtr,endPtr);
			//下一个
			startPtr=endPtr+1;
		}
	}
	//fillGlyphs();
	
	//内存回收
	glyfFlags.deleteDataPointer();
	glyfXcoordinate.deleteDataPointer();
	glyfYcoordinate.deleteDataPointer();
	return true;
}
