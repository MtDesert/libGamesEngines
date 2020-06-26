#ifndef DIRECTORY_H
#define DIRECTORY_H

#include"StringList.h"
#include"Array.h"

#include<dirent.h>
#include<sys/stat.h>

/*目录条目,实际上这个类扩展了系统的dirent结构体,主要目的是提供方便的函数来判断条目的属性*/
struct DirectoryEntry:public dirent{
	//构造/析构函数
	DirectoryEntry();
	DirectoryEntry(const dirent &dirEnt);
	~DirectoryEntry();

	struct stat structStat;

	decltype(d_ino) indexNode()const;//索引节点号
	decltype(d_reclen) nameLength()const;//文件名长度

	bool isFIFO()const;//先进先出的设备
	bool isCharacterDevice()const;//字符设备
	bool isDirectory()const;//目录
	bool isBlockDevice()const;//块设备
	bool isRegularFile()const;//正常文件

	//字符串值(推导结果)
	string name()const;//条目的名字
	string strSize()const;//条目的大小(以B,K,M,G为单位,显示大概数值)
	string strAccessTime()const;//访问时间(字符串)
	string strModifyTime()const;//修改时间(字符串)
	string strChangeTime()const;//权限变更时间(字符串)
};

//目录条目表,提供按特性来排序的功能
class DirentList:public Array<DirectoryEntry>{
public:
	enum SortBy{//排序,按照dirent结构体的属性来排
		ByIndexNode,
		ByRecLen,
		ByType,
		ByName,
		ByTypeAndName,
		AmountOf_SortBy
	};
	void sortBy(SortBy by);//开始排序
};

/*本类代表一个目录,提供目录操作的功能*/
struct Directory{
	//成员变量
	StringList dirNames;//各个层级的目录名,越前面的则越接近根目录,最好不要试图对此进行排序,除非你知道你在干什么
	DirentList direntList;//保存读取结果的成员

	//主要成员方法
	void clear();//清除成员变量的值
	string toString()const;//把目录转化成字符串的形式
	//切换目录
	bool changeDir(const string &path,WhenErrorString whenError=nullptr);//其实就相当于cd dirName命令,readDir为true的时候会读取目录并存入成员变量direntList中,不能保证目录一定能访问,请留意返回值
};
#endif