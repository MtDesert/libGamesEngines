#include"GameMenu.h"
#include"extern.h"

//构造函数
GameMenu::GameMenu():renderItemAmount(4),itemWidth(0),itemHeight(32),selectingItemIndex(0),menuStatus(Selecting){}
GameMenu::~GameMenu(){}

//菜单项数
uint GameMenu::rowAmount()const{return subObjects.size();}

Point2D<float> GameMenu::sizeF()const{//菜单尺寸,根据项尺寸和显示项数决定(子类可能会多出边缘或者间距等部分)
	return Point2D<float>(itemWidth,itemHeight*renderItemAmount);
}

//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
void GameMenu::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	if(key==Keyboard::Key_Down && !pressed){
		++selectingItemIndex;
	}else if(key==Keyboard::Key_Up && !pressed){
		--selectingItemIndex;
	}else if(key==Keyboard::Key_Enter && !pressed){
		menuStatus=Confirm;//确认选择项
	}else if(key==Keyboard::Key_Esc && !pressed){
		menuStatus=Cancel;//取消选择
	}
}
//渲染
void GameMenu::render()const{
	//画个矩形
	rect=rectF();
	shapeRenderer.fillColor=0xFF000000;
	shapeRenderer.hasFill=false;
	shapeRenderer.drawRectangle(rect);
}