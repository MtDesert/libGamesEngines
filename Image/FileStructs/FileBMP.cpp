#include "FileBMP.h"
#include<cmath>
#include<string>
using namespace std;
#include<stdio.h>

#define RGBA_SIZE 4
static ColorRGBA rgba;
static uint32 value32;

DataBlock::SizeType FileBMP_FileHeader::parseData(){
	if(dataLength>14)dataLength=14;
	return dataLength;
}

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_FileHeader,Type,uint16,0,true)//always "BM"
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_FileHeader,Size,uint32,2,true)//file size
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_FileHeader,Reserved1,uint16,6,true)//always 0
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_FileHeader,Reserved2,uint16,8,true)//always 0
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_FileHeader,OffBits,uint32,10,true)//where BMP data start

bool FileBMP_FileHeader::isValid_Type(uint16 type){
	switch(type){
		case Type_BM:case Type_BA:case Type_CI:case Type_CP:case Type_IC:case Type_PT:return true;
		default:return false;
	}
}
bool FileBMP_FileHeader::isValid_Reserved1(uint16 value){return value==0;}
bool FileBMP_FileHeader::isValid_Reserved2(uint16 value){return value==0;}

DataBlock::SizeType FileBMP_InfoHeader::parseData(){
	uint32 size;
	getSize(size);
	dataLength=size;
	return dataLength;
}

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Size,uint32,0,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Width,uint32,4,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Height,int32,8,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Planes,uint16,12,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,BitCount,uint16,14,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Compression,uint32,16,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ImageSize,uint32,20,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,XPelsPerMeter,uint32,24,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,YPelsPerMeter,uint32,28,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ClrUsed,uint32,32,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ClrImportant,uint32,36,true)

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,RedMask,uint32,40,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,GreenMask,uint32,44,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,BlueMask,uint32,48,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,AlphaMask,uint32,52,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ColorSpaceType,uint32,56,true)

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieRedX,uint32,60,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieRedY,uint32,64,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieRedZ,uint32,68,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieGreenX,uint32,72,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieGreenY,uint32,76,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieGreenZ,uint32,80,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieBlueX,uint32,84,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieBlueY,uint32,88,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,CieBlueZ,uint32,92,true)

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,GammaRed,uint32,96,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,GammaGreen,uint32,100,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,GammaBlue,uint32,104,true)

DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Intent,uint32,108,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ProfileData,uint32,112,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,ProfileSize,uint32,116,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileBMP_InfoHeader,Reserved,uint32,120,true)

#define FILEBMP_POSITIVE_HEIGHT height<0?height=-height:0;

uint FileBMP_InfoHeader::lineSize()const{
	uint16 bitCount;uint32 width;
	if(getBitCount(bitCount)&&getWidth(width)){
		uint bitAmount=bitCount*width;
		uint fourByteAmount=bitAmount/32;
		if(bitAmount%32!=0)++fourByteAmount;
		return fourByteAmount*4;
	}return 0;
}
uint FileBMP_InfoHeader::colorsCount()const{
	uint16 bitCount;
	if(getBitCount(bitCount)){
		return pow(2,(double)bitCount);
	}return 0;
}

bool FileBMP_InfoHeader::isValid_Size(uint32 size){
	switch(size){
		case OS2_1X_BitmapHeader:case OS2_2X_BitmapHeader:
		case BitmapInfoHeader:
		case BitmapV2Header:case BitmapV3Header:
		case OS2_2X_BitmapHeaderX:
		case BitmapV4Header:case BitmapV5Header:
			return true;
		default:return false;
	}
}
bool FileBMP_InfoHeader::isValid_Planes(uint16 planes){return planes==1;}
bool FileBMP_InfoHeader::isValid_BitCount(uint16 bitCount){
	switch(bitCount){
		case 1:case 4:case 8:case 16:case 24:case 32:return true;
		default:return false;
	}
}
bool FileBMP_InfoHeader::isValid_Compression(uint32 compression){
	switch(compression){
		case NoCompression:case RLE8:case RLE4:
		case BitField:case JPEG:case PNG:
		case AlphaBitField:
		case CMYK:case CMYKRLE8:case CMYKRLE4:
			return true;
		default:return false;
	}
}
bool FileBMP_InfoHeader::isValid_ImageSize()const{
	int32 height;uint32 imageSize,compression;
	if(getHeight(height)&&getImageSize(imageSize)&&getCompression(compression)){
		FILEBMP_POSITIVE_HEIGHT
		return lineSize()*height==imageSize||(compression==NoCompression&&imageSize==0);
	}return false;
}

