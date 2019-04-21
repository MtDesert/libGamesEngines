#ifndef FILEPNG_H
#define FILEPNG_H

#include"DataBlock_deflate.h"
#include"List.h"
#include"ColorRGB.h"
#include"Bitmap.h"

#define FILEPNG_CHUNK_MACRO(macro)\
	macro(IHDR)\
	macro(PLTE)\
	macro(tRNS)\
	macro(IDAT)\
	macro(IEND)\
	macro(code)

struct FilePNG_Chunk:public DataBlock{
	SizeType parseData();

	//块结构
	DATABLOCK_CUSTOM_TYPE(ChunkLength,uint32)
	DATABLOCK_CUSTOM_TYPE(ChunkType,uint32)
	DataBlock chunkDataBlock()const;
	DATABLOCK_CUSTOM_TYPE(ChunkCRC,uint32)
	//块名称
	string chunkName()const;
	void setChunkName(const string &name);

	//块属性
	DATABLOCK_CUSTOM_BOOL(AncillaryBit)//0:关键的,1:辅助的
	DATABLOCK_CUSTOM_BOOL(PrivateBit)//0:公有的,1:私有的
	DATABLOCK_CUSTOM_BOOL(ReserveBit)//保留位0
	DATABLOCK_CUSTOM_BOOL(SafeToCopyBit)//0:非安全复制,1:安全复制

	//循环冗余校验
	uint32 makeCRC()const;

	//创建(本结构自己申请空间)
	void makeChunk(uint32 length,const string &name);
};

/** @brief 关键块:IHDR,PLTE,IDAT,IEND*/
struct FilePNG_IHDR:public FilePNG_Chunk{//图像头,首个块,一般紧跟在PNG签名后
	DATABLOCK_CUSTOM_TYPE(Width,uint32)
	DATABLOCK_CUSTOM_TYPE(Height,uint32)
	DATABLOCK_CUSTOM_TYPE(BitDepth,uint8)//有效值:1,2,4,8,16.参考enum ColorType
	DATABLOCK_CUSTOM_TYPE(ColorType,uint8)//参考enum ColorType
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint8)//0:默认,其他保留
	DATABLOCK_CUSTOM_TYPE(FilterMethod,uint8)//0:默认,其他无效
	DATABLOCK_CUSTOM_TYPE(InterlaceMethod,uint8)//0:无,1:Adam7交错,其它无效

	//颜色类型(bit)
	DATABLOCK_CUSTOM_BOOL(ColorMask_Palette)//0x01是否有调色板
	DATABLOCK_CUSTOM_BOOL(ColorMask_Color)//0x02是否彩色
	DATABLOCK_CUSTOM_BOOL(ColorMask_Alpha)//0x04有无Alpha通道

	//颜色类型枚举
	enum ColorType{
		//标准格式
		ColorType_Gray=0,//BitDepth=1,2,4,8,16;(每个像素为灰度采样)
		ColorType_RGB=2,//BitDepth=8,16;(每个像素为RGB)
		ColorType_Palette=3,//BitDepth=1,2,4,8;(每个像素为调色板索引,必须有PLTE块,可能有tRNS)
		ColorType_GrayAlpha=4,//BitDepth=8,16;(每个像素为灰度采样,紧接着alpha采样)
		ColorType_RGBAlpha=6,//BitDepth=8,16;(每个像素为RGB,紧接着alpha采样)
		//非标准格式
		ColorType_PaletteGray=1,//带调色板的灰度图,必须有PLTE块(8位灰度表)
		ColorType_PaletteGrayAlpha=5,//带调色板的灰度Alpha图,必须有PLTE块(16位灰度Alpha表)
		ColorType_PaletteRGBAlpha=7 //带调色板的彩色Alpha图,必须有PLTE块(32位RGBAlpha表)
	};
	enum InterlaceMethod{
		InterlaceMethod_None,
		InterlaceMethod_Adam7
	};

	static uint channels(uint colorType);//通道数,灰度图只有1个通道,RGB有3个通道,如果有ALPHA则再多一个通道
	static uint pixelDepth(uint8 bitDepth,uint8 colorType);//像素深度,即表示一个像素所用的bit数
	static uint rowBytes(uint8 width,uint8 bitDepth,uint8 colorType,bool withFilterMethod=false);//行字节数,根据width计算出一行数据需要的字节数,如果包含FilterMethod则多出一个字节
	//有效性
	static bool isValid_BitDepth(uint8 colorType,uint8 bitDepth);
	static bool isValid_ColorType(uint8 type);
	static bool isValid_CompressionMethod(uint8 method);
	static bool isValid_FilterMethod(uint8 method);
	static bool isValid_InterlaceMethod(uint8 method);

	//创建
	void makeChunk(uint32 width,uint32 height,uint8 bitDepth,bool hasPalette,bool hasColor,bool hasAlpha);
};

