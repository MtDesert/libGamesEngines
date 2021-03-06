#include "FileLoader.h"

#define CALLBACK(funcName,para) \
if(funcName){funcName(para);}

FileLoader::FileLoader():
whenLoadingFilename(nullptr),
whenLoadFinish(nullptr){}

void FileLoader::startLoad(){
	while(allFilenamesToLoad.size()){
		auto filename=allFilenamesToLoad.front();
		CALLBACK(whenLoadingFilename,filename)
		//清除
		allFilenamesToLoad.pop_front();
	}
}