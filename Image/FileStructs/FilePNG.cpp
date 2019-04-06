#include"FilePNG.h"
#include"DataBlock_zlib.h"
#include"FileGZIP.h"
#include"Number.h"

#include"zlib.h"

#include<iostream>
#include<cmath>
#include<string.h>

//常量宏
#define DEFAULT_CHUNK_SIZE 12
#define CHUNK_DATA_START 8
#define DEFAULT_IHDR_SIZE 13
#define RGB_SIZE 3
#define RGBA_SIZE 4

//静态缓冲区
static uint offset;
static uint8 value8;
static uint16 value16;
static uint32 value32;
//static uint64 value64;
ColorRGB rgb;
ColorRGBA rgba;

DataBlock::SizeType FilePNG_Chunk::parseData(){
	uint32 chunkLength;
	if(getChunkLength(chunkLength)){
		chunkLength=8+chunkLength+4;
		if(dataLength>chunkLength)dataLength=chunkLength;
	}
	return dataLength;
}

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_Chunk,ChunkLength,uint32,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_Chunk,ChunkType,uint32,4,true)
DataBlock FilePNG_Chunk::chunkDataBlock()const{
	uint32 chunkLength;
	if(getChunkLength(chunkLength)){
		if(dataLength<CHUNK_DATA_START)return subDataBlock(dataLength,0);//no chunk data
		if(dataLength-CHUNK_DATA_START<chunkLength)return subDataBlock(CHUNK_DATA_START,dataLength-CHUNK_DATA_START);//not enough
		return subDataBlock(CHUNK_DATA_START,chunkLength);
	}
	return DataBlock();
}
bool FilePNG_Chunk::getChunkCRC(uint32 &value)const{
	uint32 chunkLength;
	if(getChunkLength(chunkLength)&&get_uint32(CHUNK_DATA_START+chunkLength,value)){
		reverseByte(value);
		return true;
	}else return false;
}
bool FilePNG_Chunk::setChunkCRC(const uint32 &value){
	uint32 chunkLength;
	if(getChunkLength(chunkLength)){
		uint32 v=value;
		reverseByte(v);
		return set_uint32(CHUNK_DATA_START+chunkLength,v);
	}else return false;
}

string FilePNG_Chunk::chunkName()const{
	uint32 type;
	if(getChunkType(type)){
		return string((const char*)&type,4);
	}else return string();
}
void FilePNG_Chunk::setChunkName(const string &name){
	if(name.length()!=4)return;
	uint32 *u32=(uint32*)name.data();
	setChunkType(*u32);
}

DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_Chunk,AncillaryBit,4,5)
DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_Chunk,PrivateBit,5,5)
DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_Chunk,ReserveBit,6,5)
DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_Chunk,SafeToCopyBit,7,5)

uint32 FilePNG_Chunk::makeCRC()const{
	uint32 length;
	if(getChunkLength(length)){
		auto block=subDataBlock(4,4+length);
		return crc32(0,block.dataPointer,block.dataLength);
	}
	return 0;
}

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,Width,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,Height,uint32,12,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,BitDepth,uint8,16,true)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,ColorType,uint8,17,true)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,CompressionMethod,uint8,18,true)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,FilterMethod,uint8,19,true)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_IHDR,InterlaceMethod,uint8,20,true)

DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_IHDR,ColorMask_Palette,17,0)
DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_IHDR,ColorMask_Color,17,1)
DATABLOCK_CUSTOM_BOOL_CPP(FilePNG_IHDR,ColorMask_Alpha,17,2)

//根据像素深度和宽度,计算出存储一行所需的字节数
uint FilePNG_rowBytes(uint pixelDepth,uint width,bool withFilterMethod){
	uint rowBits=pixelDepth*width;
	uint rowBytes=rowBits/8 + (rowBits%8?1:0);
	if(withFilterMethod)rowBytes+=1;
	return rowBytes;
}
uint8 FilePNG_PaethPredictor(uint8 left,uint8 above,uint8 upperLeft){
	int16 p = left + above - upperLeft;
	int16 pa=abs(p - left);
	int16 pb=abs(p - above);
	int16 pc=abs(p - upperLeft);
	if(pa <= pb && pa <= pc )return left;
	else if(pb <= pc)return above;
	else return upperLeft;
}

