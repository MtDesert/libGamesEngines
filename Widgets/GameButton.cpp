#include"GameButton.h"
#include"Game.h"
#include"extern.h"

GameButton::GameButton():buttonSize(160,32),isPressed(false),bgColor(0xFF000000){}
GameButton::~GameButton(){}

void GameButton::mouseMove(int x,int y){
	rect=rectF();
	auto &pos(Game::currentGame()->mousePos);
	if(rect.containPoint(pos.x(),pos.y())){
	}else{
		isPressed=false;
	}
}
void GameButton::mouseKey(MouseKey key,bool pressed){
	//检查点击的位置
	rect=rectF();
	auto &pos(Game::currentGame()->mousePos);
	if(rect.containPoint(pos.x(),pos.y())){
		bool changed=(isPressed!=pressed);
		isPressed=pressed;
		if(changed && !isPressed){
			printf("clicked!\n");
		}
	}
}
void GameButton::render()const{
	rect=rectF();
	//绘制按钮纹理
	shapeRenderer.hasFill=true;
	shapeRenderer.fillColor=(isPressed ? color : bgColor);
	shapeRenderer.drawRectangle(rect);
	//绘制按钮边框
	shapeRenderer.hasFill=false;
	shapeRenderer.drawRectangle(rect);
	//递归绘制
	GameSprite::render();
}
Point2D<float> GameButton::sizeF()const{
	size2D=texture.sizeF();
	if(size2D.x()==0 || size2D.y()==0){//无精灵的时候可以使用自定义大小
		size2D.x()=buttonSize.x();
		size2D.y()=buttonSize.y();
	}
	return size2D;
}