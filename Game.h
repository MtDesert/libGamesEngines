#ifndef GAME_H
#define GAME_H

#include"GameObject.h"

/** Game是整个游戏运行的环境，游戏的主要数据都在本类中
该类主要是内存操作，与现有的游戏（或非游戏）图形环境等只做对接，比如glfw,glut,Qt,SDL,SFML等各种数不清的图形环境
可以接受控制设备的输入，但是控制设备是怎么工作的，这是图形库或者系统相关的图形界面程序要去考虑的事情，本类并不考虑，只提供成员函数来接受输入
*/

class Game:public GameObject
{
public:
	Game();
	static Game* newGame();

	//模拟屏幕输出
	virtual void render();
};

#endif