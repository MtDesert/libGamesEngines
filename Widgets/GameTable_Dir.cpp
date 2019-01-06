#include"GameTable_Dir.h"
#include"extern.h"

GameTable_Dir::GameTable_Dir(){
	//必须先创建缓冲区
	uint amount=12;

	//设定数据源
	renderItemAmount=amount;
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
		case 0:return 400;
		case 1:return 120;
		case 2:return 80;
		default:return 0;
	}
}

void GameTable_Dir::updateBuffer(){}