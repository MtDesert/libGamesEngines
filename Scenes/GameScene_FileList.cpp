#include "GameScene_FileList.h"
#include"Game.h"
#include"extern.h"

GameScene_FileList::GameScene_FileList(){
	auto &res=Game::resolution;
	//标题,顶部居中
	textTitle.position=res;
	textTitle.position.x()/=2;
	textTitle.anchorPoint.y()=1;
	subObjects.push_back(&textTitle);
	//路径
	textPath.position = textTitle.position;
	textPath.position.y() -= textTitle.charSize.y();
	textPath.anchorPoint.y()=1;
	textPath.setString(".");
	subObjects.push_back(&textPath);
	//文件列表
	gameTableDir.position = textPath.position;
	gameTableDir.position.y() -= textPath.charSize.y();
	gameTableDir.anchorPoint.y()=1;
	subObjects.push_back(&gameTableDir);

	//按钮
	auto game=Game::currentGame();
	const char* names[]={"Confirm","File","Directory","Cancel"};
	GameButton_String *buttons[]={&buttonConfirm,&buttonFile,&buttonDir,&buttonCancel};
	auto btnWidth=res.x()/4;
	for(auto i=0;i<4;++i){
		auto btn=buttons[i];
		btn->setString(game->translate(names[i]));
		btn->position.x() = btnWidth*i + btnWidth/2;
		btn->position.y() = btn->buttonSize.y()/2;
		subObjects.push_back(btn);
	}
}

bool GameScene_FileList::changeDirectory(const string &dirName){
	if(directory.changeDir(dirName,true)){
		for(auto &dirEntry:directory.direntList){
			printf("%s\n",dirEntry.name().data());
		}
	}
	return false;
}