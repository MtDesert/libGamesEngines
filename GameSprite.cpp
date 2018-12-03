#include"GameSprite.h"
#include"ShapeRenderer.h"

static Point2D<float> sz;
static Rectangle<float> rect;

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
	sz=sizeF();
	rect.p0.x()=position.x()-anchorPoint.x()*sz.x();
	rect.p0.y()=position.y()-anchorPoint.y()*sz.y();
	rect.p1 = rect.p0 + sz;
	return rect;
}
