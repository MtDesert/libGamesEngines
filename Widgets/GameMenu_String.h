#ifndef GAMEMENU_STRING_H
#define GAMEMENU_STRING_H

#include"GameMenu.h"
#include"GameString.h"

/*只显示文字项的菜单,实现方法比较简单,子类可以参考这个例子*/
class GameMenu_String:public GameMenu<DataBlock>{
public:
	GameMenu_String();
	~GameMenu_String();

	virtual Point2D<GLfloat> sizeF()const;//在原基础上加上上下两部分的一点间距用于显示小箭头
	virtual void render()const;//利用GameText的效果来进行渲染
	//成员函数
	void insertItem(const string &itemText);//插入新的文本内容
	void clearAllItems();//清除所有文本内容
	void refresh();//刷新,更新项数和最大长度
	GameString mGameString;//用来转换和渲染,可以修改此值以便修改菜单风格
protected:
	//变量
	List<DataBlock> stringCodeList;//保存各个菜单项的文字
	//函数
	virtual void renderItem(const List<DataBlock>::iterator &itr,const Rectangle<float> &rect)const;
};

#endif
