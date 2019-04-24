#include"GameText.h"
#include"Charset.h"
#include"extern.h"

GameText::GameText():lineCharAmount(16){}
GameText::GameText(const string &text):GameString(text){}
GameText::~GameText(){}

uint GameText::lineAmount()const{return lineStart.size();}

void GameText::setString(const string &str){
	GameString::setString(str);
	updateRenderParameter();
}
void GameText::updateRenderParameter(){
	//在这里立刻计算行数,并将行信息存入数组
	lineStart.clear();
	uint from=0,count=0,charLen=0;
	auto len=arrayCharAttr.size();
	for(uint i=0;i<len;++i){
		charLen = arrayCharAttr.data(i)->isAscii ? 1 : 2;
		count += charLen;
		//这里判断是否需要换行
		if(count>lineCharAmount){//超过行上限,要换
			lineStart.push_back(from);//保存起来
			from=i;//确定新的起点
			count=charLen;//确定新的长度
		}
	}
	//保存最后一行,即使没达到上限
	lineStart.push_back(from);
}

Point2D<GLfloat> GameText::sizeF()const{
	return Point2D<GLfloat>(charSize.x()*lineCharAmount,charSize.y()*lineAmount());
}
void GameText::renderX()const{
	ShapeRenderer::setColor(color);
	shapeRenderer.setColor(color);
	rect=rectF();
	point2D=rect.p0;
	point2D.y()=rect.p1.y()-charSize.y();
	size2D.y()=charSize.y();
	//开始计算
	uint from=0,to=0;
	for(auto itr=lineStart.begin();itr!=lineStart.end();++itr){
		//获取渲染范围
		auto itrNext=itr;++itrNext;
		from=*itr;
		to=(itrNext!=lineStart.end()?*itrNext:arrayCharAttr.size());
		GameString::renderX(from,to-from);
		//下一个点
		point2D.x()=rect.p0.x();
		point2D.y()-=charSize.y();
	}
}