#ifndef FILEBMP_H
#define FILEBMP_H

#include"BitBlock.h"
#include"ColorRGB.h"
#include"Bitmap.h"
#include<string>
using namespace std;

struct FileBMP_FileHeader:public DataBlock//14 bytes
{
	SizeType parseData();

	DATABLOCK_CUSTOM_TYPE(Type,uint16)//always "BM"
	DATABLOCK_CUSTOM_TYPE(Size,uint32)//file size
	DATABLOCK_CUSTOM_TYPE(Reserved1,uint16)//always 0
	DATABLOCK_CUSTOM_TYPE(Reserved2,uint16)//always 0
	DATABLOCK_CUSTOM_TYPE(OffBits,uint32)//where BMP data start
	enum Type{
		Type_BM=0x4D42,//Windows Bitmap
		Type_BA=0x4142,//OS/2 Bitmap Array
		Type_CI=0x4943,//OS/2 Color Icon
		Type_CP=0x5043,//OS/2 Color Pointer
		Type_IC=0x4349,//OS/2 Icon
		Type_PT=0x5450//OS/2 Pointer
	};

	//validation
	static bool isValid_Type(uint16 type);
	static bool isValid_Reserved1(uint16 value);
	static bool isValid_Reserved2(uint16 value);
};
struct FileBMP_InfoHeader:public DataBlock//40 bytes at least. See getSize()
{
	SizeType parseData();

	DATABLOCK_CUSTOM_TYPE(Size,uint32)//See enum Size
	DATABLOCK_CUSTOM_TYPE(Width,uint32)
	DATABLOCK_CUSTOM_TYPE(Height,int32)
	DATABLOCK_CUSTOM_TYPE(Planes,uint16)//always 1
	DATABLOCK_CUSTOM_TYPE(BitCount,uint16)//range:1,4,8,16,24,32
	DATABLOCK_CUSTOM_TYPE(Compression,uint32)//0(NoCompression),1(BI_RLE8),2(BI_RLE4)
	DATABLOCK_CUSTOM_TYPE(ImageSize,uint32)//size of BMP data part
	DATABLOCK_CUSTOM_TYPE(XPelsPerMeter,uint32)
	DATABLOCK_CUSTOM_TYPE(YPelsPerMeter,uint32)
	DATABLOCK_CUSTOM_TYPE(ClrUsed,uint32)//for 256 colors bitmap, this always 256
	DATABLOCK_CUSTOM_TYPE(ClrImportant,uint32)
	//v4 header
	DATABLOCK_CUSTOM_TYPE(RedMask,uint32)
	DATABLOCK_CUSTOM_TYPE(GreenMask,uint32)
	DATABLOCK_CUSTOM_TYPE(BlueMask,uint32)
	DATABLOCK_CUSTOM_TYPE(AlphaMask,uint32)
	DATABLOCK_CUSTOM_TYPE(ColorSpaceType,uint32)

	DATABLOCK_CUSTOM_TYPE(CieRedX,uint32)
	DATABLOCK_CUSTOM_TYPE(CieRedY,uint32)
	DATABLOCK_CUSTOM_TYPE(CieRedZ,uint32)
	DATABLOCK_CUSTOM_TYPE(CieGreenX,uint32)
	DATABLOCK_CUSTOM_TYPE(CieGreenY,uint32)
	DATABLOCK_CUSTOM_TYPE(CieGreenZ,uint32)
	DATABLOCK_CUSTOM_TYPE(CieBlueX,uint32)
	DATABLOCK_CUSTOM_TYPE(CieBlueY,uint32)
	DATABLOCK_CUSTOM_TYPE(CieBlueZ,uint32)

	DATABLOCK_CUSTOM_TYPE(GammaRed,uint32)
	DATABLOCK_CUSTOM_TYPE(GammaGreen,uint32)
	DATABLOCK_CUSTOM_TYPE(GammaBlue,uint32)
	//v5 header
	DATABLOCK_CUSTOM_TYPE(Intent,uint32)
	DATABLOCK_CUSTOM_TYPE(ProfileData,uint32)
	DATABLOCK_CUSTOM_TYPE(ProfileSize,uint32)
	DATABLOCK_CUSTOM_TYPE(Reserved,uint32)

	enum Size{
		OS2_1X_BitmapHeader=12,
		OS2_2X_BitmapHeader=16,
		BitmapCoreHeader=OS2_1X_BitmapHeader,
		BitmapCoreHeader2=OS2_2X_BitmapHeader,
		BitmapInfoHeader=40,
		BitmapV2Header=52,//Adobe Photoshop?
		BitmapV3Header=56,//Adobe Photoshop?
		OS2_2X_BitmapHeaderX=64,
		BitmapV4Header=108,
		BitmapV5Header=124
	};
	enum Compression{
		NoCompression=0,
		RLE8=1,//only for 8bit
		RLE4=2,//only for 4bit
		BitField=3,//only for 16/32bit
		JPEG=4,//only for printer
		PNG=5,//only for printer
		AlphaBitField=6,//only os/2 2.x?
		CMYK=11,//only os/2 2.x?
		CMYKRLE8=12,//only os/2 2.x?
		CMYKRLE4=13//only os/2 2.x?
	};

	uint lineSize()const;
	uint colorsCount()const;
	DataBlock unknownBlock()const;//see attribute Size
	//validation
	static bool isValid_Size(uint32 size);
	static bool isValid_Planes(uint16 plane);
	static bool isValid_BitCount(uint16 bitCount);
	static bool isValid_Compression(uint32 compression);
	bool isValid_ImageSize()const;
};
struct FileBMP_BGRAsList:public DataBlock
{
	SizeType parseData();
	//
	uint colorsCount()const;
	bool getColor(uint index,ColorRGBA &color)const;
	bool setColor(uint index,const ColorRGBA &color);
	bool setColor(uint index,uint color);//0xAARRGGBB
};

class FileBMP:public DataBlock
{
public:
	SizeType parseData();
	//数据块
	FileBMP_FileHeader fileHeader;//文件头,一般14字节
	FileBMP_InfoHeader infoHeader;//信息头,一般40字节
	FileBMP_BGRAsList colorsList;//颜色表,一般来说BitCount为1,4,8的图会包含此表
	DataBlock unknownBlockBefore;//可能存在的未知数据
	BitBlock bitmapData;//图像数据
	DataBlock unknownBlockAfter;//可能存在的隐藏数据
	//有效性
	bool isValid_FileSize()const;
	bool isValid_FileOffbits()const;
	uint colorCountOfColorsList()const;
	//编码解码
	bool encodeFrom(const Bitmap_32bit &bitmap,uint16 bitCount=32,const List<uint32> *rgbaList=nullptr);//将bitmap的内容编码到BMP文件中,bitCount为希望保存的位数
	bool decodeTo(Bitmap_32bit &bitmap)const;//将BMP的内容解码到bitmap
};
#endif