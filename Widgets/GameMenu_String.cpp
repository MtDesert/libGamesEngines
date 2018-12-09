#include"GameMenu_String.h"
#include"ShapeRenderer.h"

/*static ShapeRenderer sr;//绘制边框用
static Rectangle<float> rect,rct;//绘制矩形
static GameString gameString;

//GameMenu_String
GameMenu_String::GameMenu_String(){setSourceList(stringCodeList);}
GameMenu_String::~GameMenu_String(){clearAllItems();}

static const int GameMenu_String_arrowHeight=16;

void GameMenu_String::insertItem(const string &text){
	mGameString.setString(text);//存成unicode
	stringCodeList.push_back(mGameString.stringCode);//保存内容
	mGameString.stringCode=DataBlock();//清空
}
void GameMenu_String::clearAllItems(){
	for(auto &code:stringCodeList){
		code.deleteDataPointer();
	}
	stringCodeList.clear();
}
void GameMenu_String::refresh(){
	gotoBegin();
	decltype(itemWidth) width;
	for(auto &unicode:stringCodeList){//计算每个文本的长度并找出最大长度
		mGameString.stringCode=unicode;
		width=mGameString.stringWidth();
		mGameString.stringCode=DataBlock();//清空
		if(width>itemWidth){
			itemWidth=width;
		}
	}
	itemHeight=mGameString.charSize.y();
}

Point2D<GLfloat> GameMenu_String::sizeF()const{
	auto size=GameMenu::sizeF();
	//size.y()+=GameMenu_String_arrowHeight*2;
	return size;
}
void GameMenu_String::render()const{
	gameString=mGameString;
	//画个矩形
	GameMenu::render();
	//画箭头
	auto itemAmount=rowAmount();
	if(itemAmount){
		Triangle2D<GLfloat> tri;
		tri.p0.data[0]=text.position.x();
		tri.p1.data[0]=text.position.x()-10;
		tri.p2.data[0]=text.position.x()+10;
		if(renderItemFrom>0){//需要画上箭头
			tri.p0.data[1]=rect.p1.y();
			tri.p1.data[1]=tri.p2.data[1]= rect.p1.y()-GameMenu_String_arrowHeight;
			sr.drawTriangle(tri);
		}
		if(renderTo<itemAmount){//需要画下箭头
			tri.p0.data[1]=rect.p0.y();
			tri.p1.data[1]=tri.p2.data[1]= rect.p0.y()+GameMenu_String_arrowHeight;
			sr.drawTriangle(tri);
		}
	}
	text.stringCode=DataBlock();//清空
}
void GameMenu_String::renderItem(const List<DataBlock>::iterator &itr,const Rectangle<float> &rect)const{
	auto center=rect.center();
	gameString.position.x()=center.x();
	gameString.position.y()=center.y();
	gameString.stringCode=*itr;
	if(itr==itrSelecting){
		gameString.color=0xFF000000;
		sr.fillColor=color;//当前底
		sr.drawRectangle(gameString.rectF());
	}else{
		gameString.color=0xFFFFFFFF;
	}
	gameString.render();
	gameString.stringCode=DataBlock();
}*/
