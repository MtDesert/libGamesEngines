#ifndef GAMESCENE_H
#define GAMESCENE_H

#include"GameSprite.h"

class GameScene:public GameSprite
{
public:
	GameScene();

	virtual void render()const;
};
#endif