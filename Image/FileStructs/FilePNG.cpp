#include"FilePNG.h"
#include"DataBlock_zlib.h"
#include"FileGZIP.h"

#include"zlib.h"

#include<iostream>
#include<cmath>
#include<string.h>

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
		if(dataLength<8)return subDataBlock(dataLength,0);//no chunk data
		if(dataLength-8<chunkLength)return subDataBlock(8,dataLength-8);//not enough
		return subDataBlock(8,chunkLength);
	}
	return DataBlock();
}
bool FilePNG_Chunk::getChunkCRC(uint32 &value)const{
	uint32 chunkLength;
	if(getChunkLength(chunkLength)&&get_uint32(8+chunkLength,value)){
		reverseByte(value);
		return true;
	}else return false;
}
bool FilePNG_Chunk::setChunkCRC(const uint32 &value){
	uint32 chunkLength;
	if(getChunkLength(chunkLength)){
		uint32 v=value;
		reverseByte(v);
		return set_uint32(8+chunkLength,v);
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

uint32 FilePNG_Chunk::make_CRC()const{
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
		case ColorType_Gray:case ColorType_Palette:
		return 1;
		break;
		case ColorType_GrayAlpha:
		return 2;
		break;
		case ColorType_RGB:
		return 3;
		break;
		case ColorType_RGBAlpha:
		return 4;
		break;
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
bool FilePNG_IHDR::isValid_CompressionMethod(uint8 method){
	switch(method){
		case 0:return true;break;
		default:return false;
	}
}
bool FilePNG_IHDR::isValid_FilterMethod(uint8 method){
	switch(method){
		case 0:return true;break;
		default:return false;
	}
}
bool FilePNG_IHDR::isValid_InterlaceMethod(uint8 method){
	return (method>=InterlaceMethod_None && method<=InterlaceMethod_Adam7);
}

uint FilePNG_PLTE::colorCount()const{
	uint32 len;
	return getChunkLength(len)?len/3:0;
}
bool FilePNG_PLTE::getColor(uint index,ColorRGB &color)const{
	uint offset=8+index*3;
	return get_uint8(offset,color.red)&&get_uint8(offset+1,color.green)&&get_uint8(offset+2,color.blue);
}
bool FilePNG_PLTE::setColor(uint index,const ColorRGB &color){
	int offset=8+index*3;
	return set_uint8(offset,color.red)&&set_uint8(offset+1,color.green)&&set_uint8(offset+2,color.blue);
}
bool FilePNG_PLTE::setColor(uint index,uint color){
	ColorRGB c;
	c.fromRGB(color);
	return setColor(index,c);
}

DataBlock FilePNG_IDAT::getZlibData()const{
	return chunkDataBlock();
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
	colorType(FilePNG_IHDR::ColorType_Gray),bitDepth(0),
	filePng_PLTE(nullptr),filePng_tRNS(nullptr){
	dataPointer=pointer;
}

DataBlock::SizeType FilePNG_Scanline::parseData(){
	BitBlock::parseData();
	switch(colorType){
		case FilePNG_IHDR::ColorType_Gray:{
			uint8 lv=(uint8)pow(2,bitDepth)-1;
			unit=lv?0xFF/lv:0;
		}break;
		case FilePNG_IHDR::ColorType_RGB:;
		case FilePNG_IHDR::ColorType_Palette:;
		case FilePNG_IHDR::ColorType_GrayAlpha:;
		case FilePNG_IHDR::ColorType_RGBAlpha:;
	}
	return 0;
}
ColorRGBA FilePNG_Scanline::getColor4B(uint x)const{
	switch(colorType){
		case FilePNG_IHDR::ColorType_Gray:return getColor4B_Gray(x);break;
		case FilePNG_IHDR::ColorType_RGB:return getColor4B_RGB(x);break;
		case FilePNG_IHDR::ColorType_Palette:return getColor4B_Palette(x);break;
		case FilePNG_IHDR::ColorType_GrayAlpha:return getColor4B_GrayAlpha(x);break;
		case FilePNG_IHDR::ColorType_RGBAlpha:return getColor4B_RGBAlpha(x);break;
		default:return ColorRGBA();
	};
}
ColorRGBA FilePNG_Scanline::getColor4B_Gray(uint x)const{
	uint8 val;
	if(get_Bits8(x*bitDepth,bitDepth,val)){
		val*=unit;
		return ColorRGBA(val,val,val);
	}
	return ColorRGBA();
}
ColorRGBA FilePNG_Scanline::getColor4B_RGB(uint x)const{
	if(bitDepth==8){
		uint offset=3*x;
		uint8 r,g,b;
		if(get_uint8(offset,r)&&get_uint8(offset+1,g)&&get_uint8(offset+2,b)){
			return ColorRGBA(r,g,b);
		}
	}else if(bitDepth==16){
		uint offset=6*x;
		uint8 r,g,b;
		if(get_uint8(offset,r)&&get_uint8(offset+2,g)&&get_uint8(offset+4,b)){
			return ColorRGBA(r,g,b);
		}
	}
	return ColorRGBA();
}
ColorRGBA FilePNG_Scanline::getColor4B_Palette(uint x)const{
	if(filePng_PLTE){
		uint16 val;
		if(get_Bits16(x*bitDepth,bitDepth,val)){
			ColorRGB color3B;
			if(filePng_PLTE->getColor(val,color3B)){
				ColorRGBA color4B(color3B.red,color3B.green,color3B.blue);
				if(filePng_tRNS){
					filePng_tRNS->getAlpha(val,color4B.alpha);
				}
				return color4B;
			}
		}
	}
	return ColorRGBA();
}
ColorRGBA FilePNG_Scanline::getColor4B_GrayAlpha(uint x)const{
	if(bitDepth==8){
		uint offset=2*x;
		uint8 gray,alpha;
		if(get_uint8(offset,gray)&&get_uint8(offset+1,alpha)){
			return ColorRGBA(gray,gray,gray,alpha);
		}
	}else if(bitDepth==16){
		uint offset=4*x;
		uint8 gray,alpha;
		if(get_uint8(offset,gray)&&get_uint8(offset+2,alpha)){
			return ColorRGBA(gray,gray,gray,alpha);
		}
	}
	return ColorRGBA();
}
ColorRGBA FilePNG_Scanline::getColor4B_RGBAlpha(uint x)const{
	if(bitDepth==8){
		uint offset=4*x;
		uint8 r,g,b,a;
		if(get_uint8(offset,r)&&get_uint8(offset+1,g)&&get_uint8(offset+2,b)&&get_uint8(offset+3,a)){
			return ColorRGBA(r,g,b,a);
		}
	}else if(bitDepth==16){
		uint offset=8*x;
		uint8 r,g,b,a;
		if(get_uint8(offset,r)&&get_uint8(offset+2,g)&&get_uint8(offset+4,b)&&get_uint8(offset+6,a)){
			return ColorRGBA(r,g,b,a);
		}
	}
	return ColorRGBA();
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
	DataBlock::reset();
	for(auto chunk:allChunks){
		if(chunk)delete chunk;
	}
	allChunks.clear();
	return 0;
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
	for(auto chunk:allChunks){
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

DataBlock FilePNG::decode_contactIDATs()const{
	//find all IDAT
	uint32 idatSize=0,len;
	for(FilePNG_Chunk *chunk:allChunks){
		if(chunk && chunk->chunkName()=="IDAT" && chunk->getChunkLength(len)){
			idatSize+=len;
		}
	}
	//allocate memory
	DataBlock idat;
	if(idat.newDataPointer(idatSize)){
		uint pos=0;
		//copy all data
		for(FilePNG_Chunk *chunk:allChunks){
			if(chunk && chunk->chunkName()=="IDAT" && chunk->getChunkLength(len)){
				auto buffer=idat.subDataBlock(pos,len);
				auto chunkBlock=chunk->chunkDataBlock();
				chunkBlock.subDataBlock(0,len,buffer,true);
				pos+=len;
			}
		}
	}
	return idat;
}
DataBlock_deflate FilePNG::decode_getDeflateFromIDATs(const DataBlock &idatBlock)const{
	DataBlock_deflate deflateData;
	bool formatOK=false;

	//check whether zlib format
	if(idatBlock.dataLength>=2){
		DataBlock_zlib zlibBlock;
		idatBlock.subDataBlock(0,idatBlock.dataLength,zlibBlock);
		zlibBlock.bitLength=zlibBlock.dataLength*8;
		bool isZlibHeader=
			zlibBlock.isValid_CompressionMethod()&&
			zlibBlock.isValid_CompressionInfo()&&
			zlibBlock.isValid_FlagCheck();
		if(isZlibHeader){
			deflateData=zlibBlock.deflateDataBlock();
			formatOK=true;
		}
	}
	//check whether gzip format
	if(!formatOK){
		FileGZIP fileGZIP;
		fileGZIP.DataBlock::operator=(idatBlock);
		if(fileGZIP.isValid_Header()){
			deflateData=fileGZIP.deflateData();
			formatOK=true;
		}
	}
	//check whether deflate block
	if(!formatOK){
		deflateData.DataBlock::operator=(idatBlock);
		formatOK=true;
	}
	return deflateData;
}
DataBlock FilePNG::decode_uncompressDeflate(DataBlock_deflate &deflateData)const{
	//decode
	deflateData.parseData();
	return deflateData.inflate();
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
	BitBlock line;
	FilePNG_Scanline scanLine;
	scanLine.colorType=(FilePNG_IHDR::ColorType)colorType;
	scanLine.bitDepth=bitDepth;
	if(scanLine.colorType==FilePNG_IHDR::ColorType_Palette){
		scanLine.filePng_PLTE=findPLTE();
		scanLine.filePng_tRNS=findtRNS();
	}
	scanLine.parseData();
	//copy data
	uint32 value;
	for(uint32 y=0;y<height;++y){
		filterBlock.subDataBlock(lineSize*y,lineSize,line);
		line.subDataBlock(1,lineSize-1,scanLine);
		for(uint32 x=0;x<width;++x){
			value=scanLine.getColor4B(x).toRGBA();
			bitmap.setColor(x,y,value);
		}
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
	BitBlock line;
	FilePNG_Scanline scanLine;
	scanLine.colorType=(FilePNG_IHDR::ColorType)colorType;
	scanLine.bitDepth=bitDepth;
	if(scanLine.colorType==FilePNG_IHDR::ColorType_Palette){
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
				value=scanLine.getColor4B(x).toRGBA();
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

bool FilePNG::decodeTo(Bitmap_32bit &bitmap)const{
	//if no IHDR,we can not decided how to decode
	auto ihdr=findIHDR();
	if(!ihdr)return false;
	//get block
	auto allIDATs=decode_contactIDATs();
	auto deflateBlock=decode_getDeflateFromIDATs(allIDATs);
	auto filterBlock=decode_uncompressDeflate(deflateBlock);
	allIDATs.deleteDataPointer();
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
