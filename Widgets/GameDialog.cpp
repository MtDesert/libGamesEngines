#include"GameDialog.h"
#include"ShapeRenderer.h"

static ShapeRenderer sr;
const int border=16;
uint GameDialog::maxLineCharAmount = 40;

GameDialog::GameDialog(){
	subObjects.push_back(&mGameText);
	subObjects.push_back(&mGameButton);
}
GameDialog::~GameDialog(){}

void GameDialog::render()const{
	//画底板
	sr.hasEdge=true;
	sr.edgeColor=0xFFFFFFFF;
	sr.hasFill=true;
	sr.fillColor=0xFF000000;
	sr.drawRectangle(rectF());
	GameSprite::render();
}

Point2D<float> GameDialog::sizeF()const{
	Point2D<float> ret;
	auto textRect=mGameText.rectF();
	ret.x()=textRect.width() + border*2;
	ret.y()=textRect.height() + mGameButton.sizeF().y()+border*3;
	return ret;
}

void GameDialog::setText(const string &text){
	mGameText.setString(text);
	//调整mGameText的行字数
	auto strWidth = mGameText.stringWidth();
	if(strWidth > maxLineCharAmount * mGameText.charSize.x()){
		mGameText.lineCharAmount = maxLineCharAmount;
	}else{
		mGameText.lineCharAmount = strWidth / mGameText.charSize.x();
	}
	mGameText.refresh();
	//设置mGameText的几何位置
	auto rct=rectF();
	auto center=rct.center();
	mGameText.anchorPoint=Point2D<float>(0.5,0.5);
	mGameText.position.x()=center.x();
	mGameText.position.y()=rct.p1.y() - border - mGameText.sizeF().y()/2;
	//设置mGameButton的几何位置
	mGameButton.anchorPoint=Point2D<float>(0.5,0.5);
	mGameButton.position.x()=center.x();
	mGameButton.position.y()=rct.p0.y() + border + mGameButton.sizeF().y()/2;
}
