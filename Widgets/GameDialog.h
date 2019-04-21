#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include"GameText.h"
#include"GameButton.h"

/*游戏对话框，用于在游戏中显示一些提示信息*/
class GameDialog:public GameSprite{
public:
	GameDialog();
	~GameDialog();

	virtual Point2D<float> sizeF()const;//根据内容动态计算尺寸
	virtual void renderX()const;
	//主要控件
	GameText mGameText;//文本内容,显示给玩家需要看的信息
	GameButton_String mGameButton;//按钮,用来给玩家进行操作
	//设置内容
	void setText(const string &text);
	static uint maxLineCharAmount;//最大行字符数,超出此行宽会自动换行
};

#endif