uint FilePNG_IHDR::channels(uint colorType){
	switch(colorType){
		case ColorType_Gray:
		case ColorType_Palette:return 1;
		case ColorType_GrayAlpha:return 2;
		case ColorType_RGB:return 3;
		case ColorType_RGBAlpha:return 4;
		//特殊格式
		case ColorType_PaletteGray:
		case ColorType_PaletteGrayAlpha:
		case ColorType_PaletteRGBAlpha:
			return 1;
		default:return 0;
	}
}
uint FilePNG_IHDR::pixelDepth(uint8 bitDepth,uint8 colorType){return bitDepth*channels(colorType);}
uint FilePNG_IHDR::rowBytes(uint8 width, uint8 bitDepth, uint8 colorType, bool withFilterMethod){
	return FilePNG_rowBytes(pixelDepth(bitDepth,colorType),width,withFilterMethod);
}

bool FilePNG_IHDR::isValid_BitDepth(uint8 colorType, uint8 bitDepth){
	switch(colorType){
		case ColorType_Gray:{
			switch(bitDepth){case 1:case 2:case 4:case 8:case 16:return true;
				default:return false;}
		}break;
		case ColorType_RGB:case ColorType_GrayAlpha:case ColorType_RGBAlpha:{
			switch(bitDepth){case 8:case 16:return true;
				default:return false;}
		}break;
		case ColorType_Palette:{
			switch(bitDepth){case 1:case 2:case 4:case 8:return true;
				default:return false;}
		}break;
		default:return false;
	}
}
bool FilePNG_IHDR::isValid_ColorType(uint8 type){
	switch(type){
		case ColorType_Gray:
		case ColorType_RGB:
		case ColorType_Palette:
		case ColorType_GrayAlpha:
		case ColorType_RGBAlpha:
		return true;break;
		default:return false;
	}
}
bool FilePNG_IHDR::isValid_CompressionMethod(uint8 method){return method==0;}
bool FilePNG_IHDR::isValid_FilterMethod(uint8 method){return method==0;}
bool FilePNG_IHDR::isValid_InterlaceMethod(uint8 method){
	return (method>=InterlaceMethod_None && method<=InterlaceMethod_Adam7);
}

uint FilePNG_PLTE::rgbCount()const{
	uint32 len;
	return getChunkLength(len)?len/3:0;
}
bool FilePNG_PLTE::getColor(uint index,ColorRGB &color)const{
	offset=CHUNK_DATA_START+index*3;
	return get_uint8(offset,color.red)&&get_uint8(offset+1,color.green)&&get_uint8(offset+2,color.blue);
}
bool FilePNG_PLTE::setColor(uint index,const ColorRGB &color){
	offset=CHUNK_DATA_START+index*3;
	return set_uint8(offset,color.red)&&set_uint8(offset+1,color.green)&&set_uint8(offset+2,color.blue);
}
bool FilePNG_PLTE::setColor(uint index,uint32 color){
	rgb.fromRGB(color);
	return setColor(index,rgb);
}

bool FilePNG_PLTE::getGray(uint index,uint8 &gray) const{return get_uint8(CHUNK_DATA_START+index,gray);}
bool FilePNG_PLTE::setGray(uint index,const uint8 &gray){return set_uint8(CHUNK_DATA_START+index,gray);}

bool FilePNG_PLTE::getGrayAlpha(uint index,uint16 &grayAlpha)const{return get_uint16(CHUNK_DATA_START+index*2,grayAlpha);}
bool FilePNG_PLTE::setGrayAlpha(uint index,const uint16 &grayAlpha){return set_uint16(CHUNK_DATA_START+index*2,grayAlpha);}

