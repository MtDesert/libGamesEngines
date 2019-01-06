#ifndef GAMESCENE_FILELIST_H
#define GAMESCENE_FILELIST_H

#include"GameScene.h"
#include"GameString.h"
#include"GameTable_Dir.h"
#include"GameButton.h"

/*选择文件用的场景*/
class GameScene_FileList:public GameScene{
public:
	GameScene_FileList();

	GameString textTitle;//标题,告知玩家需要选择什么文件
	GameString textPath;//路径,用于显示文件位置
	GameTable_Dir gameTableDir;//文件列表,用于显示文件名等基本信息

	GameButton buttonConfirm;//确认按钮,用来打开文件
	GameButton buttonFile;//文件按钮,对选中文件进行操作
	GameButton buttonDir;//目录按钮,使用目录的常用功能
	GameButton buttonCancel;//取消按钮,关闭本页面
};
#endif // GAMESCENE_FILELIST_H