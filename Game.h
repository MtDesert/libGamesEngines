#ifndef GAME_H
#define GAME_H

#ifdef __cplusplus
extern"C"{
#endif
#include"typedef.h"
#include<sys/time.h>

class Game
{
public:
	Game();

	//keyboard
	void keyboard_Up(bool);
	void keyboard_Down(bool);
	void keyboard_Left(bool);
	void keyboard_Right(bool);

	//mouse
	void mouse_Move(int x,int y);
	void mouse_LeftButton(int x,int y,bool);
	void mouse_MiddleButton(int x,int y,bool);
	void mouse_RightButton(int x,int y,bool);

	//timer
	void startTicks();
	void goTicks();
	bool needRedraw;
protected:
	//timer
	timeval oldTimeVal;
	timeval newTimeVal;
	uint32 usecRemain;

	uint32 interval;
	uint32 fps;
	uint32 frameElapse;
private:
};

#ifdef __cplusplus
}
#endif

#endif