bool FilePNG_PLTE::getRGBA(uint index,ColorRGBA &color)const{
	if(getRGBA(index,value32)){
		color.fromRGBA(value32);
		return true;
	}
	return false;
}
bool FilePNG_PLTE::getRGBA(uint index,uint32 &color)const{
	offset=CHUNK_DATA_START+index*4;
	return get_uint32(offset,color);
}
bool FilePNG_PLTE::setRGBA(uint index,const ColorRGBA &color){
	offset=CHUNK_DATA_START+index*4;
	value32=color.toRGBA();
	return set_uint32(offset,value32);
}
bool FilePNG_PLTE::setRGBA(uint index,uint32 &color){
	rgba.fromRGBA(color);
	return setRGBA(index,rgba);
}

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_tRNS,Gray,uint16,8,false)
uint FilePNG_tRNS::alphaCount()const{
	uint32 len;
	return getChunkLength(len)?len:0;
}
bool FilePNG_tRNS::getAlpha(uint index,uint8 &alpha)const{
	return chunkDataBlock().get_uint8(index,alpha);
}
bool FilePNG_tRNS::setAlpha(uint index,const uint8 &alpha){
	return chunkDataBlock().set_uint8(index,alpha);
}
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_tRNS,Red,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_tRNS,Green,uint16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_tRNS,Blue,uint16,12,false)

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_gAMA,Gama,uint32,8,false)

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,WhitePointX,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,WhitePointY,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,RedX,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,RedY,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,GreenX,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,GreenY,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,BlueX,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_cHRM,BlueY,uint32,8,false)

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG_sRGB,RenderingIntent,uint8,8,false)

#define FILEPNG_CPP_PUTCHUNK(chunkType)\
	if(name==#chunkType){\
	pChunk=new FilePNG_##chunkType;\
	}else

FilePNG_Scanline::FilePNG_Scanline(uchar *pointer):
	bitDepth(0),colorType(0),channelAmount(0),pixelDepth(0),
	hasPalette(false),hasColor(false),hasAlpha(false),
	filePng_PLTE(nullptr),filePng_tRNS(nullptr),
	leastUint(0),
	buffer8(nullptr),buffer16(nullptr),buffer32(nullptr),buffer64(nullptr)
{
	dataPointer=pointer;
}

void FilePNG_Scanline::setIHDR(const FilePNG_IHDR &ihdr){
	//获取头部各个属性
	ihdr.getWidth(width);
	ihdr.getBitDepth(bitDepth);
	ihdr.getColorType(colorType);
	channelAmount=FilePNG_IHDR::channels(colorType);
	pixelDepth=bitDepth*channelAmount;
	ihdr.getColorMask_Palette(hasPalette);
	ihdr.getColorMask_Color(hasColor);
	ihdr.getColorMask_Alpha(hasAlpha);
	//确定精度
	precision=pow(2,bitDepth)-1;
	//申请缓冲区
	leastUint=leastUintToStoreBit(bitDepth);
	deleteBuffer();
	newBuffer(leastUint*channelAmount*width);
}

bool FilePNG_Scanline::decodeLine(uint y,Bitmap_32bit &bitmap)const{
	//参数检查
	if(bitDepth==0)return false;//位深肯定不能为0
	//开始转换
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:toUint##bit##Array(bitDepth,channelAmount*width,false,buffer##bit);break;
		CASE_ARRAY(1,8)
		CASE_ARRAY(2,16)
		CASE_ARRAY(4,32)
		CASE_ARRAY(8,64)
#undef CASE_ARRAY
	}
	//取值
	for(decltype(width) x=0;x<width;++x){
		//判断格式
		if(hasPalette && filePng_PLTE){
			auto index=getBufferValue(x,0);//索引值
			if(hasColor){//彩色图
				if(hasAlpha){//非标准格式
					filePng_PLTE->getRGBA(index,rgba);
				}else{//标准格式之一
					if(filePng_PLTE->getColor(index,rgb)){
						rgba=rgb;
						//可能有透明度
						if(filePng_tRNS && filePng_tRNS->getAlpha(index,rgba.alpha));
						else rgba.alpha=0xFF;
					}
				}
			}else{//黑白图
				if(hasAlpha){//非标准格式
					if(filePng_PLTE->getGrayAlpha(index,value16)){
						rgba.red=rgba.green=rgba.blue=value16&0xFF;
						rgba.alpha=value16>>8;
					}
				}else{//非标准格式
					if(filePng_PLTE->getGray(index,value8)){
						rgba.red=rgba.green=rgba.blue=value8;
						//可能有透明度
						if(filePng_tRNS && filePng_tRNS->getAlpha(index,rgba.alpha));
						else rgba.alpha=0xFF;
					}
				}
			}
		}else{
			if(hasColor){
				rgba.red=precisionQuantization(getBufferValue(x,0));
				rgba.green=precisionQuantization(getBufferValue(x,1));
				rgba.blue=precisionQuantization(getBufferValue(x,2));
				rgba.alpha=hasAlpha?
					precisionQuantization(getBufferValue(x,3)):0xFF;
			}else{
				rgba.red=precisionQuantization(getBufferValue(x,0));
				rgba.blue=rgba.green=rgba.red;
				rgba.alpha=hasAlpha?
					precisionQuantization(getBufferValue(x,1)):0xFF;
			}
		}
		//保存像素值
		value32=rgba.toRGBA();
		bitmap.setColor(x,y,value32);
	}
	return true;
}

