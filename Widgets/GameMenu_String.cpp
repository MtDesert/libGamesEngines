#include"GameMenu_String.h"
#include"extern.h"

GameMenu_String::GameMenu_String(){}
GameMenu_String::~GameMenu_String(){}

void GameMenu_String::addString(const string &str){
	gameStringList.push_back(GameString());
	auto gString=gameStringList.data(gameStringList.size()-1);
	gString->setString(str);
}

uint GameMenu_String::rowAmount()const{return gameStringList.size();}
void GameMenu_String::renderX()const{
	//绘制每一项内容
	decltype(renderItemStart) i=0,j=0;
	for(auto &gString:gameStringList){
		if(i>=renderItemStart){
			//根据选择状态调整颜色
			if(selectingItemIndex==i){
				shapeRenderer.hasFill=true;
				shapeRenderer.fillColor=0xFFFFFFFF;
				//计算矩形选择区域
				rect=gString.rectF();
				rect.p0.y()+=gString.position.y();
				rect.p1.y()+=gString.position.y();
				shapeRenderer.drawRectangle(rect);
			}
			//开始绘制文字
			gString.render();
			++j;
		}
		if(j>=renderItemAmount)break;
		++i;//下一个
	}
	//绘制菜单边框
	renderRectBorder();
}
void GameMenu_String::updateRenderParameters(){
	GameMenu::updateRenderParameters();
	//调整渲染状态
	point2D=rectF().topCenter();
	decltype(renderItemStart) i=0,j=0;
	for(auto &gString:gameStringList){
		if(i>=renderItemStart){
			gString.position.y() = point2D.y() - itemHeight*j - itemHeight/2;
			//根据选择状态调整颜色
			gString.color=(selectingItemIndex==i ? 0xFF000000 : 0xFFFFFFFF);
			++j;
		}
		if(j>=renderItemAmount)break;
		++i;//下一个
	}
}