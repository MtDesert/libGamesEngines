#include"GameDialog.h"
#include"Game.h"
#include"extern.h"

const int border=16;
uint GameDialog::maxLineCharAmount = 36;

GameDialog::GameDialog(){
	subObjects.push_back(&mGameText);
	subObjects.push_back(&mGameButton);
	mGameButton.setString(Game::currentGame()->translate("Confirm"));
}
GameDialog::~GameDialog(){}

void GameDialog::renderX()const{
	//画底板
	shapeRenderer.hasEdge=true;
	shapeRenderer.edgeColor=0xFFFFFFFF;
	shapeRenderer.hasFill=true;
	shapeRenderer.fillColor=0xFF000000;
	shapeRenderer.drawRectangle(rectF());
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
	mGameText.updateRenderParameter();
	//设置mGameText的几何位置
	auto rct=rectF();
	mGameText.position.y()=rct.p1.y() - border - mGameText.sizeF().y()/2;
	//设置mGameButton的几何位置
	mGameButton.position.y()=rct.p0.y() + border + mGameButton.sizeF().y()/2;
}