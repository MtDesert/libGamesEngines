#include "FileBMP.h"
#include<cmath>
#include<string>
using namespace std;
#include<stdio.h>

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
DataBlock FileBMP_InfoHeader::unknownBlock()const{
	return DataBlock();
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
uint FileBMP_BGRAsList::colorsCount()const{return dataLength/4;}
bool FileBMP_BGRAsList::getColor(uint index,ColorRGBA &color)const{
	uint32 value;
	if(get_uint32(index*4,value)){
		color=ColorRGBA(value);
		return true;
	}else return false;
}
bool FileBMP_BGRAsList::setColor(uint index,const ColorRGBA &color){
	return set_uint32(index*4,color.toBGRA());
}
bool FileBMP_BGRAsList::setColor(uint index,uint color){
	return set_uint32(index*4,color);
}

DataBlock::SizeType FileBMP::parseData()
{
	uint pos=0;
	//file header
	subDataBlock(pos,dataLength-pos,fileHeader);
	pos+=fileHeader.parseData();
	//info header
	subDataBlock(pos,dataLength-pos,infoHeader);
	pos+=infoHeader.parseData();
	//color list,make sure the color count first
	uint colorCount=colorCountOfColorsList();
	if(colorCount!=0){
		subDataBlock(pos,colorCount*4,colorsList);
		pos+=colorsList.parseData();
	}
	//unknown block before bitmap data
	uint32 offbits;
	if(fileHeader.getOffBits(offbits) && pos<offbits){
		subDataBlock(pos,offbits-pos,unknownBlockBefore);
		pos=offbits;
	}
	//bitmap data
	if(pos<dataLength){
		uint32 imgSize;
		if(infoHeader.getImageSize(imgSize)){
			if(imgSize==0){//ImageSize==0 && compression==NoCompression is legal
				uint32 compression;
				if(infoHeader.getCompression(compression)&&compression==FileBMP_InfoHeader::NoCompression){
					int32 height;
					if(infoHeader.getHeight(height)){
						FILEBMP_POSITIVE_HEIGHT
						imgSize=infoHeader.lineSize()*height;//update imgSize
					}
				}
			}
			if(imgSize>dataLength-pos)imgSize=dataLength-pos;
			subDataBlock(pos,imgSize,bitmapData);
			pos+=imgSize;
		}
	}
	//unknown block after
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
uint FileBMP::colorCountOfColorsList()const{
	uint colorCount=0;
	uint16 bitCount;
	if(infoHeader.getBitCount(bitCount)){
		switch(bitCount){
			case 1:colorCount=2;break;
			case 4:colorCount=16;break;
			case 8:colorCount=256;break;
		}
	}
	return colorCount;
}

bool FileBMP::encodeFrom(const Bitmap_32bit &bitmap,uint16 bitCount,const List<uint32> *pRgbaList){
	//验证
	if(!FileBMP_InfoHeader::isValid_BitCount(bitCount))return false;
	auto width=bitmap.getWidth(),height=bitmap.getHeight();
	auto lineSize=infoHeader.lineSize();
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
	infoHeader.setImageSize(lineSize*height);
	infoHeader.setXPelsPerMeter(0);
	infoHeader.setYPelsPerMeter(0);
	infoHeader.setClrUsed(0);//for 256 colors bitmap, this always 256
	infoHeader.setClrImportant(0);
	//颜色表
	auto colorsCount=colorCountOfColorsList();
	if(colorsCount>0){
		//如果没有参考颜色表,则从bitmap推断
		List<uint32> rgbaList;
		if(!pRgbaList){
			bitmap.getColorsList(rgbaList);
			pRgbaList=&rgbaList;
		}
		//开始构建颜色表
		colorsList.newDataPointer(colorsCount*4);
		decltype(colorsCount) i=0;
		ColorRGBA rgba;
		for(auto &value:*pRgbaList){
			if(i>=colorsCount)break;
			rgba.fromRGBA(value);//转换
			colorsList.setColor(i,rgba);//设置
			++i;
		}
	}
	//开始编码
	bitmapData.newDataPointer(lineSize*height);
	uint32 color32;ColorRGBA rgba;SizeType offset;
	for(decltype(height) y=0;y<height;++y){
		offset=lineSize*y;
		for(decltype(width) x=0;x<width;++x){
			bitmap.getColor(x,y,color32);//获取颜色
			//颜色转换
			if(bitCount==32){
				rgba.fromRGBA(color32);
				color32=rgba.toBGRA();
				bitmapData.set_uint32(offset,color32);
				offset+=4;
			}else if(bitCount==24){
				rgba.fromRGBA(color32);
				bitmapData.set_uint8(offset,rgba.blue);
				bitmapData.set_uint8(offset+1,rgba.green);
				bitmapData.set_uint8(offset+2,rgba.red);
				offset+=3;
			}else{
			}
		}
	}
	return true;
}
bool FileBMP::decodeTo(Bitmap_32bit &bitmap)const{
	//获取尺寸(注意,高度有可能存为负数)
	uint32 width;int32 height;
	if(!infoHeader.getWidth(width)||!infoHeader.getHeight(height))return false;
	FILEBMP_POSITIVE_HEIGHT
	uint16 bitCount;
	if(!infoHeader.getBitCount(bitCount))return false;
	auto lineSize=infoHeader.lineSize();
	if(lineSize==0)return false;
	auto needPalette=(bitCount!=24 && bitCount!=32);
	//创建图像数据
	if(!bitmap.newBitmap(width,height))return false;
	uint8 *pixels=bitmap.dataPointer;uint pixelOffset=0;//dest
	BitBlock lineData;uint lineOffset=0;//src
	if(needPalette)lineData.newDataPointer(lineSize);
	uint32 colorIndex;ColorRGBA colorRGBA;//buffer for palette
	uint8 colorIndex8;
	uint16 colorIndex16;
	//开始复制数据
	for(int y=0;y<height;++y){
		lineOffset=0;
		if(!bitmapData.subDataBlock(lineSize*y,lineSize,lineData,needPalette))break;//get line
		if(needPalette){//the bytes of each line data should be read high bit first, so we need to reverse bit of each byte
			for(uint i=0;i<lineData.dataLength;++i)reverseBit(lineData.dataPointer[i]);
		}
		for(uint x=0;x<width;++x){
			pixelOffset=(y*width+x)*4;
			if(needPalette){
				//get index
				if(bitCount<=8 && lineData.get_Bits8(lineOffset,bitCount,colorIndex8)){
					colorIndex=colorIndex8;
				}else if(bitCount<=16 && lineData.get_Bits16(lineOffset,bitCount,colorIndex16)){
					colorIndex=colorIndex16;
				}else if(bitCount<=32 && lineData.get_Bits32(lineOffset,bitCount,colorIndex));
				else break;
				//get ColorRGBA by index
				if(colorsList.getColor(colorIndex,colorRGBA)){
					pixels[pixelOffset+0]=colorRGBA.red;
					pixels[pixelOffset+1]=colorRGBA.green;
					pixels[pixelOffset+2]=colorRGBA.blue;
					pixels[pixelOffset+3]=colorRGBA.alpha;
				}else break;
			}else{//copy byte directly
				lineData.get_uint8(lineOffset+2,pixels[pixelOffset+0]);//red
				lineData.get_uint8(lineOffset+1,pixels[pixelOffset+1]);//green
				lineData.get_uint8(lineOffset+0,pixels[pixelOffset+2]);//blue
				if(bitCount==24){
					pixels[pixelOffset+3]=0xFF;//alpha
					lineOffset+=3;
				}else{
					lineData.get_uint8(lineOffset+3,pixels[pixelOffset+3]);//alpha
					lineOffset+=4;
				}
			}
		}
	}
	//finish
	if(needPalette)lineData.deleteDataPointer();
	return false;
}