#ifndef GAMEMENU_H
#define GAMEMENU_H

#include"GameSprite.h"
#include"List.h"
#include"ShapeRenderer.h"

enum GameMenuStatus{
	Selecting/*待命*/,Confirm/*确定*/,Cancel/*取消*/
};

/*游戏中用到的菜单,可以显示各种数据,具体的显示方法一般在子类实现*/
template<typename T>
class GameMenu:public GameSprite{
protected:
	//变量
	const List<T> *sourceList;//数据源
	typename List<T>::iterator itrBegin,itrSelecting,itrEnd;//开始渲染的项,选中项,停止渲染的项
	uint renderItemAmount;//最多显示多少项,多余的项不会被绘制
	//项渲染,子类差异化
	virtual void renderItem(const typename List<T>::iterator &itr,const Rectangle<float> &rect)const{}
	//更新缓冲,字类差异化
	virtual void updateBuffer(){}
public:
	//变量
	size_t itemWidth,itemHeight;//菜单项的寬高
	//菜单状态
	char menuStatus;//菜单状态,供外部进行检测以便得知用户所做的操作

	//构造函数
	GameMenu():sourceList(nullptr),renderItemAmount(4),itemWidth(0),itemHeight(32),menuStatus(Selecting){}
	~GameMenu(){}

	//设置渲染数
	virtual void setRenderItemAmount(uint amount){
		renderItemAmount=amount;
		//重新调整渲染区域
		itrEnd=itrSelecting=itrBegin;
		auto end = sourceList->end();
		for(uint i=0;i<renderItemAmount;++i){
			if(itrEnd==end)break;
			++itrEnd;
		}
		updateBuffer();
	}
	//菜单选择回到首项
	void gotoBegin(){
		itrBegin=sourceList->begin();
		setRenderItemAmount(renderItemAmount);
	}
	//菜单选择回到末项
	void gotoEnd(){
		itrBegin = itrSelecting = sourceList->itrTail();
		itrEnd = sourceList->end();
		auto begin = sourceList->begin();
		for(uint i=1;i<renderItemAmount;++i){
			if(itrBegin==begin)break;
			--itrBegin;
		}
		updateBuffer();
	}
	//当前选择索引
	int selectingIndex()const{
		int ret=0;
		auto itr=sourceList->begin();
		for(;itr!=sourceList->end();++itr,++ret){
			if(itr==itrSelecting)break;
		}
		return ret;
	}
	//设置菜单数据
	void setSourceList(const List<T> &sourceList){
		this->sourceList = &sourceList;
		gotoBegin();
	}
	//菜单项数
	virtual uint rowAmount()const{return sourceList?sourceList->size():0;}

	virtual Point2D<float> sizeF()const{//菜单尺寸,根据项尺寸和显示项数决定(子类可能会多出边缘或者间距等部分)
		return Point2D<float>(itemWidth,itemHeight*renderItemAmount);
	}

	//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
	virtual void keyboardKey(Keyboard::KeyboardKey key,bool pressed){
		if(!sourceList || sourceList->size()==0)return;//无List不操作
		if(key==Keyboard::Key_Down && !pressed){
			++itrSelecting;
			if(itrSelecting==itrEnd){
				if(itrEnd==sourceList->end()){//已经到底,需要回到首项
					gotoBegin();
				}else{//还可以往下移动
					++itrBegin;++itrEnd;
				}
				updateBuffer();
			}
		}else if(key==Keyboard::Key_Up && !pressed){
			if(itrSelecting==itrBegin){
				if(itrBegin==sourceList->begin()){//已经到顶,需要回到末项
					gotoEnd();
				}else{//还可以往上移动
					--itrSelecting;--itrBegin;
					if(itrEnd==sourceList->end()){
						itrEnd=sourceList->itrTail();
					}else --itrEnd;
				}
				updateBuffer();
			}else{
				--itrSelecting;
			}
		}else if(key==Keyboard::Key_Enter && !pressed){
			menuStatus=Confirm;//确认选择项
		}else if(key==Keyboard::Key_Esc && !pressed){
			menuStatus=Cancel;//取消选择
		}
	}
	//渲染
	virtual void render()const{
		//画个矩形
		auto rect=rectF(),rct=rect;
		rct.p0.y()=rct.p1.y()-itemHeight+1;
		ShapeRenderer sr;
		sr.fillColor=0xFF000000;
		sr.hasFill=false;
		sr.drawRectangle(rect);
		//画表格
		auto itr=itrBegin;
		for(uint row=0;row<renderItemAmount;++row){
			if(itr==itrEnd)break;
			//计算区域并渲染
			renderItem(itr,rct);
			rct.p1.y()=rct.p0.y()-1;
			rct.p0.y()=rct.p1.y()-itemHeight+1;
			//下一个迭代器
			++itr;
		}
	}
};
#endif
