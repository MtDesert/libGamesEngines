#include"Game.h"
#include"GameScene.h"
#include"GameString.h"

#include"errno.h"

//全局变量
string errorString;//错误描述,如果游戏运行过程中出现各种错误,都可以存到此变量中

Game::Game():sceneFileList(nullptr){
	//加载字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.loadFile("fonts/ascii");
	cache.bitmapFontGb2312.symbolBlock.loadFile("fonts/lv0");
	cache.bitmapFontGb2312.lv1Chinese.loadFile("fonts/lv1");
	cache.bitmapFontGb2312.lv2Chinese.loadFile("fonts/lv2");
}
Game::~Game(){
	//删除控件
	deleteScene_FileList();
	//删除字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.deleteDataPointer();
	cache.bitmapFontGb2312.symbolBlock.deleteDataPointer();
	cache.bitmapFontGb2312.lv1Chinese.deleteDataPointer();
	cache.bitmapFontGb2312.lv2Chinese.deleteDataPointer();
}

//Game* Game::newGame(){return new Game();}

#define STATIC(name)\
decltype(name) name

STATIC(Game::game)=nullptr;
STATIC(Game::resolution);
STATIC(Game::mousePos);

Game* Game::currentGame(){return Game::game;}

bool Game::loadTranslationFile(const string &filename){
	translationMap.clear();
	auto file=fopen(filename.data(),"r");
	if(!file){
		switch(errno){
		}
		return false;
	}
	//开始读取映射
	int bufferSize=256;
	char buffer[bufferSize];
	char *start,*finish;
	while(fgets(buffer,bufferSize,file)){
		//',' -> '\0'
		start=strstr(buffer,",");
		if(!start)continue;
		*start='\0';//',' -> '\0'
		++start;
		//'\n' -> '\0'
		finish=strstr(start,"\n");
		if(finish)*finish='\0';
		//开始插入映射
		translationMap.insert(buffer,start);
	}
	return true;
}
const char* Game::translate(const string &english)const{
	auto value=translationMap.value(english);
	return value ? value->data() : english.data();
}

GameScene_FileList *Game::showScene_FileList(){
	if(!sceneFileList){
		sceneFileList=new GameScene_FileList();
	}
	subObjects.push_front(sceneFileList);
	return sceneFileList;
}
void Game::deleteScene_FileList(){
	if(sceneFileList){
		subObjects.remove(sceneFileList);
		delete sceneFileList;
	}
	sceneFileList=nullptr;
}

void Game::mouseMove(int x,int y){
	mousePos.x()=x;
	mousePos.y()=y;
	GameObject::mouseMove(x,y);
}
void Game::addTimeSlice(uint usec){
	auto scene=findFirstGameScene();
	if(scene)scene->addTimeSlice(usec);
}
void Game::render()const{
	auto scene=findFirstGameScene();
	if(scene)scene->render();
}

GameScene* Game::findFirstGameScene()const{
	GameScene *scene=nullptr;
	for(auto obj:subObjects){
		scene=dynamic_cast<GameScene*>(obj);
		if(scene)break;
	}
	return scene;
}