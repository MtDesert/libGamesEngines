#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include"GameSprite.h"
#include"GameString.h"

/*游戏中的按钮,可以响应鼠标或触摸事件*/
class GameButton:public GameSprite{
public:
	GameButton();
	~GameButton();

	//成员变量
	Point2D<int> buttonSize;//按钮的大小,可用于无纹理的按钮绘制边框
	decltype(GameString::stringCode) stringCode;//支持文字显示
	decltype(GameString::charSize) charSize;//支持文字调整大小
	bool isPressed;//是否处于按下状态
	ColorRGBA bgColor;//背景色,按下状态时颜色互换

	//设置函数
	void setString(const string &str);
	//以下函数有可能子类差异化
	virtual void mouseMove(int x,int y);//鼠标移动出区域的时候恢复为不按下状态
	virtual void mouseKey(MouseKey key,bool pressed);//让按钮可以响应鼠标点击
	virtual void render()const;//按钮可能有纹理和文字需要绘制
	virtual Point2D<float> sizeF()const;//无纹理的情况下可以使用自定义的大小
};

#endif
