#include"GameButton.h"
#include"Game.h"
#include"ShapeRenderer.h"

static ShapeRenderer sr;//绘制边框用
static Point2D<float> sz;
static Rectangle<float> rect;
static GameString gameString;//绘制按钮标题

GameButton::GameButton():buttonSize(160,32),charSize(GameString::defaultCharSize),isPressed(false),bgColor(0xFF000000){
	sr.hasFill=false;
}
GameButton::~GameButton(){stringCode.memoryFree();}

void GameButton::setString(const string &str){
	stringCode.memoryFree();//删除掉旧的数据
	stringCode=GameString::newString(str);
}

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
	sr.hasFill=true;
	sr.fillColor=(isPressed ? color : bgColor);
	sr.drawRectangle(rect);
	//绘制文字
	auto center=rect.center();
	gameString.position.x()=center.x();
	gameString.position.y()=center.y();
	gameString.stringCode=stringCode;
	gameString.charSize=charSize;
	gameString.color=(isPressed ? bgColor : color);
	gameString.render();//渲染文字
	gameString.stringCode=DataBlock();//渲染完成后要清空
	//绘制按钮边框
	sr.hasFill=false;
	sr.drawRectangle(rect);
	//递归绘制
	GameObject::render();
}
Point2D<float> GameButton::sizeF()const{
	sz=texture.sizeF();
	if(sz.x()==0 || sz.y()==0){//无精灵的时候可以使用自定义大小
		sz.x()=buttonSize.x();
		sz.y()=buttonSize.y();
	}
	return sz;
}