uint8 FilePNG_Scanline::precisionQuantization(decltype(precision) value)const{
	return Number::devideRound(value*0xFF,precision);
}
void FilePNG_Scanline::newBuffer(SizeType size){
	buffer8=new uint8[size];
	buffer16=reinterpret_cast<uint16*>(buffer8);
}
void FilePNG_Scanline::deleteBuffer(){
	delete []buffer8;
	buffer8=nullptr;
}
uint64 FilePNG_Scanline::getBufferValue(uint x, uint8 channel)const{
	switch(leastUint){
#define CASE_ARRAY(byte,bit)\
		case byte:return buffer##bit[x*channelAmount+channel];break;
		CASE_ARRAY(1,8)
		CASE_ARRAY(2,16)
		CASE_ARRAY(4,32)
		CASE_ARRAY(8,64)
#undef CASE_ARRAY
	}
	return 0;
}

FilePNG_SubImage::FilePNG_SubImage(uint width, uint height, uint pixelDepth):
	width(width),height(height),pixelDepth(pixelDepth){
	dataLength=FilePNG_rowBytes(pixelDepth,width,true)*height;
}
void FilePNG_SubImage::filter(){
	auto bpp=pixelDepth;//byte per pixel
	bpp = bpp/8 + (bpp%8?1:0);//round up to 1
	//filter each line
	uint lineSize=FilePNG_rowBytes(pixelDepth,width,true);
	uchar *data=nullptr;//current line
	uchar *last=nullptr;//last line
	for(uint y=0;y<height;++y){
		last=data;
		data=&dataPointer[lineSize*y];
		switch(data[0]){
			case FilterMethod_None:break;
			case FilterMethod_Sub:
				for(uint i=1+bpp;i<lineSize;++i){
					data[i] += data[i-bpp];
				}
			break;
			case FilterMethod_Up:
				if(last){
					for(uint i=1;i<lineSize;++i){
						data[i] += last[i];
					}
				}
			break;
			case FilterMethod_Average:
				for(uint i=1;i<lineSize;++i){
					data[i]+=((i<1+bpp?0:data[i-bpp]) + (y<=0?0:last[i]))/2;
				}
			break;
			case FilterMethod_Paeth:
				for(uint i=1;i<lineSize;++i){
					data[i]+=FilePNG_PaethPredictor(
					i<1+bpp?0:data[i-bpp],
					y<=0?0:last[i],
					i<1+bpp?0:last[i-bpp]);
				}
			break;
		}
	}
}

