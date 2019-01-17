#include "Directory.h"

#include<string.h>
#include<time.h>

DirectoryEntry::DirectoryEntry(){}
DirectoryEntry::DirectoryEntry(const dirent &dirEnt):dirent(dirEnt){}
DirectoryEntry::~DirectoryEntry(){}

bool DirectoryEntry::operator==(const DirectoryEntry &directoryEntry)const{
	return *this==directoryEntry;
}

decltype(dirent::d_ino) DirectoryEntry::indexNode()const{return d_ino;}
decltype(dirent::d_reclen) DirectoryEntry::nameLength()const{return d_reclen;}

bool DirectoryEntry::isUnknown()const{return d_type==DT_UNKNOWN;}
bool DirectoryEntry::isFIFO()const{return d_type==DT_FIFO;}
bool DirectoryEntry::isCharacterDevice()const{return d_type==DT_CHR;}
bool DirectoryEntry::isDirectory()const{return d_type==DT_DIR;}
bool DirectoryEntry::isBlockDevice()const{return d_type==DT_BLK;}
bool DirectoryEntry::isRegularFile()const{return d_type==DT_REG;}
bool DirectoryEntry::isSymbolicLink()const{return d_type==DT_LNK;}
bool DirectoryEntry::isSocket()const{return d_type==DT_SOCK;}

#define DIRENT_CASE(name) case name:return #name;
string DirectoryEntry::directoryTypename()const{
	switch(d_type){
		DIRENT_CASE(DT_UNKNOWN)
		DIRENT_CASE(DT_FIFO)
		DIRENT_CASE(DT_CHR)
		DIRENT_CASE(DT_DIR)
		DIRENT_CASE(DT_BLK)
		DIRENT_CASE(DT_REG)
		DIRENT_CASE(DT_LNK)
		DIRENT_CASE(DT_SOCK)
		default:return "????????";
	}
}

string DirectoryEntry::name()const{return d_name;}
string DirectoryEntry::strSize()const{
	char str[8],ch='B';
	decltype(st_size) sz(st_size);
	if(sz>=1000000){//转换成K为单位
		sz/=1000;
		ch='K';
	}
	if(sz>=1000000){//转换成K为单位
		sz/=1000;
		ch='M';
	}
	sprintf(str,"%ld%c",st_size,ch);
	return str;
}
string DirectoryEntry::strModifyDate()const{
	char str[20];
	strftime(str,20,"%F %T",localtime(&st_mtime));
	return str;
}

//DirentList
static bool compareIndexNode(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.d_ino<entryB.d_ino;}
static bool compareRecLen(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.d_reclen<entryB.d_reclen;}
static bool compareType(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.d_type<entryB.d_type;}
static bool compareName(const DirectoryEntry &entryA,const DirectoryEntry &entryB){
	return strcmp(entryA.d_name,entryB.d_name)<0;
}
#define DIRENTLIST_COMPARE(name) case By##name:sort(compare##name);break;
void DirentList::sortBy(SortBy by){
	switch(by){
		DIRENTLIST_COMPARE(IndexNode)
		DIRENTLIST_COMPARE(RecLen)
		DIRENTLIST_COMPARE(Type)
		DIRENTLIST_COMPARE(Name)
		default:;
	}
}

//目录
void Directory::clear(){
	dirNames.clear();
	direntList.clear();
}
string Directory::toString()const{
	string ret;
	if(dirNames.size()==0){
		return ".";
	}
	for(auto name:dirNames){
		ret+="/"+name;//Linux系列的风格,以后可以加上WINDOWS的风格
	}
	ret.erase(0,1);//去掉最开头的斜杠变成相对路径
	return ret;
}
bool Directory::changeDir(const string &dirName,bool read){
	bool ret=false;
	dirNames.push_back(dirName);//组合出新的目录
	if(read){
		ret=readDir(toString(),direntList);
	}else{
		auto dir=opendir(toString().data());//尝试打开
		ret=dir;
		if(dir)closedir(dir);
	}
	dirNames.pop_back();//还原
	if(ret){//目录可以访问,那么我们就保存结果
		parseAndSaveDirName(dirName);
	}
	return ret;
}

bool Directory::readDir(){return readDir(direntList);}
bool Directory::readDir(StringList &stringList)const{return readDir(toString(),stringList);}
bool Directory::readDir(DirentList &entryList)const{return readDir(toString(),entryList);}

bool Directory::isDotStr(const string &str){return str.compare(".")==0;}
bool Directory::isDotDotStr(const string &str){return str.compare("..")==0;}
bool Directory::isDotEntry(const dirent &entry){return isDotStr(entry.d_name);}
bool Directory::isDotDotEntry(const dirent &entry){return isDotDotStr(entry.d_name);}

bool Directory::readDir(const string &dirName,StringList &stringList){
	DIR *dir=opendir(dirName.data());
	if(!dir)return false;
	//开始读取
	stringList.clear();
	dirent *entry=readdir(dir);
	for(;entry!=nullptr;entry=readdir(dir)){
		stringList.push_back(entry->d_name);
	}
	//移除.和..
	stringList.remove_if(isDotStr);
	//stringList.remove_if(isDotDotStr);
	//完成
	return closedir(dir)==0;
}
bool Directory::readDir(const string &dirName,DirentList &entryList){
	DIR *dir=opendir(dirName.data());
	if(!dir)return false;
	//开始读取
	entryList.clear();
	dirent *entry=readdir(dir);
	for(;entry!=nullptr;entry=readdir(dir)){
		DirectoryEntry de(*entry);
		::stat((dirName+"/"+entry->d_name).data(),&de);
		entryList.push_back(de);
	}
	//移除.和..
	entryList.remove_if(isDotEntry);
	//entryList.remove_if(isDotDotEntry);
	//完成
	return closedir(dir)==0;
}

void Directory::parseAndSaveDirName(const string &dirName){
	StringList names;
	names.splitString(dirName,"/");
	for(auto item:names){//处理.和..的情况
		if(item=="."){//.直接忽略
		}else if(item==".."){//..可能要添加,可能要移除上级目录
			if(dirNames.size() && dirNames.back()!=".."){
				dirNames.pop_back();//弹出
			}else{
				dirNames.push_back(item);//item其实就是..
			}
		}else{//是目录名的情况下,直接添加
			dirNames.push_back(item);
		}
	}
}