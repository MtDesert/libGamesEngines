#include"GameSprite.h"
#include"extern.h"

GameSprite::GameSprite():color(0xFFFFFFFF),anchorPoint(0.5,0.5){}
GameSprite::~GameSprite(){}

void GameSprite::render()const{
	ShapeRenderer::setColor(color);
	texture.draw(rectF());
	GameObject::render();
}
Point2D<float> GameSprite::posF()const{return Point2D<float>(position.x(),position.y());}
Point2D<float> GameSprite::sizeF()const{return texture.sizeF();}
Rectangle<float> GameSprite::rectF()const{
	size2D=sizeF();
	rect.p0.x()=position.x()-anchorPoint.x()*size2D.x();
	rect.p0.y()=position.y()-anchorPoint.y()*size2D.y();
	rect.p1 = rect.p0 + size2D;
	return rect;
}
