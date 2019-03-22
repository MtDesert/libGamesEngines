#include"DataBlock_deflate.h"
#include"List.h"
#include"ColorRGB.h"
#include"Bitmap.h"

#define FILEPNG_CHUNK_MACRO(macro)\
	macro(IHDR)\
	macro(PLTE)\
	macro(IDAT)\
	macro(IEND)\

struct FilePNG_Chunk:public DataBlock{
	virtual ~FilePNG_Chunk(){}
	SizeType parseData();

	DATABLOCK_CUSTOM_TYPE(ChunkLength,uint32)
	DATABLOCK_CUSTOM_TYPE(ChunkType,uint32)
	DataBlock chunkDataBlock()const;
	DATABLOCK_CUSTOM_TYPE(ChunkCRC,uint32)
	//infer by ChunkType
	string chunkName()const;
	void setChunkName(const string &name);

	//infer by ChunkType
	DATABLOCK_CUSTOM_BOOL(AncillaryBit)//0:Critical,1:Ancillary
	DATABLOCK_CUSTOM_BOOL(PrivateBit)//0:Public,1:Private
	DATABLOCK_CUSTOM_BOOL(ReserveBit)//Must be 0
	DATABLOCK_CUSTOM_BOOL(SafeToCopyBit)//0:Unsafe to copy,1:Safe to copy

	//infer by ChunkType & chunkDataBlock
	uint32 make_CRC()const;
};

/**
 * @brief Critical trunks:IHDR,PLTE,IDAT,IEND
 */
struct FilePNG_IHDR:public FilePNG_Chunk{//Image Header
	DATABLOCK_CUSTOM_TYPE(Width,uint32)
	DATABLOCK_CUSTOM_TYPE(Height,uint32)
	DATABLOCK_CUSTOM_TYPE(BitDepth,uint8)//valid:1,2,4,8,16.see enum ColorType
	DATABLOCK_CUSTOM_TYPE(ColorType,uint8)//see enum ColorType
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint8)//0:Default,Other reverse
	DATABLOCK_CUSTOM_TYPE(FilterMethod,uint8)//0:Default,Other invalid
	DATABLOCK_CUSTOM_TYPE(InterlaceMethod,uint8)//0:None,1:Adam7 interlace,Other invalid

	//infer by ColorType
	DATABLOCK_CUSTOM_BOOL(ColorMask_Palette)//0x01
	DATABLOCK_CUSTOM_BOOL(ColorMask_Color)//0x02
	DATABLOCK_CUSTOM_BOOL(ColorMask_Alpha)//0x04

	//enum of ColorType
	enum ColorType{
		ColorType_Gray=0,//BitDepth=1,2,4,8,16;(Each pixel is a grayscale sample)
		ColorType_RGB=2,//BitDepth=8,16;(Each pixel is an R,G,B triple)
		ColorType_Palette=3,//BitDepth=1,2,4,8;(Each pixel is a palette index;a PLTE chunk must appear)
		ColorType_GrayAlpha=4,//BitDepth=8,16;(Each pixel is a grayscale sample,followed by an alpha sample)
		ColorType_RGBAlpha=6//BitDepth=8,16;(Each pixel is an R,G,B triple,followed by an alpha sample.)
	};
	enum InterlaceMethod{
		InterlaceMethod_None,
		InterlaceMethod_Adam7
	};

	static uint channels(uint colorType);
	static uint pixelDepth(uint8 bitDepth,uint8 colorType);
	static uint rowBytes(uint8 width,uint8 bitDepth,uint8 colorType,bool withFilterMethod=false);
	//validation
	static bool isValid_BitDepth(uint8 colorType,uint8 bitDepth);
	static bool isValid_ColorType(uint8 type);
	static bool isValid_CompressionMethod(uint8 method);
	static bool isValid_FilterMethod(uint8 method);
	static bool isValid_InterlaceMethod(uint8 method);
};
struct FilePNG_PLTE:public FilePNG_Chunk{//Palette
	uint colorCount()const;
	bool getColor(uint index,ColorRGB &color)const;
	bool setColor(uint index,const ColorRGB &color);
	bool setColor(uint index,uint color);//0x00RRGGBB
};
struct FilePNG_IDAT:public FilePNG_Chunk{//Image Data
	DataBlock getZlibData()const;
};
struct FilePNG_IEND:public FilePNG_Chunk{//Image End
};

/**
 * @brief Ancillary chunks:tRNS,
 * gAMA,cHRM,sRGB,iCCP
 * tEXt,zTXt,ITXt
 * bKGD,pHYs,sBIT,sPLT,hIST,tIME
 */
//Transparency Information
struct FilePNG_tRNS:public FilePNG_Chunk{//Transparency
	//ColorType_Gray only
	DATABLOCK_CUSTOM_TYPE(Gray,ushort)

	//ColorType_Palette only
	uint alphaCount()const;
	bool getAlpha(uint index,uint8 &alpha)const;
	bool setAlpha(uint index,const uint8 &alpha);

	//ColorType_RGB only
	DATABLOCK_CUSTOM_TYPE(Red,ushort)
	DATABLOCK_CUSTOM_TYPE(Green,ushort)
	DATABLOCK_CUSTOM_TYPE(Blue,ushort)
};

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

//this struct use for transform the scanline data to colordata
struct FilePNG_Scanline:public BitBlock{
	FilePNG_Scanline(uchar *pointer=0);
	FilePNG_IHDR::ColorType colorType;
	uint bitDepth;
	uint8 unit;//for Gray
	FilePNG_PLTE *filePng_PLTE;//for Palette
	FilePNG_tRNS *filePng_tRNS;
	SizeType parseData();

	ColorRGBA getColor4B(uint x)const;
	ColorRGBA getColor4B_Gray(uint x)const;
	ColorRGBA getColor4B_RGB(uint x)const;
	ColorRGBA getColor4B_Palette(uint x)const;
	ColorRGBA getColor4B_GrayAlpha(uint x)const;
	ColorRGBA getColor4B_RGBAlpha(uint x)const;
};

//this struct use for filter the deflated data to scanline data
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

struct FilePNG:public DataBlock{
	SizeType parseData();
	SizeType reset();
	bool saveFilePNG(const string &filename)const;

	DATABLOCK_CUSTOM_TYPE(Signature,uint64)
	List<FilePNG_Chunk*> allChunks;
	FilePNG_Chunk* findChunk(const string &name)const;
	FILEPNG_FINDCHUNK(IHDR)
	FILEPNG_FINDCHUNK(PLTE)
	FILEPNG_FINDCHUNK(tRNS)
	//validation
	bool isValid_Signature()const;
	static uint64 make_Signature();

	//decode functions
	//Some return value of these function were allocated memory,
	//don't forget to call deleteDataPointer(). (DELETE)
	DataBlock decode_allIDATs()const;//(DELETE)
	//decode functions - Interlace
	bool decode_InterlaceNone(DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const;
	bool decode_InterlaceAdam7(DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const;
	bool decode_makeBitmap(Bitmap_32bit &bitmap,const DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const;
	bool decode_makeBitmapAdam7(Bitmap_32bit &bitmap,const DataBlock &filterBlock,const FilePNG_IHDR &ihdr)const;

	//图像编码解码
	bool encodeFrom(const Bitmap_32bit &bitmap,bool hasPalette,bool hasColor,bool hasAlpha);
	bool decodeTo(Bitmap_32bit &bitmap)const;
};