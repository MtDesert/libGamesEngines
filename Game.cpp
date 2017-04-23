#include"Game.h"
#include"GameScene.h"
#include<iostream>
using namespace std;

Game::Game(){}
//Game* Game::newGame(){return new Game();}

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