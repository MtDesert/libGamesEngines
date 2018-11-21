#include"DataBlock_deflate.h"

struct DataBlock_zlib:public BitBlock{
	BITBLOCK_CUSTOM_BITS(CompressionMethod,8)
	BITBLOCK_CUSTOM_BITS(CompressionInfo,8)
	BITBLOCK_CUSTOM_BITS(FlagCheck,8)
	DATABLOCK_CUSTOM_BOOL(PresetDictionary)
	BITBLOCK_CUSTOM_BITS(CompressionLevel,8)

	enum CompressionMethod{
		Deflate=8,Reserve=15
	};
	enum CompressionLevel{
		Fastest,Fast,DefaultLevel,Maximum//Maximum is slowest
	};

	//if PresetDictionary is true
	DATABLOCK_CUSTOM_TYPE(DictID,uint32)

	DataBlock_deflate deflateDataBlock()const;
	DATABLOCK_CUSTOM_TYPE(Adler32CheckSum,uint32)

	//validation
	bool isValid_CompressionMethod()const;
	bool isValid_CompressionInfo()const;
	bool isValid_FlagCheck()const;
	bool make_FlagCheck(uint8 &value)const;
};