DataBlock::SizeType FileBMP_BGRAsList::parseData(){
	return dataLength-dataLength%4;
}
uint FileBMP_BGRAsList::colorsAmount()const{return dataLength/RGBA_SIZE;}
bool FileBMP_BGRAsList::getColor(uint index,ColorRGBA &color)const{
	if(getColor(index,value32)){
		color.fromBGRA(value32);
		return true;
	}else return false;
}
bool FileBMP_BGRAsList::getColor(uint index,uint32 &color)const{
	return get_uint32(index*RGBA_SIZE,color);
}
bool FileBMP_BGRAsList::setColor(uint index,const ColorRGBA &color){
	return set_uint32(index*4,color.toBGRA());
}
bool FileBMP_BGRAsList::setColor(uint index,uint32 color){
	return set_uint32(index*4,color);
}
void FileBMP_BGRAsList::getColorsList(List<uint32> &colorsList)const{
	colorsList.clear();
	auto len=colorsAmount();
	for(decltype(len) i=0;i<len;++i){
		getColor(i,rgba);
		colorsList.push_back(rgba.toRGBA());
	}
}

void FileBMP_Scanline::setInfoHeader(const FileBMP_InfoHeader &infoHeader){
	int32 h;
	//获取属性
	infoHeader.getWidth(width);
	infoHeader.getHeight(h);
	FILEBMP_POSITIVE_HEIGHT
	height=h;
	uint16 bitCount;
	infoHeader.getBitCount(bitCount);
	bitDepth=bitCount;
	//推断通道数
	if(bitDepth==24 || bitDepth==32){
		channelAmount=bitDepth/8;
		bitDepth=8;
	}else{
		channelAmount=1;
	}
	lineSize=infoHeader.lineSize();
	//设置缓冲区
	createBuffer();
}
bool FileBMP_Scanline::decodeLine(uint y,Bitmap_32bit &bitmap)const{
	decodeLineData();
	for(decltype(width) x=0;x<width;++x){
		switch(channelAmount){
			case 3:case 4:
				rgba.red=getBufferValue(x,0);
				rgba.green=getBufferValue(x,1);
				rgba.blue=getBufferValue(x,2);
				if(channelAmount>3){
					rgba.alpha=getBufferValue(x,3);
				}
				value32=rgba.toRGBA();
			break;
			default:{
				auto ptr=colorsList->data(getBufferValue(x,0));
				value32=ptr ? *ptr : 0;
			}
		}
		bitmap.setColor(x,y,value32);
	}
	return true;
}
//编码一行数据
bool FileBMP_Scanline::encodeLine(uint y,const Bitmap_32bit &bitmap){
	if(bitDepth==0)return false;//位深肯定不能为0
	for(decltype(width) x=0;x<width;++x){
		//取颜色值
		bitmap.getColor(x,y,value32);
		rgba.fromRGBA(value32);
		//开始编码
		switch(channelAmount){
			case 3:case 4:
				setBufferValue(x,0,rgba.blue);
				setBufferValue(x,1,rgba.green);
				setBufferValue(x,2,rgba.red);
				if(channelAmount>3){
					setBufferValue(x,3,rgba.alpha);
				}
			break;
			default:
				setBufferValue(x,0,colorsList->indexOf(value32));
		}
	}
	encodeLineData();
	return true;
}

//BMP文件
DataBlock::SizeType FileBMP::parseData(){
	uint pos=0;
	//文件头
	subDataBlock(pos,dataLength-pos,fileHeader);
	pos+=fileHeader.parseData();
	//信息头
	subDataBlock(pos,dataLength-pos,infoHeader);
	pos+=infoHeader.parseData();
	//可能存在颜色表
	auto colorCount=colorCountOfColorsList();
	if(colorCount!=0){
		subDataBlock(pos,colorCount*4,bgrasList);
		pos+=bgrasList.parseData();
	}
	//图像数据前的未知数据,可能存在
	uint32 offbits;
	if(fileHeader.getOffBits(offbits) && pos<offbits){
		subDataBlock(pos,offbits-pos,unknownBlockBefore);
		pos=offbits;
	}
	//图像数据
	if(pos<dataLength){
		int32 height;
		if(infoHeader.getHeight(height)){
			FILEBMP_POSITIVE_HEIGHT
			auto imgSize=infoHeader.lineSize()*height;//计算数据大小
			if(pos+imgSize>dataLength)imgSize=dataLength-pos;//可能数据不足
			subDataBlock(pos,imgSize,bitmapData);//确定图像数据
			pos+=imgSize;
		}
	}
	//图像数据后的未知数据,可能存在
	if(pos<dataLength){
		subDataBlock(pos,dataLength-pos,unknownBlockAfter);
	}
	return dataLength;
}
bool FileBMP::isValid_FileSize()const{
	uint32 size;
	return fileHeader.getSize(size)&&size==dataLength;
}
bool FileBMP::isValid_FileOffbits()const{
	uint32 offbits;
	return fileHeader.getOffBits(offbits)&&offbits<dataLength;
}
uint64 FileBMP::colorCountOfColorsList()const{
	uint16 bitCount;
	if(infoHeader.getBitCount(bitCount)){
		if(bitCount==24 || bitCount==32)return 0;//24位和32位没有颜色表
		return pow(2,bitCount);
	}
	return 0;
}

