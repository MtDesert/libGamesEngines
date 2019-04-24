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
static decltype(rect) rct;
static uint colAmount,x,y;
void GameTable::renderX()const{
	//渲染整个表格内容
	colAmount=columnAmount();
	for(y=0;y<renderItemAmount;++y){
		//调整渲染参数
		rct.p0.x()=rect.p0.x();
		rct.p0.y()=rect.p1.y()-(y+1)*itemHeight;
		rct.p1.y()=rct.p0.y()+itemHeight;
		for(x=0;x<colAmount;++x){
			rct.p1.x() = rct.p0.x() + columnWidth(x);//调整渲染参数
			//渲染内容
			shapeRenderer.drawRectangle(rct);
			renderItem(x,y,rct);//渲染子类内容
			rct.p0.x() = rct.p1.x();//调整渲染参数
		}
	}
}
Point2D<float> GameTable::sizeF()const{
	colAmount=columnAmount();
	size2D.x()=0;
	for(x=0;x<colAmount;++x){size2D.x()+=columnWidth(x);}
	size2D.y()=itemHeight*renderItemAmount;
	return size2D;
}
void GameTable::renderItem(uint x,uint y,const Rectangle2D<float> &rectArea)const{}