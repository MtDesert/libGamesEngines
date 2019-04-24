#ifndef GAMETEXT_H
#define GAMETEXT_H

#include"GameString.h"

#include<list>
using namespace std;

/*游戏文本,用于在屏幕上显示
用于显示多行
*/
class GameText:public GameString{
public:
	//构造/析构函数
	GameText();
	GameText(const string &text);
	~GameText();

	//换行相关
	uint lineCharAmount;//一行的字母数量,超过的话绘制成自动换行
	uint lineAmount()const;//根据内容推断行数
	void updateRenderParameter();

	virtual void setString(const string &str);
	virtual Point2D<float> sizeF()const;//根据文字内容计算尺寸
	virtual void renderX()const;//实时渲染文字
protected:
	list<uint> lineStart;//保存每行的起始位置
};

#endif
