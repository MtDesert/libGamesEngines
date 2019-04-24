#ifndef GAMEMENU_STRING_H
#define GAMEMENU_STRING_H

#include"GameMenu.h"
#include"GameString.h"

/*只显示文字项的菜单,实现方法比较简单,子类可以参考这个例子*/
class GameMenu_String:public GameMenu{
	//变量
	List<GameString> gameStringList;//保存解码的内容
public:
	//构造/析构函数
	GameMenu_String();
	~GameMenu_String();

	void addString(const string &str);//添加字符串

	//重写
	uint rowAmount()const;
	void renderX()const;//渲染方法,主要渲染文字
	//更新渲染参数
	virtual void updateRenderParameters();
};

#endif