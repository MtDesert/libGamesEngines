#include"GameTable_Dir.h"
#include"extern.h"

enum ColumnType{
	Filename,FileSize,AmountOfColumnType
};

struct FileInfo{//文件信息缓冲
};

GameTable_Dir::GameTable_Dir(){
	//设定数据源
	renderItemAmount=10;
	itemHeight=32;
}
GameTable_Dir::~GameTable_Dir(){}

bool GameTable_Dir::changeDir(const string &dirName){
	bool b=directory.changeDir(dirName);
	if(b){
		directory.readDir();
		directory.direntList.sortBy(DirentList::ByName);
		updateBuffer();
	}
	return b;
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
	GameTable::keyboardKey(key,pressed);
}

void GameTable_Dir::renderItem(uint x,uint y)const{}
void GameTable_Dir::updateBuffer(){}