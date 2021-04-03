#ifndef FILEJPG_H
#define FILEJPG_H

#include"DataBlock.h"

//JPG文件的各个数据区段,每个区段由2字节区段类型和n字节负载数据组成,有些区段(如SOI和EOI)没有负载数据
struct FileJPG_Segment:public DataBlock{
	//区段类型
	enum SegmentType{
		SOF0=0xC0,//Start Of Frame,帧数据开始
		SOF2=0xC2,//Start Of Frame,帧数据开始
		DHT=0xC4,//Define Huffman Table,定义哈夫曼表
		RST0=0xD0,//Restart重新开始
		//注意,0xD1~0xD7对应RST1~RST7,无负载信息
		SOI=0xD8,//Start Of Image,图像开始,即JPG文件以FF D8开头
		EOI=0xD9,//End Of Image,图像结束,即JPG文件以FF D9结束
		SOS=0xDA,//Start Of Scan,开始扫描
		DQT=0xDB,//Define Quantization Table,定义量化表
		DRI=0xDD,//Define Restart Interval,定义重启间隔(4字节)
		APP0=0xE0,//Application Marker,应用标记
		//注意,0xE1~0xEF对应APP1~APP15,一般只带有负载信息
		COM=0xFE,//Comment,注释
	};
	DATABLOCK_CUSTOM_TYPE(SegmentType,uint8)//区段类型,首字节都是0xFF,故只取次字节来判断
	DATABLOCK_CUSTOM_TYPE(SegmentSize,uint16)//区段大小(大尾序列),不包括2字节的"区段类型",但包含2字节的"区段大小"本身
	bool isRST()const;
	bool isAPP()const;
	string strType()const;
	virtual SizeType parseData();
};
//应用标记
struct FileJPG_APP0:public FileJPG_Segment{
	DATABLOCK_CUSTOM_TYPE(Identifier,string)//通常为"JFIF\0",但是1.02版本中有可能是"JFXX\0"
	//若Identifier为"JFIF\0",则结构如下
	DATABLOCK_CUSTOM_TYPE(MajorVersion,uint8)//主版本号(通常是1)
	DATABLOCK_CUSTOM_TYPE(MinorVersion,uint8)//次版本号(通常是1或2)
	DATABLOCK_CUSTOM_TYPE(Units,uint8)//单位:0.密度给出长宽比,1.密度值为每英寸点数,2.密度值为每厘米点数
	DATABLOCK_CUSTOM_TYPE(Xdensity,uint16)//水平像素密度
	DATABLOCK_CUSTOM_TYPE(Ydensity,uint16)//垂直像素密度
	DATABLOCK_CUSTOM_TYPE(Xthumbnail,uint8)//缩略图水平像素数
	DATABLOCK_CUSTOM_TYPE(Ythumbnail,uint8)//缩略图垂直像素数
	DataBlock thumbnailData()const;//3n字节的缩略图像素数据,n = Xthumbnail * Ythumbnai
	//若Identifier为"JFXX\0",则结构如下
	DATABLOCK_CUSTOM_TYPE(ExtensionCode,uint8)//扩展编码:0x10->缩略图为JPEG,0x11->缩略图1字节/像素,0x13->缩略图3字节/像素
};

class FileJPG:public DataBlock{
public:
	FileJPG();
	SizeType parseData();
};
#endif // FILEJPG_H