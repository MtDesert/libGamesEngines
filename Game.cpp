#include"Game.h"
#include"GameScene.h"
#include<iostream>
using namespace std;

Game::Game(){}
//Game* Game::newGame(){return new Game();}

void Game::render(){
	for(GameObject *obj:subObjects){
		GameScene *scene=dynamic_cast<GameScene*>(obj);
		if(scene){
			scene->render();
			break;
		}
	}
}