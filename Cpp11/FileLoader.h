#ifndef FILELOADER_H
#define FILELOADER_H

#include"StringList.h"

/*文件加载模块,负责把文件从外存加载到内存中
一般使用线程加载,也可以改成不用线程加载的方案*/
class FileLoader{
public:
	FileLoader();
	//文件加载队列
	StringList allFilenamesToLoad;//要加载的所有文件队列

	void startLoad();
	//加载过程回调函数
	void (*whenLoadingFilename)(const string &filename);
	void (*whenLoadFinish)();//加载结束
};
#endif