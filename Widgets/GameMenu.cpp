#include"GameMenu.h"
#include"extern.h"

//构造函数
GameMenu::GameMenu():
	renderItemStart(0),renderItemAmount(4),itemWidth(0),itemHeight(32),
	selectingItemIndex(0),recycleMode(true),
	onConfirm(nullptr),onCancel(nullptr){}
GameMenu::~GameMenu(){}

void GameMenu::updateRenderParameters(){
	if(selectingItemIndex<renderItemStart){
		renderItemStart=selectingItemIndex;
	}else{
		auto renderEnd = renderItemStart + renderItemAmount;
		auto amount=rowAmount();
		if(renderEnd>amount)renderEnd=amount;
		if(selectingItemIndex>=renderEnd){
			renderItemStart=selectingItemIndex - renderItemAmount + 1;
		}
	}
}
//菜单项数
uint GameMenu::rowAmount()const{return subObjects.size();}

//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
void GameMenu::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	if(key==Keyboard::Key_Down && !pressed){
		++selectingItemIndex;
		if(selectingItemIndex>=rowAmount()){
			selectingItemIndex = recycleMode ? 0 : selectingItemIndex-1;
		}
		updateRenderParameters();
	}else if(key==Keyboard::Key_Up && !pressed){
		if(selectingItemIndex>0){
			--selectingItemIndex;
		}else{
			selectingItemIndex = recycleMode ? rowAmount()-1 : 0;
		}
		updateRenderParameters();
	}else if(key==Keyboard::Key_Enter && !pressed){
		if(onConfirm)onConfirm();
	}else if(key==Keyboard::Key_Esc && !pressed){
		if(onCancel)onCancel();
	}
}

Point2D<float> GameMenu::sizeF()const{//菜单尺寸,根据项尺寸和显示项数决定(子类可能会多出边缘或者间距等部分)
	size2D.x()=itemWidth;
	size2D.y()=itemHeight*renderItemAmount;
	return size2D;
}