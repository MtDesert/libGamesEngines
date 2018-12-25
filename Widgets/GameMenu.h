#ifndef GAMEMENU_H
#define GAMEMENU_H

#include"GameSprite.h"
#include"List.h"
#include"ShapeRenderer.h"

enum GameMenuStatus{
	Selecting/*待命*/,Confirm/*确定*/,Cancel/*取消*/
};

/*游戏中用到的菜单,可以显示各种数据,具体的显示方法一般在子类实现*/
class GameMenu:public GameSprite{
public:
	//变量
	uint renderItemStart;//渲染开始项,表示从第几项开始渲染
	uint renderItemAmount;//最多显示多少项,多余的项不会被绘制
	size_t itemWidth,itemHeight;//菜单项的寬高
	//菜单状态
	uint selectingItemIndex;
	char menuStatus;//菜单状态,供外部进行检测以便得知用户所做的操作

	//构造函数
	GameMenu();
	~GameMenu();

	//菜单项数
	virtual uint rowAmount()const;
	virtual Point2D<float> sizeF()const;//菜单尺寸,根据项尺寸和显示项数决定(子类可能会多出边缘或者间距等部分)

	//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
	virtual void keyboardKey(Keyboard::KeyboardKey key,bool pressed);
	//渲染
	virtual void render()const;
};
#endif