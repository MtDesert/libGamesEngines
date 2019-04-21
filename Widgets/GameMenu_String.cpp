#include"GameMenu_String.h"
#include"extern.h"

GameMenu_String::GameMenu_String(){}
GameMenu_String::~GameMenu_String(){}

void GameMenu_String::addString(const string &str){
	DataBlock block;
	GameString::charset.newString(str.data(),block);
	stringCodeList.push_back(block);
}

uint GameMenu_String::rowAmount()const{return stringCodeList.size();}
void GameMenu_String::renderX()const{
	//绘制每一项内容
	point2D=rect.topCenter();
	gameString.position.x()=point2D.x();
	decltype(renderItemStart) i=0,j=0;
	for(auto &stringCode:stringCodeList){
		if(i>=renderItemStart){
			gameString.position.y() = point2D.y() - itemHeight*j - itemHeight/2;
			gameString.stringCode=stringCode;
			//根据选择状态调整颜色
			if(selectingItemIndex==i){
				shapeRenderer.hasFill=true;
				shapeRenderer.fillColor=0xFFFFFFFF;
				gameString.color=0xFF000000;
				//计算矩形选择区域
				rect=gameString.rectF();
				rect.p0.y()+=gameString.position.y();
				rect.p1.y()+=gameString.position.y();
				shapeRenderer.drawRectangle(rect);
			}else{
				gameString.color=0xFFFFFFFF;
			}
			//开始绘制文字
			gameString.render();
			++j;
		}
		if(j>=renderItemAmount)break;
		++i;//下一个
	}
	//绘制菜单边框
	renderRectBorder();
}