struct FilePNG_tRNS:public FilePNG_Chunk{//透明度
	//ColorType_Gray only
	DATABLOCK_CUSTOM_TYPE(Gray,ushort)

	//ColorType_Palette only
	uint alphaAmount()const;
	bool getAlpha(uint index,uint8 &alpha)const;
	bool setAlpha(uint index,const uint8 &alpha);

	//ColorType_RGB only
	DATABLOCK_CUSTOM_TYPE(Red,ushort)
	DATABLOCK_CUSTOM_TYPE(Green,ushort)
	DATABLOCK_CUSTOM_TYPE(Blue,ushort)

	void makeChunk(uint32 size);
	//移除不必要的alpha,若全部移除,则移除掉该结构体的数据
	void removeUnnecessaryAlpha();
};

struct FilePNG_PLTE:public FilePNG_Chunk{//调色板,个别PNG格式会带有
	FilePNG_tRNS *filePng_tRNS;//透明度需要此结构体
	FilePNG_PLTE();

	uint rgbAmount()const;//颜色数
	//读写颜色RGB
	bool getColor(uint index,ColorRGBA &color)const;
	bool setColor(uint index,const ColorRGBA &color);
	bool setColor(uint index,uint32 color);//0x00RRGGBB
	//以下为非标准接口
	//读写灰度
	uint grayAmount()const;
	bool getGray(uint index,uint8 &gray)const;
	bool setGray(uint index,const uint8 &gray);
	//读写灰度alpha
	uint grayAlphaAmount()const;
	bool getGrayAlpha(uint index,uint16 &grayAlpha)const;//0xAAGG
	bool setGrayAlpha(uint index,const uint16 &grayAlpha);
	//读写颜色RGBA
	uint rgbaAmount()const;
	bool getRGBA(uint index,ColorRGBA &color)const;
	bool getRGBA(uint index,uint32 &color)const;
	bool setRGBA(uint index,const ColorRGBA &color);
	bool setRGBA(uint index,uint32 &color);//0x00RRGGBB
	//导入导出
	void getColorsList(bool hasColor,bool hasAlpha,List<uint32> &colorsList)const;
	void getColorsList(const FilePNG_IHDR &ihdr,List<uint32> &colorsList)const;
	void setColorsList(bool hasColor,bool hasAlpha,const List<uint32> &colorsList);
};
struct FilePNG_IDAT:public FilePNG_Chunk{//图像数据本体
	void makeChunk(const DataBlock &idatData);
};
struct FilePNG_IEND:public FilePNG_Chunk{//图像结束
	void makeChunk();
};

/**
 * @brief Ancillary chunks:tRNS,
 * gAMA,cHRM,sRGB,iCCP
 * tEXt,zTXt,ITXt
 * bKGD,pHYs,sBIT,sPLT,hIST,tIME
 */

//Color Space Information
struct FilePNG_gAMA:public FilePNG_Chunk{//Image Gama
	DATABLOCK_CUSTOM_TYPE(Gama,uint)
};
struct FilePNG_cHRM:public FilePNG_Chunk{//Primary chromaticities
	DATABLOCK_CUSTOM_TYPE(WhitePointX,uint)
	DATABLOCK_CUSTOM_TYPE(WhitePointY,uint)
	DATABLOCK_CUSTOM_TYPE(RedX,uint)
	DATABLOCK_CUSTOM_TYPE(RedY,uint)
	DATABLOCK_CUSTOM_TYPE(GreenX,uint)
	DATABLOCK_CUSTOM_TYPE(GreenY,uint)
	DATABLOCK_CUSTOM_TYPE(BlueX,uint)
	DATABLOCK_CUSTOM_TYPE(BlueY,uint)
};
struct FilePNG_sRGB:public FilePNG_Chunk{//Standard RGB color space
	DATABLOCK_CUSTOM_TYPE(RenderingIntent,uint8)
	enum RenderingIntent{
		Perceptual,
		RelativeColorimetric,
		Saturation,
		AbsoluteColorimetric,
	};
};
struct FilePNG_iCCP:public FilePNG_Chunk{//International Color Consortium profile
	string profileName()const;//1-79 bytes(character string),Null separator(1 byte)at the end of the string
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint8)
	//   Compression method: 1 byte
	//   Compressed profile: n bytes
};

