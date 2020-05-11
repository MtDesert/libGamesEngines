#include"CurlEasy.h"
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>

CurlEasy::CurlEasy(){curl=curl_easy_init();}
CurlEasy::~CurlEasy(){curl_easy_cleanup(curl);}

static char progressText[32]={'\0'};
static int progressCallback(void *clientp,double dlTotal,double dlNow,double ulTotal,double ulNow){
	//删除原文本
	auto len=strlen(progressText);
	for(size_t i=0;i<len;++i)printf("\b \b");
	progressText[0]='\0';
	//输出进度值
	if((int)dlTotal>0){//防止除数为0
		sprintf(progressText,"%d/%d(%.2f%%)",(int)dlNow,(int)dlTotal,dlNow*100/dlTotal);
		printf("%s",progressText);
		fflush(stdout);
	}
	return CURLE_OK;
}
int CurlEasy::downloadFile(const string &url, const string &filename,int minSize)const{
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
	curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,progressCallback);
	curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0);
	//启用长连接
	//curl_easy_setopt(curl,CURLOPT_TCP_KEEPALIVE,1L);
	//curl_easy_setopt(curl,CURLOPT_TCP_KEEPIDLE,120L);
	//curl_easy_setopt(curl,CURLOPT_TCP_KEEPINTVL,60L);
	auto result=curl_easy_perform(curl);//开始下载
	if(result==CURLE_OK){
		printf("成功\n");
	}else{
		printf("curl失败,错误号%d\n",result);
	}
	//关闭文件
	fflush(file);
	fclose(file);
	//确认大小
	struct stat st;
	if(::stat(filename.data(),&st)==0){
		if(st.st_size<=minSize){
			printf("%s文件大小为%ld,删除\n",filename.data(),st.st_size);
			remove(filename.data());
			return -1;
		}
	}
	return errno;
}

#define BUFFER_SIZE 65536
static char buffer[BUFFER_SIZE];
int CurlEasy::parseHtml(const string &filename){
	clearCache();
	//打开文件
	FILE *file=fopen(filename.data(),"r");
	if(!file)return errno;
	//分析数据
	auto line=0;
	while(fgets(buffer,BUFFER_SIZE,file)){//逐行处理
		buffer[BUFFER_SIZE-1]='\0';//hack
		++line;
		//printf("scanning %d\n",line);
		parseHtmlLine(buffer);
		//usleep(100000);
	}
	//关闭
	fclose(file);
	return 0;
}
void CurlEasy::clearCache(){}
void CurlEasy::parseHtmlLine(char *buffer){}

string CurlEasy::parseContent(const char *line,const char *startMark,const char *finMark,bool reserveMark){
	//搜索数据起始位置
	auto start=strstr(line,startMark);
	if(!start)return "";
	auto startMarkLen=strlen(startMark);
	//搜索数据终止位置
	auto fin=strstr(start+startMarkLen,finMark);
	if(!fin)return "";
	auto finMarkLen=strlen(finMark);
	//返回字符串
	if(reserveMark){
		fin+=finMarkLen;
	}else{
		start+=startMarkLen;
	}
	return string(start,fin-start);
}
string CurlEasy::getUrlFilename(const string &url){
	auto pos=url.find_last_of("/");
	string ret;
	if(pos!=string::npos){
		ret=url.substr(pos+1);
	}
	return ret;
}
string CurlEasy::getSuffix(const string &url){
	auto pos=url.find_last_of(".");
	string ret;
	if(pos!=string::npos){
		ret=url.substr(pos);
	}
	return ret;
}