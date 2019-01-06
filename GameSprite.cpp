#include"GameSprite.h"
#include"extern.h"

GameSprite::GameSprite():color(0xFFFFFFFF),borderColor(0),anchorPoint(0.5,0.5){}
GameSprite::~GameSprite(){}

void GameSprite::render()const{
	//绘制纹理
	ShapeRenderer::setColor(color);
	rect=rectF();
	texture.draw(rect);
	//绘制边框
	if(borderColor){
		shapeRenderer.hasEdge=true;
		shapeRenderer.hasFill=false;
		shapeRenderer.edgeColor=borderColor;
		shapeRenderer.drawRectangle(rect);
	}
	GameObject::render();
}

Point2D<float> GameSprite::posF()const{
	point2D.x()=position.x();
	point2D.y()=position.y();
	return point2D;
}
Point2D<float> GameSprite::sizeF()const{return texture.sizeF();}
Rectangle<float> GameSprite::rectF()const{
	size2D=sizeF();
	rect.p0.x()=position.x()-anchorPoint.x()*size2D.x();
	rect.p0.y()=position.y()-anchorPoint.y()*size2D.y();
	rect.p1 = rect.p0 + size2D;
	return rect;
}