DataBlock::SizeType FilePNG::parseData(){
	//check header
	if(dataLength<8)return 0;
	uint pos=8;
	FilePNG_Chunk chunk;
	//scan all chunks
	while(pos<dataLength){
		subDataBlock(pos,dataLength-pos,chunk);
		pos+=chunk.parseData();

		//check which chunk it is
		FilePNG_Chunk *pChunk=nullptr;
		string name=chunk.chunkName();
		FILEPNG_CHUNK_MACRO(FILEPNG_CPP_PUTCHUNK)
		{//this is unknown block
			pChunk=new FilePNG_Chunk(chunk);
		}
		chunk.subDataBlock(0,chunk.dataLength,*pChunk);//pass self
		allChunks.push_back(pChunk);
	}
	return dataLength;
}
DataBlock::SizeType FilePNG::reset(){
	for(auto chunk:allChunks){
		if(chunk)delete chunk;
	}
	allChunks.clear();
	deleteDataPointer();
	return DataBlock::reset();;
}
bool FilePNG::saveFilePNG(const string &filename)const{
	//open file
	auto file=fopen(filename.data(),"wb");
	if(!file)return false;
	//write signature
	uint64 signature;
	if(!getSignature(signature))return false;
	fwrite(&signature,sizeof(signature),1,file);
	//write each chunk
	for(auto &chunk:allChunks){
		fwrite(chunk->dataPointer,chunk->dataLength,1,file);
	}
	//close file
	fflush(file);
	fclose(file);
	return true;
}

DATABLOCK_CUSTOM_TYPE_CPP(FilePNG,Signature,uint64,0,true)

FilePNG_Chunk* FilePNG::findChunk(const string &name)const{
	for(auto chunk:allChunks){
		if(chunk->chunkName()==name)return chunk;
	}
	return nullptr;
}
#define FILEPNG_FINDCHUNK_CPP(name)\
FilePNG_##name* FilePNG::find##name()const{\
	FilePNG_##name *p=nullptr;\
	for(auto chunk:allChunks){\
	p=dynamic_cast<FilePNG_##name*>(chunk);\
	if(p)break;\
	}\
	return p;\
}
FILEPNG_FINDCHUNK_CPP(IHDR)
FILEPNG_FINDCHUNK_CPP(PLTE)
FILEPNG_FINDCHUNK_CPP(tRNS)

bool FilePNG::isValid_Signature()const{
	uint64 signature;
	return getSignature(signature)&&signature==make_Signature();
}
uint64 FilePNG::make_Signature(){return 0x0A1A0A0D474E5089;}

DataBlock FilePNG::encode_makeFilterBlock(const Bitmap_32bit &bitmap,const FilePNG_IHDR &ihdr,List<uint32> *colorsList){
	DataBlock filterBlock;
	//计算需要存储的数据量
	uint8 bitDepth;uint8 colorType;
	if(!ihdr.getBitDepth(bitDepth) || !ihdr.getColorType(colorType))return filterBlock;
	//计算一行所需的字节数,并申请空间
	auto width=bitmap.getWidth(),height=bitmap.getHeight();
	auto lineSize=FilePNG_IHDR::rowBytes(width,bitDepth,colorType,true);
	filterBlock.newDataPointer(lineSize*height);
	//开始对数据进行编码
	for(decltype(height) y=0;y<height;++y){
	}
	return filterBlock;
}

DataBlock FilePNG::decode_allIDATs()const{
	List<DataBlock> allIDATs;
	for(FilePNG_Chunk *chunk:allChunks){
		if(chunk && chunk->chunkName()=="IDAT"){
			allIDATs.push_back(chunk->chunkDataBlock());
		}
	}
	return DataBlock_deflate::inflate(allIDATs);
}

//interlace module
static const uint wStart[]={0,4,0,2,0,1,0};
static const uint wInterval[]={8,8,4,4,2,2,1};
static const uint hStart[]={0,0,4,0,2,0,1};
static const uint hInterval[]={8,8,8,4,4,2,2};
static uint adam7Width(uint w,uint pass){return w/wInterval[pass] + (w%wInterval[pass] > wStart[pass]?1:0);}
static uint adam7Height(uint h,uint pass){return h/hInterval[pass] + (h%hInterval[pass] > hStart[pass]?1:0);}

