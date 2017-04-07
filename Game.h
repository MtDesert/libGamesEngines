#ifndef GAME_H
#define GAME_H

#include"typedef.h"
#include<sys/time.h>

class Game
{
public:
	Game();
	static Game* getGame();

	//keyboard
	virtual void keyboard_Up(bool);
	virtual void keyboard_Down(bool);
	virtual void keyboard_Left(bool);
	virtual void keyboard_Right(bool);
	void keyboard_bigLetter(unsigned char);
	void keyboard_smallLetter(unsigned char);

	//mouse
	void mouse_Move(int x,int y);
	void mouse_LeftButton(int x,int y,bool);
	void mouse_MiddleButton(int x,int y,bool);
	void mouse_RightButton(int x,int y,bool);

	//joystick
#define GAME_JOYSTICK(key) virtual void joystick_##key(bool pressed);
	GAME_JOYSTICK(Up)
	GAME_JOYSTICK(Down)
	GAME_JOYSTICK(Left)
	GAME_JOYSTICK(Right)
	GAME_JOYSTICK(Select)
	GAME_JOYSTICK(Start)
	GAME_JOYSTICK(A)
	GAME_JOYSTICK(B)
	GAME_JOYSTICK(C)
	GAME_JOYSTICK(X)
	GAME_JOYSTICK(Y)
	GAME_JOYSTICK(Z)
	GAME_JOYSTICK(L1)
	GAME_JOYSTICK(L2)
	GAME_JOYSTICK(R1)
	GAME_JOYSTICK(R2)

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

#endif