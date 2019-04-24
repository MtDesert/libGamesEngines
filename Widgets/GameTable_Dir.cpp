#include"GameTable_Dir.h"
#include"extern.h"

enum ColumnType{
	Filename,FileSize,AmountOfColumnType
};

GameTable_Dir::GameTable_Dir(){
	//设定数据源
	renderItemAmount=10;
	itemHeight=32;
	entryBuffer=new DirectoryEntry[renderItemAmount];//创建缓冲
}
GameTable_Dir::~GameTable_Dir(){
	delete []entryBuffer;//删除缓冲
}

bool GameTable_Dir::changeDir(const string &dirName){
	bool b=directory.changeDir(dirName);
	if(b){
		directory.readDir();
		directory.direntList.sortBy(DirentList::ByName);
		renderItemStart=selectingItemIndex=0;
		updateBuffer();
	}
	return b;
}

void GameTable_Dir::renderItem(uint x,uint y,const Rectangle2D<float> &rectArea)const{
	switch(x){
		case FileSize:break;
	}
}

uint GameTable_Dir::columnAmount()const{return 3;}
uint GameTable_Dir::columnWidth(uint col)const{
	switch(col){
		case Filename:return 400;
		case FileSize:return 120;
		default:return 0;
	}
}
void GameTable_Dir::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	auto old=renderItemStart;
	GameTable::keyboardKey(key,pressed);
	//改变状态,这会影响到渲染过程
	if(old!=renderItemStart){
		updateBuffer();
	}
}

void GameTable_Dir::updateBuffer(){
	//更新迭代器
	auto itrStart=directory.direntList.begin();
	for(decltype(renderItemStart) i=0;i<renderItemStart;++i){
		++itrStart;
	}
	//更新缓冲
	for(decltype(renderItemAmount) i=0;i<renderItemAmount;++i){
		entryBuffer[i] = *itrStart;
		++itrStart;
	}
}