#include"GameTable.h"
#include"extern.h"

GameTable::GameTable():page(0),maxPage(0){}
GameTable::~GameTable(){}

//函数
uint GameTable::columnAmount()const{return 0;}
uint GameTable::columnWidth(uint column)const{return 0;}
void GameTable::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	GameMenu::keyboardKey(key,pressed);
	if(key==Keyboard::Key_Left && !pressed){}
	else if(key==Keyboard::Key_Right && !pressed){}
}
//渲染
void GameTable::render()const{
	GameMenu::render();
	//渲染整个表格内容
	auto colAmount=columnAmount();
	for(uint y=0;y<renderItemAmount;++y){
		for(uint x=0;x<colAmount;++x){
			//渲染内容
			renderItem(x,y);
		}
	}
}
Point2D<float> GameTable::sizeF()const{return point2D;}
void GameTable::renderItem(uint x,uint y)const{}