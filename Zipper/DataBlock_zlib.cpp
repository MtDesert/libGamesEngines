#include"DataBlock_zlib.h"

//zlib block
BITBLOCK_CUSTOM_BITS_CPP(DataBlock_zlib,CompressionMethod,8,0,4)
BITBLOCK_CUSTOM_BITS_CPP(DataBlock_zlib,CompressionInfo,8,4,4)
BITBLOCK_CUSTOM_BITS_CPP(DataBlock_zlib,FlagCheck,8,8,5)
DATABLOCK_CUSTOM_BOOL_CPP(DataBlock_zlib,PresetDictionary,1,5)
BITBLOCK_CUSTOM_BITS_CPP(DataBlock_zlib,CompressionLevel,8,14,2)
DATABLOCK_CUSTOM_TYPE_CPP(DataBlock_zlib,DictID,uint32,2,false)

DataBlock_deflate DataBlock_zlib::deflateDataBlock()const{
	DataBlock_deflate block;
	bool presetDict;
	if(!getPresetDictionary(presetDict))return block;
	uint off=2+(presetDict?4:0);
	if(off<=dataLength&&off+4<=dataLength){
		subDataBlock(off,dataLength-off-4,block);//last 4 byte is checksum
	}return block;
}
DATABLOCK_CUSTOM_TYPE_CPP(DataBlock_zlib,Adler32CheckSum,uint32,dataLength-4,false)

//zlib block validation
bool DataBlock_zlib::isValid_CompressionMethod()const{
	uint8 method;
	return getCompressionMethod(method)&&method==Deflate;
}
bool DataBlock_zlib::isValid_CompressionInfo()const{
	uint8 info;
	return getCompressionInfo(info)&&info>=0&&info<=7;
}
bool DataBlock_zlib::isValid_FlagCheck()const{
	uint8 check,correct;
	if(!getFlagCheck(check)|| !make_FlagCheck(correct))return false;
	return check==correct || check-correct==31 || correct-check==31;
}
bool DataBlock_zlib::make_FlagCheck(uint8 &value)const{
	uint16 sum;
	if(!get_uint16(0,sum))return false;
	reverseByte(sum);
	uint16 x=sum & 0xFFE0;
	//let sum=x + y,sum % 31 == 0,then ->
	//(x+y) % 31 == 0 (0<=y<=31) ->
	//x % 31 == 31 - y ->
	//y == 31 - x % 31
	value=31 - x % 31;
	return true;
}