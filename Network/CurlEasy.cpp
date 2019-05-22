#include"CurlEasy.h"
#include<stdio.h>
#include<string.h>

CurlEasy::CurlEasy(){
	curl=curl_easy_init();
}
CurlEasy::~CurlEasy(){curl_easy_cleanup(curl);}

int CurlEasy::downloadFile(const string &url,const string &filename)const{
	printf("正在下载%s 到 %s....",url.data(),filename.data());
	fflush(stdout);
	//打开要写入的文件
	FILE *file=fopen(filename.data(),"wb");
	if(!file){
		printf("文件写入失败\n");
		return errno;
	}
	//设置curl参数
	curl_easy_setopt(curl,CURLOPT_URL,url.data());
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
	//启用长连接
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE,1L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE,120L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL,60L);
	auto result=curl_easy_perform(curl);//开始下载
	if(result==CURLE_OK){
		printf("成功\n");
	}else{
		printf("curl失败,错误号%d\n",result);
	}
	//关闭文件
	fflush(file);
	fclose(file);
	return errno;
}

//解析内容(源数据行,开始标志,结束标志),返回解析结果,解析不出返回nullptr
/*char* parseContent(char *line,const char *startMark,const char *finMark){
	char *start=strstr(line,startMark);//搜索数据起始位置
	if(!start)return nullptr;
	start+=strlen(startMark);
	char *fin=strstr(start,finMark);
	if(!fin)return nullptr;
	*fin='\0';//让start变成字符串
	return start;
}*/