//Textual information
struct FilePNG_tEXt:public FilePNG_Chunk{//Textual data
};
struct FilePNG_zTXt:public FilePNG_Chunk{//Compressed textual data
};
struct FilePNG_ITXt:public FilePNG_Chunk{//International textual data
};

//Miscellaneous information
struct FilePNG_bKGD:public FilePNG_Chunk{//Background color
};
struct FilePNG_pHYs:public FilePNG_Chunk{//Physical pixel dimensions
};
struct FilePNG_sBIT:public FilePNG_Chunk{//Significant bits
};
struct FilePNG_sPLT:public FilePNG_Chunk{//Suggested palette
};
struct FilePNG_hIST:public FilePNG_Chunk{//Palette histogram
};
struct FilePNG_tIME:public FilePNG_Chunk{//Image last-modification time
};

//自定义结构
/**PNG代码,可在PNG结构中插入特定的代码
对于代码规范目前没有统一的标准,主要依靠宿主程序解读
*/
struct FilePNG_code:public FilePNG_Chunk{
	SizeType parseData();
	//使用文本代码创建此结构
	void makeChunk(const string &textCode);
};

//扫描线,其实是一行字节,用于在扫描线和颜色数据之间相互转化
struct FilePNG_Scanline:public Bitmap32_Scanline{
	FilePNG_Scanline();
	~FilePNG_Scanline();

	uint pixelDepth;
	bool hasPalette,hasColor,hasAlpha;

	//设置位深,同时设置一些缓冲变量
	void setIHDR(const FilePNG_IHDR &ihdr);
	//解码一行数据,结果保存到bitmap中
	bool decodeLine(uint y,Bitmap_32bit &bitmap)const;
	//编码一行数据
	bool encodeLine(uint y,const Bitmap_32bit &bitmap);
};

//PNG的子图像
struct FilePNG_SubImage:public DataBlock{
	FilePNG_SubImage(uint width=0,uint height=0,uint pixelDepth=0);
	void filter();

	enum FilterMethod{
		FilterMethod_None,
		FilterMethod_Sub,
		FilterMethod_Up,
		FilterMethod_Average,
		FilterMethod_Paeth,
	};

	uint width;
	uint height;
	uint pixelDepth;
};

#define FILEPNG_FINDCHUNK(name)\
FilePNG_##name* find##name()const;

//PNG文件结构,由1个PNG签名和n个块组成
struct FilePNG:public DataBlock{
	SizeType parseData();
	bool saveFilePNG(const string &filename)const;

	//主要结构
	DATABLOCK_CUSTOM_TYPE(Signature,uint64)
	List<FilePNG_Chunk*> allChunks;
	FilePNG_Chunk* findChunk(const string &name)const;
	FILEPNG_FINDCHUNK(IHDR)
	FILEPNG_FINDCHUNK(PLTE)
	FILEPNG_FINDCHUNK(tRNS)

	//验证
	bool isValid_Signature()const;
	static uint64 make_Signature();

	//编码函数
	DataBlock encode_makeFilterBlock(const Bitmap_32bit &bitmap,const FilePNG_IHDR &ihdr,List<uint32> *colorsList=nullptr);
	//解码函数
	DataBlock decode_allIDATs()const;
	bool decode_InterlaceNone(DataBlock &filterBlock,const FilePNG_Scanline &scanLine)const;
	bool decode_InterlaceAdam7(DataBlock &filterBlock, const FilePNG_Scanline &scanLine)const;
	bool decode_makeBitmap(Bitmap_32bit &bitmap,const DataBlock &filterBlock,FilePNG_Scanline &scanLine)const;
	bool decode_makeBitmapAdam7(Bitmap_32bit &bitmap,const DataBlock &filterBlock,FilePNG_Scanline &scanLine)const;

	//图像编码解码
	bool encodeFrom(const Bitmap_32bit &bitmap,uint8 bitDepth,bool hasPalette,bool hasColor,bool hasAlpha,List<uint32> *colorsList=nullptr);
	bool decodeTo(Bitmap_32bit &bitmap)const;

	//删除
	bool deleteDataPointer(bool freeAllChunks=false);
};

#endif