#define FILEBMP_EACH_BLOCK(MACRO) \
	MACRO(fileHeader)\
	MACRO(infoHeader)\
	MACRO(bgrasList)\
	MACRO(unknownBlockBefore)\
	MACRO(bitmapData)\
	MACRO(unknownBlockBefore)\

bool FileBMP::saveFileBMP(const string &filename)const{
	//打开文件
	auto file=fopen(filename.data(),"wb");
	if(!file)return false;
	//写入各个结构
#define FILEBMP_FWRITE(block) \
	block.fileWrite(file);
	FILEBMP_EACH_BLOCK(FILEBMP_FWRITE)
#undef FILEBMP_FWRITE
	//关闭文件
	fflush(file);
	fclose(file);
	return true;
}

bool FileBMP::encodeFrom(const Bitmap_32bit &bitmap,uint16 bitCount,List<uint32> *colorsList){
	//验证
	auto width=bitmap.getWidth(),height=bitmap.getHeight();
	//文件头
	fileHeader.newDataPointer(14);
	fileHeader.setType(FileBMP_FileHeader::Type_BM);
	fileHeader.setReserved1(0);
	fileHeader.setReserved2(0);
	//信息头
	infoHeader.newDataPointer(FileBMP_InfoHeader::Size::BitmapInfoHeader);
	infoHeader.setSize(infoHeader.dataLength);
	infoHeader.setWidth(width);
	infoHeader.setHeight(height);
	infoHeader.setPlanes(1);
	infoHeader.setBitCount(bitCount);
	infoHeader.setCompression(FileBMP_InfoHeader::Compression::NoCompression);
	auto lineSize=infoHeader.lineSize();
	infoHeader.setImageSize(lineSize*height);
	infoHeader.setXPelsPerMeter(0);
	infoHeader.setYPelsPerMeter(0);
	infoHeader.setClrUsed(0);
	infoHeader.setClrImportant(0);
	//颜色表
	auto colorsCount=colorCountOfColorsList();
	bool newList=false;
	if(colorsCount>0){
		//如果没有参考颜色表,则从bitmap推断
		if(!colorsList){
			colorsList=new List<uint32>();
			newList=true;
			bitmap.getColorsList(*colorsList,colorsCount);
		}
		//开始构建颜色表
		bgrasList.newDataPointer(colorsCount*4);
		decltype(colorsCount) i=0;
		for(auto &value:*colorsList){
			if(i>=colorsCount)break;
			rgba.fromRGBA(value);//转换
			bgrasList.setColor(i,rgba);//设置
			++i;
		}
	}
	//扫描线
	FileBMP_Scanline scanLine;
	scanLine.setInfoHeader(infoHeader);
	scanLine.colorsList=colorsList;
	//开始编码
	bitmapData.newDataPointer(lineSize*height);
	SizeType offset;
	for(decltype(height) y=0;y<height;++y){
		offset=lineSize*y;
		bitmapData.subDataBlock(offset,lineSize,scanLine);
		scanLine.encodeLine(height-1-y,bitmap);
	}
	//设置文件大小
	uint32 size=0;
#define FILEBMP_SIZE(block) \
	size+=block.dataLength;
	FILEBMP_EACH_BLOCK(FILEBMP_SIZE)
#undef FILEBMP_SIZE
	fileHeader.setSize(size);
	size-=(unknownBlockAfter.dataLength+bitmapData.dataLength);
	fileHeader.setOffBits(size);
	//收尾
	if(newList){
		delete colorsList;
	}
	return true;
}
bool FileBMP::decodeTo(Bitmap_32bit &bitmap)const{
	//行字节数
	List<uint32> colorsList;
	FileBMP_Scanline scanLine;
	scanLine.setInfoHeader(infoHeader);
	if(bgrasList.dataLength){
		bgrasList.getColorsList(colorsList);
		scanLine.colorsList=&colorsList;
	}
	//创建图像数据
	if(!bitmap.newBitmap(scanLine.width,scanLine.height))return false;
	//开始复制数据
	for(decltype(scanLine.height) y=0;y<scanLine.height;++y){
		bitmapData.subDataBlock(scanLine.lineSize*y,scanLine.lineSize,scanLine);//获取行数据
		scanLine.decodeLine(y,bitmap);//解码到缓冲中
	}
	return true;
}

bool FileBMP::deleteDataPointer(bool deleteAllChunk){
	if(deleteAllChunk){
#define FILEBMP_DELETE(chunk) \
	chunk.deleteDataPointer();
		FILEBMP_EACH_BLOCK(FILEBMP_DELETE)
#undef FILEBMP_DELETE
	}
	return DataBlock::deleteDataPointer();
}