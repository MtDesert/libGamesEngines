#include"GameTable_Dir.h"
#include"GameString.h"
#include"ShapeRenderer.h"
#include"Charset.h"

//绘制变量
/*static ShapeRenderer sr;//绘制边框用
static Rectangle<float> rect;//绘制矩形
static GameString gameString;

const char* headerNames[]={"文件名","大小","日期"};

GameTable_Dir::GameTable_Dir(){
	//static
	gameString=GameString();
	//必须先创建缓冲区
	uint amount=12;
	itemBuffers=new ItemBuffer[amount];

	//设定数据源
	renderItemAmount=amount;
	setSourceList(directory.direntList);
	itemHeight=32;
	//准备要显示的表头字符串
	headerHeight=32;
	setHeadersStrings(headerNames);
}
GameTable_Dir::~GameTable_Dir(){
	delete []itemBuffers;
}

bool GameTable_Dir::changeDir(const string &dirName){
	bool b=directory.changeDir(dirName);
	if(b){
		directory.readDir();
		directory.direntList.sortBy(DirentList::ByName);
		gotoBegin();
		updateBuffer();
	}
	return b;
}
uint GameTable_Dir::columnAmount()const{return 3;}
uint GameTable_Dir::columnWidth(uint col)const{
	switch(col){
		case 0:return 376;
		case 1:return 120;
		case 2:return 19*16;
		default:return 0;
	}
}
void GameTable_Dir::renderItem(const typename List<DirectoryEntry>::iterator &itr,uint row,uint column,const Rectangle<float> &itemRect)const{
	bool isSelecting=(itr==itrSelecting);
	//画边框
	sr.hasFill=isSelecting;
	sr.drawRectangle(itemRect);
	//画文字
	auto center=itemRect.center();
	gameString.position.x()=center.x();
	gameString.position.y()=center.y();
	gameString.color=isSelecting?0xFF000000:0xFFFFFFFF;
	auto &item=itemBuffers[row];
	switch(column){
		case 0:gameString.stringCode=item.name;break;
		case 1:gameString.stringCode=item.size;break;
		case 2:gameString.stringCode=item.date;break;
		break;
	}
	gameString.render();
	gameString.stringCode=DataBlock();
}
void GameTable_Dir::renderHeader(uint col,const Rectangle<float> &itemRect)const{
	//画边框
	sr.hasFill=false;
	sr.drawRectangle(itemRect);
	//画表头
	auto center=itemRect.center();
	gameString.position.x()=center.x();
	gameString.position.y()=center.y();
	gameString.color=0xFFFFFFFF;
	gameString.stringCode=stringCode_Headers[col];
	gameString.render();
	gameString.stringCode=DataBlock();
}

void GameTable_Dir::updateBuffer(){
	auto itr=itrBegin;
	for(size_t i=0;i<renderItemAmount;++i){
		bool b=itr!=itrEnd;//b==是否有数据
		auto &item=itemBuffers[i];
		item.name=GameString::newString(b?(*itr).name():"");//名字
		item.size=GameString::newString(b?(*itr).strSize():"");//大小
		item.date=GameString::newString(b?(*itr).strModifyDate():"");//修改日期
		if(b)++itr;
	}
}*/