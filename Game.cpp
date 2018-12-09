#include"Game.h"
#include"GameScene.h"

Game::Game(){
	//加载字体
	fontTextureCache.bitmapFontAscii.charBlock.loadFile("fonts/ascii");
	fontTextureCache.bitmapFontGb2312.symbolBlock.loadFile("fonts/lv0");
	fontTextureCache.bitmapFontGb2312.lv1Chinese.loadFile("fonts/lv1");
	fontTextureCache.bitmapFontGb2312.lv2Chinese.loadFile("fonts/lv2");
}
Game::~Game(){
	//删除自体
	fontTextureCache.bitmapFontAscii.charBlock.deleteDataPointer();
	fontTextureCache.bitmapFontGb2312.symbolBlock.deleteDataPointer();
	fontTextureCache.bitmapFontGb2312.lv1Chinese.deleteDataPointer();
	fontTextureCache.bitmapFontGb2312.lv2Chinese.deleteDataPointer();
}

//Game* Game::newGame(){return new Game();}

#define STATIC(name)\
decltype(name) name

STATIC(Game::game)=nullptr;
STATIC(Game::resolution);
STATIC(Game::mousePos);

Game* Game::currentGame(){return Game::game;}
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
	for(GameObject *obj:subObjects){
		scene=dynamic_cast<GameScene*>(obj);
		if(scene)break;
	}
	return scene;
}