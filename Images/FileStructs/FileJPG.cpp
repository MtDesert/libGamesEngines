#include "FileJPG.h"

FileJPG::FileJPG(){}

DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_Segment,SegmentType,uint8,1,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_Segment,SegmentSize,uint16,2,false)

string FileJPG_Segment::strType()const{
	uint8 type;
	if(!getSegmentType(type))return"No get";
	switch(type){
#define CASE(name) case name:return #name;
		CASE(SOF0)
		CASE(SOF2)
		CASE(DHT)
		CASE(RST0)
	}
	return"Unknown";
}
SizeType FileJPG_Segment::parseData(){
	uint16 size;
	if(getSegmentSize(size)){
		dataLength=size;
	}
	return dataLength;
}

bool FileJPG_APP0::getIdentifier(string &value)const{return get_string(4,value);}
bool FileJPG_APP0::setIdentifier(const string &value){
	if(value.size()>4)return false;
	return set_string(4,value);
}
//Identifier=="JFIF\0"
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,MajorVersion,uint8,9,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,MinorVersion,uint8,10,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,Units,uint8,11,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,Xdensity,uint16,12,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,Ydensity,uint16,14,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,Xthumbnail,uint8,16,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,Ythumbnail,uint8,17,true)
DataBlock FileJPG_APP0::thumbnailData()const{
	uint8 x,y;
	if(getXthumbnail(x)&&getYthumbnail(y)){
		return subDataBlock(18,x*y*3);
	}return DataBlock();
}
//Identifier=="JFXX\0"
DATABLOCK_CUSTOM_TYPE_CPP(FileJPG_APP0,ExtensionCode,uint8,9,true)

SizeType FileJPG::parseData(){
	SizeType pos=0;
	FileJPG_Segment seg;
	while(pos<dataLength){
		//扫描标记头
		while(dataPointer[pos]==0xFF)++pos;//跳过连续的0xFF
		if(pos>0)--pos;//找到可能为标记头的区段,开始识别
		subDataBlock(pos,dataLength-pos,seg);
		pos+=seg.parseData();
	}
	return pos;
}