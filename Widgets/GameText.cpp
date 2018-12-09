#include"GameText.h"
#include"ShapeRenderer.h"
#include"Charset.h"

static GameString gameString;

GameText::GameText():lineCharAmount(16){
	gameString.anchorPoint=Point2D<float>(0,1);
}
GameText::GameText(const string &text):GameString(text){}
GameText::~GameText(){}

uint GameText::lineAmount()const{return lineStart.size();}

void GameText::setString(const string &str){
	GameString::setString(str);
	refresh();
}
void GameText::refresh(){
	//在这里立刻计算行数,并将行信息存入数组
	lineStart.clear();
	uint from=0,count=0;
	uint8 u8,len=0;uint16 u16;
	for(uint i=0;i<stringCode.dataLength;i+=len){
		if(stringCode.get_uint8(i,u8) && u8<0x80){//ASCII
			if(u8==0x00)break;//字符串结束
			len=1;
		}else if(stringCode.get_uint16(i,u16)){
			len=2;
		}
		count+=len;
		//这里判断是否需要换行
		if(count>lineCharAmount){//超过行上限,要换
			lineStart.push_back(from);//保存起来
			from=i;//确定新的起点
			count=len;//确定新的长度
		}
	}
	//保存最后一行,即使没达到上限
	lineStart.push_back(from);
}

Point2D<GLfloat> GameText::sizeF()const{return Point2D<GLfloat>(charSize.x()*lineCharAmount,charSize.y()*lineStart.size());}
void GameText::render()const{
	ShapeRenderer::setColor(color);
	auto rect=rectF();
	gameString.position.x()=rect.p0.x();
	gameString.position.y()=rect.p1.y();
	//开始计算
	uint renderTo=0;
	for(auto itr=lineStart.begin();itr!=lineStart.end();++itr){
		auto itrNext=itr;++itrNext;
		renderTo=(itrNext!=lineStart.end()?*itrNext:stringCode.dataLength);
		gameString.stringCode.dataPointer=&stringCode.dataPointer[*itr];
		gameString.stringCode.dataLength=renderTo-(*itr);
		gameString.render();
		gameString.position.y()-=charSize.y();
	}
	gameString.stringCode=DataBlock();
}
