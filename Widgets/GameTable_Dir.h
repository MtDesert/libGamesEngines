#ifndef GAMETABLE_DIR_H
#define GAMETABLE_DIR_H

#include"GameTable.h"
#include"Directory.h"

/*显示目录内容的表格,有的时候我们需要在游戏中动态加载文件*/
class GameTable_Dir:public GameTable{
protected:
	//渲染
	virtual void updateBuffer();
public:
	GameTable_Dir();
	~GameTable_Dir();

	Directory directory;//保存当前的目录,以便提供数据显示文件
	bool changeDir(const string &dirName);//切换目录,并刷新视图

	//重写
	virtual void renderItem(uint x,uint y,const Rectangle2D<float> &rectArea)const;

	virtual uint columnAmount()const;//显示文件的属性列数
	virtual uint columnWidth(uint column)const;//设定各个列宽
	virtual void keyboardKey(Keyboard::KeyboardKey key,bool pressed);
protected:
	//缓存结构
	struct FileInfo{
		DataBlock fileName;//文件名
		DataBlock fileSize;//文件大小
	};
	DirectoryEntry *entryBuffer;
};

#endif