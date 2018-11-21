#include"DataBlock_deflate.h"
#include<string>
using namespace std;

struct FileGZIP_SubField:public DataBlock{
	SizeType parseData();
	DATABLOCK_CUSTOM_TYPE(ID1,uint8)
	DATABLOCK_CUSTOM_TYPE(ID2,uint8)
	DATABLOCK_CUSTOM_TYPE(SubFieldLength,uint16)
	DataBlock subFieldData()const;
};

struct FileGZIP:public DataBlock{
	FileGZIP();
	SizeType parseData();

	DATABLOCK_CUSTOM_TYPE(Header,uint16)//0x1F 0x8B
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint8)
	DATABLOCK_CUSTOM_TYPE(Flags,uint8)
	DATABLOCK_CUSTOM_TYPE(FileModificationTime,uint32)//Unix Format
	DATABLOCK_CUSTOM_TYPE(ExtraFlags,uint8)//depend on compression method
	DATABLOCK_CUSTOM_TYPE(OperationSystemType,uint8)

	FileGZIP_SubField extraField()const;
	string fileName()const;//if Flags|OriginalFileNamePresent
	string comment()const;//if Flags|FileCommentPresent
	DATABLOCK_CUSTOM_TYPE(HeaderCRC,uint16)

	DataBlock_deflate deflateData()const;

	//final field
	DATABLOCK_CUSTOM_TYPE(CRC32,uint32)
	DATABLOCK_CUSTOM_TYPE(UncompressedSize,uint32)

	enum CompressionMethod{//4..7 reverse
		Store,//Copied
		Compress,
		Pack,
		LZH,
		Deflate=8
	};
	enum Flags{//bit 5,6,7 reverse
		Flag_Text=0x01,//probably text file, ignore normally
		Flag_HeaderCRC=0x02,//
		Flag_Extra=0x04,//contain a sub field, See FileGZIP_SubField
		Flag_FileName=0x08,//contian filename, end with '\0'
		Flag_Comment=0x10,//contian comment, end with '\0'
	};
	enum OperationSystemType{
		FAT,//(MS-DOS, OS/2, NT/Win32)
		Amiga,
		VMS,//(or OpenVMS)
		Unix,
		VM_CMS,
		AtariTOS,
		HPFS,//(OS/2, NT)
		Macintosh,
		Z_System,
		CP_M,
		TOPS_20,
		NTFS,//filesystem (NT)
		QDOS,
		AcornRISCOS,
		Unknown=0xFF
	};

	//validation
	bool isValid_Header()const;
	bool isValid_CompressionMethod()const;
	bool isValid_Flags()const;
private:
	uint offset_FileName;
	uint offset_Comment;
	uint offset_HeaderCRC;
	uint offset_DeflateData;
};