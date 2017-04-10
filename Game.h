#ifndef GAME_H
#define GAME_H

#include"TimeSlice.h"
#include"GameScene.h"
#include"List.h"

/** Game是整个游戏运行的环境，游戏的主要数据都在本类中
该类主要是内存操作，与现有的游戏（或非游戏）图形环境等只做对接，比如glfw,glut,Qt,SDL,SFML等各种数不清的图形环境
可以接受控制设备的输入，但是控制设备是怎么工作的，这是图形库或者系统相关的图形界面程序要去考虑的事情，本类并不考虑，只提供成员函数来接受输入
*/

class Game:public TimeSlice
{
public:
	Game();
	static Game* newGame();

	//keyboard(模拟键盘输入)
	virtual void keyboard_Up(bool);
	virtual void keyboard_Down(bool);
	virtual void keyboard_Left(bool);
	virtual void keyboard_Right(bool);
	void keyboard_bigLetter(unsigned char);
	void keyboard_smallLetter(unsigned char);

	//mouse(模拟鼠标输入)
	void mouse_Move(int x,int y);
	void mouse_LeftButton(int x,int y,bool);
	void mouse_MiddleButton(int x,int y,bool);
	void mouse_RightButton(int x,int y,bool);

	//joystick(模拟手柄输入)
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
	
	//RAM(内存管理,主要是存储成员变量)
	List<GameScene*> allScenes;
	GameScene *currentScene;
	void addTimeSlice(uint usec);
};

#endif