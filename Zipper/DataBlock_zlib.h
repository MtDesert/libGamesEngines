#include"DataBlock_deflate.h"
#include"BitBlock.h"

struct DataBlock_zlib:public BitBlock{
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint8)
	DATABLOCK_CUSTOM_TYPE(CompressionInfo,uint8)
	DATABLOCK_CUSTOM_TYPE(FlagCheck,uint8)
	DATABLOCK_CUSTOM_BOOL(PresetDictionary)
	DATABLOCK_CUSTOM_TYPE(CompressionLevel,uint8)

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