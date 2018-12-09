#ifndef GAMETABLE_H
#define GAMETABLE_H

#include"GameMenu.h"
#include"GameString.h"

/*游戏中常用的表,表的作用是用来给玩家查询一些数据
本类只负责简单画上表格线,以及一些文字数据,不负责存储数据
*/
template<typename T>
class GameTable:public GameMenu<T>{
protected:
	//表头
	DataBlock *stringCode_Headers;//这是表头,可以用来显示标题
	//清除表头
	void clearHeadersStrings(){
		if(stringCode_Headers){
			auto colAmount=columnAmount();
			for(decltype(colAmount) i=0;i<colAmount;++i){
				stringCode_Headers[i].memoryFree();//删除每条数据
			}
			delete []stringCode_Headers;
			stringCode_Headers=nullptr;
		}
	}
	//设置新的表头
	void setHeadersStrings(const char* headerNames[]){
		clearHeadersStrings();//移除旧数据
		auto colAmount=columnAmount();
		stringCode_Headers=new DataBlock[colAmount];
		for(decltype(colAmount) i=0;i<colAmount;++i){
			stringCode_Headers[i]=GameString::newString(headerNames[i]);
		}
	}
	//渲染
	virtual void renderItem(const typename List<T>::iterator &itr,uint row,uint column,const Rectangle<float> &rect)const{}
	virtual void renderItem(const typename List<T>::iterator &itr,uint row,const Rectangle<float> &rect)const{
		auto colAmount=columnAmount();
		auto rct=rect;
		rct.p1.x()=rct.p0.x();
		for(uint col=0;col<colAmount;++col){
			auto w=columnWidth(col);
			rct.p1.x()+=w;
			renderItem(itr,row,col,rct);
			rct.p0.x()=rct.p1.x();
		}
	}
	virtual void renderHeader(uint col,const Rectangle<float> &itemRect)const{}//渲染表头(列索引,表头的矩形区域)
public:
	//变量
	uint headerHeight;//表头高度,表头的文字通常用来表示列的内容
	uint page,maxPage;//当前页,最大页数(某些游戏数据庞大,一页是显示不完的)

	//构造/析构函数
	GameTable():stringCode_Headers(nullptr),headerHeight(32),page(0),maxPage(0){}
	~GameTable(){
		clearHeadersStrings();
	}

	//函数
	virtual uint columnAmount()const{return 0;}//列数
	virtual uint columnWidth(uint column)const{return 0;}//各个列的宽度,在子类中实现
	virtual void keyboardKey(Keyboard::KeyboardKey key,bool pressed){//添加左右键翻页功能
		GameMenu<T>::keyboardKey(key,pressed);
		if(key==Keyboard::Key_Left && pressed){
			if(page)--page;//减少页数
		}else if(key==Keyboard::Key_Right && pressed){
			++page;//增加页数
			if(page>=maxPage)page=(maxPage?maxPage-1:0);
		}
	}
	virtual Point2D<float> sizeF()const{
		Point2D<float> ret;
		auto colAmount=columnAmount();
		for(uint i=0;i<colAmount;++i){
			ret.x()+=columnWidth(i);
		}
		ret.y()=headerHeight+this->renderItemAmount*this->itemHeight;//动态确定高度
		return ret;
	}
	//渲染
	virtual void render()const{
		auto rect=this->rectF(),rct=rect;
		//画表头
		rct.p0.y()=rect.p1.y()+1-headerHeight;
		rct.p1.x()=rect.p0.x();
		auto colAmount=columnAmount();
		for(uint i=0;i<colAmount;++i){
			rct.p1.x()=rct.p0.x()+columnWidth(i);
			renderHeader(i,rct);//绘制方框和标题
			rct.p0.x()=rct.p1.x();
		}
		rct.p0.x()=rect.p0.x();
		//画表格
		auto itr=this->itrBegin;
		for(uint row=0;row<this->renderItemAmount;++row){
			if(itr==this->itrEnd)break;
			//计算区域并渲染
			rct.p1.y()=rct.p0.y()-1;
			rct.p0.y()=rct.p1.y()-this->itemHeight+1;
			renderItem(itr,row,rct);
			//下一个迭代器
			++itr;
		}
	}
};
#endif
