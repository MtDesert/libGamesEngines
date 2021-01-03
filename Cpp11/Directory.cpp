#include"Directory.h"
#include"define.h"

#include<string.h>
#include<time.h>

//目录分隔符
#define DIRECTORY_SEPERATOR "/"
#define ASSERT_DIRECTORY(code) ASSERT_ERRNO(code,path+": ");

DirectoryEntry::DirectoryEntry(){}
DirectoryEntry::DirectoryEntry(const dirent &dirEnt):dirent(dirEnt){}
DirectoryEntry::~DirectoryEntry(){}

decltype(dirent::d_ino) DirectoryEntry::indexNode()const{return d_ino;}
decltype(dirent::d_reclen) DirectoryEntry::nameLength()const{return d_reclen;}

bool DirectoryEntry::isFIFO()const{return S_ISFIFO(structStat.st_mode);}
bool DirectoryEntry::isCharacterDevice()const{return S_ISCHR(structStat.st_mode);}
bool DirectoryEntry::isDirectory()const{return S_ISDIR(structStat.st_mode);}
bool DirectoryEntry::isBlockDevice()const{return S_ISBLK(structStat.st_mode);}
bool DirectoryEntry::isRegularFile()const{return S_ISREG(structStat.st_mode);}

string DirectoryEntry::name()const{return d_name;}
string DirectoryEntry::strSize()const{
	if(isDirectory())return"DIR";
	char str[16],ch='B';
	float sz(structStat.st_size);
	if(sz>=1000){//转换成K为单位
		sz/=1000;
		ch='K';
	}
	if(sz>=1000){//转换成M为单位
		sz/=1000;
		ch='M';
	}
	sprintf(str,"%.2f%c",sz,ch);
	return str;
}

#define ENTRY_TIME(Name,var) \
string DirectoryEntry::str##Name##Time()const{\
	char str[100];\
	strftime(str,100,"%F %X",localtime(&structStat.var));\
	return str;\
}
ENTRY_TIME(Access,st_atime)
ENTRY_TIME(Modify,st_mtime)
ENTRY_TIME(Change,st_ctime)

//DirentList
static bool compareIndexNode(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.d_ino<entryB.d_ino;}
static bool compareRecLen(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.d_reclen<entryB.d_reclen;}
static bool compareType(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return entryA.structStat.st_mode<entryB.structStat.st_mode;}
static bool compareName(const DirectoryEntry &entryA,const DirectoryEntry &entryB){return strcmp(entryA.d_name,entryB.d_name)<0;}
static bool compareTypeAndName(const DirectoryEntry &entryA,const DirectoryEntry &entryB){
	//判断是否目录
	bool aIsDir=entryA.isDirectory(),bIsDir=entryB.isDirectory();
	if(aIsDir && !bIsDir)return true;
	if(!aIsDir && bIsDir)return false;
	//类型相同就按名字来排
	return compareName(entryA,entryB);
}
#define DIRENTLIST_COMPARE(name) case By##name:sort(compare##name);break;
void DirentList::sortBy(SortBy by){
	switch(by){
		DIRENTLIST_COMPARE(IndexNode)
		DIRENTLIST_COMPARE(RecLen)
		DIRENTLIST_COMPARE(Type)
		DIRENTLIST_COMPARE(Name)
		DIRENTLIST_COMPARE(TypeAndName)
		default:;
	}
}

//目录
void Directory::clear(){
	dirNames.clear();
	direntList.clear();
}

//把路径转换成字符串列表
static void path2stringList(const string &path,StringList &strList,bool clearBeforeConvert){
	strList.splitString(path,DIRECTORY_SEPERATOR,clearBeforeConvert);
	//处理.和..的情况
	auto itr = strList.begin();
	while(itr != strList.end()){
		if(*itr=="." || *itr==""){itr=strList.erase(itr);}
		else if(*itr==".."){
			--itr;
			itr=strList.erase(itr);//移除上级目录
			itr=strList.erase(itr);//移除..
		}
		else ++itr;
	}
}
//把字符串列表转换成路径
static string stringList2path(const StringList &strList){
	if(strList.size()==0){return ".";}
	return strList.combineString(DIRECTORY_SEPERATOR);
}

string Directory::toString()const{return stringList2path(dirNames);}
bool Directory::changeDir(const string &path,WhenErrorString whenError){
	bool ret=false;
	//生成新目录(不保证新目录一定能访问)
	auto tmpList=dirNames;
	path2stringList(path,tmpList,false);
	auto fullpath=stringList2path(tmpList);
	//开始切换目录
	DIR *dir=opendir(fullpath.data());
	ASSERT_DIRECTORY(dir)
	if(dir){
		//开始读取子目录文件
		direntList.clear();
		errno=0;
		auto entry=readdir(dir);
		for(;entry!=nullptr;entry=readdir(dir)){//得到entry信息
			ASSERT_DIRECTORY(errno==0)//readdir
			//if(strcmp(entry->d_name,".")==0)continue;//过滤"."
			if(strcmp(entry->d_name,"..")==0 && tmpList.size()<=0)continue;//根目录,过滤".."
			DirectoryEntry de(*entry);
			ASSERT_DIRECTORY(::stat((fullpath+DIRECTORY_SEPERATOR+entry->d_name).data(),&de.structStat)==0);//得到stat信息
			direntList.push_back(de);
		}
		ASSERT_DIRECTORY(errno==0)//readdir
		ASSERT_DIRECTORY(closedir(dir)==0);//关闭目录
		//保存目录信息
		dirNames=tmpList;
		ret=true;
	}
	return ret;
}
bool Directory::exist(const string &name){
	auto dir=opendir(name.data());
	if(dir){
		closedir(dir);
		return true;
	}
	return false;
}
bool Directory::makeDirectory(const string &name){
#ifdef __MINGW32__
	return mkdir(name.data())==0;
#else
	return mkdir(name.data(),S_IRWXU)==0;
#endif
}
bool Directory::scan(const string &path,function<void(const string&)> dirCallback,function<void(const string&)> fileCallback){
	Directory dir;
	if(dir.changeDir(path)){
		string name;
		for(auto &entry:dir.direntList){//处理所有path下的文件
			//过滤
			name=entry.name();
			if(name=="."||name=="..")continue;
			//生成路径名
			name=dir.toString()+DIRECTORY_SEPERATOR+name;
			if(entry.isRegularFile()){//常规文件,处理之
				if(fileCallback)fileCallback(name);
			}else if(entry.isDirectory()){//目录,继续扫描
				scan(name,dirCallback,fileCallback);
			}
		}
	}else{
		printf("!!!!\n");
	}
	return true;
}