#include"GameSprite.h"
#include"extern.h"

GameSprite::GameSprite():color(0xFFFFFFFF),
	rotateAngle(0),
	scale(1,1,1),
	anchorPoint(0.5,0.5){}
GameSprite::~GameSprite(){}

void GameSprite::render()const{
	//绘制纹理
	glPushMatrix();//保存矩阵
	//变换
	glTranslated(position.x(),position.y(),position.z());
	glRotated(rotateAngle,rotation.x(),rotation.y(),position.z());
	glScaled(scale.x(),scale.y(),scale.z());
	//绘制纹理
	ShapeRenderer::setColor(color);
	texture.draw(rectF());
	//其它绘制
	renderX();//特殊绘制
	GameObject::render();//递归绘制子节点
	glPopMatrix();//恢复矩阵
}
void GameSprite::renderX()const{}

void GameSprite::renderRectBorder()const{
	shapeRenderer.hasEdge=true;
	shapeRenderer.hasFill=false;
	shapeRenderer.edgeColor=color;
	shapeRenderer.drawRectangle(rectF());
}

Point2D<float> GameSprite::posF()const{
	point2D.x()=position.x();
	point2D.y()=position.y();
	return point2D;
}
Point2D<float> GameSprite::sizeF()const{return texture.sizeF();}
Rectangle2D<float> GameSprite::rectF()const{
	size2D=sizeF();
	rect.p0.x()=-anchorPoint.x()*size2D.x();
	rect.p0.y()=-anchorPoint.y()*size2D.y();
	rect.p1 = rect.p0 + size2D;
	return rect;
}
