#ifndef GAME_PLANEHITPLANE_H
#define GAME_PLANEHITPLANE_H

#include"Game.h"
class Game_PlaneHitPlane:public Game{
public:
	void keyboard_Up(bool);
	void keyboard_Down(bool);
	void keyboard_Left(bool);
	void keyboard_Right(bool);
};

#endif