bool FilePNG::decode_InterlaceNone(DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const{
	uint32 width,height;
	uint8 bitDepth,colorType;
	if(!ihdr.getWidth(width))return false;
	if(!ihdr.getHeight(height))return false;
	if(!ihdr.getBitDepth(bitDepth))return false;
	if(!ihdr.getColorType(colorType))return false;
	//check
	if(filterBlock.dataLength<ihdr.rowBytes(width,bitDepth,colorType,true)*height)return false;//not enough
	//parse
	FilePNG_SubImage subImage(width,height,ihdr.pixelDepth(bitDepth,colorType));
	if(filterBlock.subDataBlock(0,filterBlock.dataLength,subImage)){
		subImage.filter();
		return true;
	}else return false;
}
bool FilePNG::decode_InterlaceAdam7(DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const{
	uint32 width,height;
	uint8 bitDepth,colorType;
	if(!ihdr.getWidth(width))return false;
	if(!ihdr.getHeight(height))return false;
	if(!ihdr.getBitDepth(bitDepth))return false;
	if(!ihdr.getColorType(colorType))return false;

	uint32 ww,hh;
	uint pos=0;
	for(uint pass=0;pass<7;++pass){
		ww = adam7Width(width,pass);
		hh = adam7Height(height,pass);
		//interlace
		FilePNG_SubImage subImage(ww,hh,ihdr.pixelDepth(bitDepth,colorType));
		if(filterBlock.subDataBlock(pos,filterBlock.dataLength-pos,subImage)){
			if(pos<filterBlock.dataLength){
				subImage.filter();
			}else return false;
			pos+=subImage.dataLength;
		}
	}
	return true;
}
bool FilePNG::decode_makeBitmap(Bitmap_32bit &bitmap,const DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const{
	//获取头部属性
	uint32 width,height;
	uint8 bitDepth,colorType;
	if(!ihdr.getWidth(width))return false;
	if(!ihdr.getHeight(height))return false;
	if(!ihdr.getBitDepth(bitDepth))return false;
	if(!ihdr.getColorType(colorType))return false;
	//确定数据块的数据量
	uint lineSize=ihdr.rowBytes(width,bitDepth,colorType,true);
	if(filterBlock.dataLength<lineSize*height)return false;//不足
	//申请空间
	bitmap.newBitmap(width,height);
	bitmap.coordinateType=Bitmap_32bit::CoordinateType_Screen;//PNG使用屏幕坐标系
	//设置扫描线信息,准备解码
	FilePNG_Scanline scanLine;
	scanLine.setIHDR(ihdr);
	if(scanLine.hasPalette){
		scanLine.filePng_PLTE=findPLTE();
		scanLine.filePng_tRNS=findtRNS();
	}
	//设定缓冲
	for(uint32 y=0;y<height;++y){
		filterBlock.subDataBlock(lineSize*y,lineSize,scanLine);//获取行数据
		scanLine.subDataBlock(1,lineSize-1,scanLine);//去掉filter字节
		scanLine.decodeLine(y,bitmap);//解码到缓冲中
	}
	return true;
}
bool FilePNG::decode_makeBitmapAdam7(Bitmap_32bit &bitmap,const DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const{
	uint32 width,height;
	uint8 bitDepth,colorType;
	if(!ihdr.getWidth(width))return false;
	if(!ihdr.getHeight(height))return false;
	if(!ihdr.getBitDepth(bitDepth))return false;
	if(!ihdr.getColorType(colorType))return false;

	uint lineSize=ihdr.rowBytes(width,bitDepth,colorType,true);
	if(filterBlock.dataLength<lineSize*height)return false;//not enough

	//allocate memory
	bitmap.newBitmap(width,height);
	bitmap.coordinateType=Bitmap_32bit::CoordinateType_Screen;
	//decode bitmap
	DataBlock line;
	FilePNG_Scanline scanLine;
	scanLine.setIHDR(ihdr);
	if(scanLine.hasPalette){
		scanLine.filePng_PLTE=findPLTE();
		scanLine.filePng_tRNS=findtRNS();
	}
	scanLine.parseData();
	//copy data
	uint ww,hh,pos=0;
	uint32 value;
	for(uint pass=0;pass<7;++pass){
		ww = adam7Width(width,pass);
		hh = adam7Height(height,pass);
		uint lineSize=FilePNG_rowBytes(bitDepth,ww,true);
		//copy from image
		for(uint32 y=0;y<hh;++y){
			filterBlock.subDataBlock(pos+lineSize*y,lineSize,line);
			line.subDataBlock(1,lineSize-1,scanLine);
			for(uint32 x=0;x<ww;++x){
				//value=scanLine.getColor4B(x).toRGBA();
				bitmap.setColor(
					wStart[pass]+wInterval[pass]*x,
					hStart[pass]+hInterval[pass]*y,
					value);
			}
		}
		//next pass
		pos+=lineSize*hh;
	}
	return false;
}

bool FilePNG::encodeFrom(const Bitmap_32bit &bitmap,uint8 bitDepth,bool hasPalette,bool hasColor,bool hasAlpha,List<uint32> *colorsList){
	newDataPointer(8);
	setSignature(make_Signature());
	//IHDR
	auto ihdr=new FilePNG_IHDR();
	ihdr->newDataPointer(DEFAULT_CHUNK_SIZE+DEFAULT_IHDR_SIZE);
	ihdr->setChunkLength(DEFAULT_IHDR_SIZE);
	ihdr->setChunkName("IDAT");
	ihdr->setWidth(bitmap.getWidth());
	ihdr->setHeight(bitmap.getHeight());
	ihdr->setBitDepth(bitDepth);
	ihdr->setColorMask_Palette(hasPalette);
	ihdr->setColorMask_Color(hasColor);
	ihdr->setColorMask_Alpha(hasAlpha);
	ihdr->setCompressionMethod(0);
	ihdr->setFilterMethod(0);
	ihdr->setInterlaceMethod(0);
	//PLTE
	FilePNG_PLTE *plte=nullptr;
	if(hasPalette){//需要调色板
		if(!colorsList){//如果没有参考调色板,则自己生成
			colorsList=new List<uint32>();
			bitmap.getColorsList(*colorsList);
		}
		//生成PLTE
		plte=new FilePNG_PLTE();
		auto colorsAmount=colorsList->size();
		plte->newDataPointer(DEFAULT_CHUNK_SIZE+colorsAmount*3);
		plte->setChunkLength(colorsAmount*3);
		plte->setChunkName("PLTE");
		//构建列表
		auto idx=0;
		for(auto &color32:*colorsList){
			plte->setColor(idx,color32);
			++idx;
		}
	}
	//IDAT
	auto idat=new FilePNG_IDAT();
	idat->newDataPointer(DEFAULT_CHUNK_SIZE);
	idat->setChunkLength(0);
	idat->setChunkName("IDAT");
	//IEND
	auto iend=new FilePNG_IEND();
	iend->newDataPointer(DEFAULT_CHUNK_SIZE);
	iend->setChunkLength(0);
	iend->setChunkName("IEND");
	//保存所有chunk
	allChunks.push_back(ihdr);
	if(plte)allChunks.push_back(plte);
	allChunks.push_back(idat);
	allChunks.push_back(iend);
	//并计算CRC值
	for(auto &chunk:allChunks){
		chunk->makeCRC();
	}
	return true;
}
bool FilePNG::decodeTo(Bitmap_32bit &bitmap)const{
	//必须得有IHDR,否则我们无法得知怎么解码
	auto ihdr=findIHDR();
	if(!ihdr)return false;
	//找出所有IDAT数据块并解压
	auto filterBlock=decode_allIDATs();
	//interlace
	uint8 method;
	if(ihdr->getInterlaceMethod(method)){
		if(method==FilePNG_IHDR::InterlaceMethod_None){
			decode_InterlaceNone(filterBlock,*ihdr);
			decode_makeBitmap(bitmap,filterBlock,*ihdr);
		}else if(method==FilePNG_IHDR::InterlaceMethod_Adam7){
			decode_InterlaceAdam7(filterBlock,*ihdr);
			decode_makeBitmapAdam7(bitmap,filterBlock,*ihdr);
		}else return false;
	}else return false;
	//end
	return true;
}