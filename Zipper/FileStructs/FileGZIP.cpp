#include"FileGZIP.h"

FileGZIP_SubField::SizeType FileGZIP_SubField::parseData(){
	uint16 len;
	if(getSubFieldLength(len)){
		SizeType len4=4+len;
		if(len4<dataLength)dataLength=len4;
	}
	return dataLength;
}
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP_SubField,ID1,uint8,0,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP_SubField,ID2,uint8,1,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP_SubField,SubFieldLength,uint16,2,true)
DataBlock FileGZIP_SubField::subFieldData()const{
	DataBlock block;uint16 len;
	if(getSubFieldLength(len))return subDataBlock(4,len);
	return block;
}

//gzip block
FileGZIP::FileGZIP():
	offset_FileName(0),
	offset_Comment(0),
	offset_HeaderCRC(0),
	offset_DeflateData(0){}

FileGZIP::SizeType FileGZIP::parseData(){
	if(dataLength<10)return dataLength;
	int pos=10;
	pos+=extraField().dataLength;
	uint8 flags,byte;
	if(getFlags(flags)){
		if(flags & Flag_FileName){
			offset_FileName=pos;
			while(get_uint8(pos,byte) && byte!=0)++pos;
		}
		if(flags & Flag_Comment){
			offset_Comment=pos;
			while(get_uint8(pos,byte) && byte!=0)++pos;
		}
		if(flags & Flag_HeaderCRC){
			offset_HeaderCRC=pos;
			pos+=2;
		}
	}
	offset_DeflateData=pos;
	return dataLength;
}

DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,Header,uint16,0,true)//0x1F 0x8B
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,CompressionMethod,uint8,2,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,Flags,uint8,3,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,FileModificationTime,uint32,4,true)//Unix Format
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,ExtraFlags,uint8,8,true)//depend on compression method
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,OperationSystemType,uint8,9,true)

FileGZIP_SubField FileGZIP::extraField()const{
	FileGZIP_SubField block;
	uint8 flags;
	if(getFlags(flags)&& flags&Flag_Extra){
		if(dataLength>10 && subDataBlock(10,dataLength-10,block)){
			block.parseData();
		}
	}
	return block;
}
string FileGZIP::fileName()const{return string((char*)&dataPointer[offset_FileName]);}
string FileGZIP::comment()const{return string((char*)&dataPointer[offset_Comment]);}
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,HeaderCRC,uint16,offset_HeaderCRC,true)

DataBlock_deflate FileGZIP::deflateData()const{
	DataBlock_deflate block;
	block.dataPointer=&dataPointer[offset_DeflateData];
	if(offset_DeflateData+8<=dataLength){
		block.dataLength=dataLength-8-offset_DeflateData;
	}else{
		block.dataLength=0;
	}
	block.dataOffset=offset_DeflateData;
	return block;
}

DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,CRC32,uint32,dataLength-8,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileGZIP,UncompressedSize,uint32,dataLength-4,true)

bool FileGZIP::isValid_Header()const{
	uint16 header;
	return getHeader(header) && header==0x8B1F;
}
bool FileGZIP::isValid_CompressionMethod()const{
	uint8 method;
	return getCompressionMethod(method) && (method==Store||method==Compress||method==Pack||method==LZH||method==Deflate);
}
bool FileGZIP::isValid_Flags()const{
	uint8 flags;
	return getFlags(flags)&&((flags&0xC0)==0);
}