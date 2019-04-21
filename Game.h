#ifndef GAME_H
#define GAME_H

#include"GameObject.h"
#include"GameScene_FileList.h"
#include"FontTextureCache.h"

/** Game是整个游戏运行的环境，游戏的主要数据都在本类中
该类主要是内存操作，与现有的游戏（或非游戏）图形环境等只做对接，比如glfw,glut,Qt,SDL,SFML等各种数不清的图形环境
可以接受控制设备的输入，但是控制设备是怎么工作的，这是图形库或者系统相关的图形界面程序要去考虑的事情，本类并不考虑，只提供成员函数来接受输入
*/

class Game:public GameObject{
public:
	Game();
	~Game();

	static Game* newGame();//创建游戏,请在子类实现
	static Game* currentGame();//当前游戏,请在子类实现
	//输入输出变量
	static Point3D<int> resolution;//分辨率
	static Point2D<int> mousePos;//鼠标位置

	//翻译
	bool loadTranslationFile(const string &filename);
	const char* translate(const string &english)const;//翻译(英文原文),返回译文,翻译失败则返回原文
	//文件选择
	GameScene_FileList* showScene_FileList();//显示选择文件列表
	void deleteScene_FileList();//删除文件列表所占的内存
	//重写方法
	void mouseMove(int x,int y);
	void addTimeSlice(uint usec);
	void render()const;
protected:
	static Game *game;//运行中的游戏
	//翻译
	Map<string,string> translationMap;
	//文件管理
	GameScene_FileList *sceneFileList;

	GameScene* findFirstGameScene()const;
};

#endif