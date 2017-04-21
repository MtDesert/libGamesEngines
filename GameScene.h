#ifndef GAMESCENE_H
#define GAMESCENE_H

#include"GameObject.h"

class GameScene:public GameObject
{
public:
	GameScene();

	virtual void render();
};
#endif