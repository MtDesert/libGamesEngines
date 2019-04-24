#ifndef GAMESTRING_H
#define GAMESTRING_H

#include"GameSprite.h"
#include"FontTextureCache.h"

#include<string>
using namespace std;

/*游戏字符串,用于在屏幕上显示
此类只负责显示单行
可以考虑后期加入局部绘制功能
*/
class GameString:public GameSprite{
public:
	//构造/析构函数
	GameString();
	GameString(const string &str);
	~GameString();

	//成员变量
	Point2D<uint> charSize;//文字宽高,请注意这里的宽是ASCII字符的宽度,汉字的话则是两倍此宽度
	//成员函数
	virtual void setString(const string &str);//设置文本内容
	uint stringWidth()const;//文本宽度,即所有文本所生成的字体加起来的总宽度
	virtual Point2D<float> sizeF()const;//根据文字内容计算尺寸
	virtual void renderX()const;//实时渲染文字

	//静态变量
	static Charset charset;//GameString共用的字符集转化方案,暂时还没提供换字符集的方案
	static FontTextureCache fontTextureCache;//GameString共用的文字缓存,暂时还没提供换字体的方案
protected:
	struct CharAttr{//控制文字渲染的结构
		bool isAscii;
		Texture tex;
	};
	Array<CharAttr> arrayCharAttr;
	uint strWidth;//文字物理宽度
	//渲染部分文字,从arrayCharAttr的from下标开始,渲染amount个字符
	void renderX(uint from,uint amount)const;
};

#endif