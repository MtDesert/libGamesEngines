#include"DataBlock.h"

struct FileZIP_Header:public DataBlock{
	DATABLOCK_CUSTOM_TYPE(Signature,uint32)
	DATABLOCK_CUSTOM_TYPE(VersionExtract,uint16)
	DATABLOCK_CUSTOM_TYPE(GeneralPurpose,uint16)
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint16)
	DATABLOCK_CUSTOM_TYPE(LastModFileTime,uint16)
	DATABLOCK_CUSTOM_TYPE(LastModFileDate,uint16)
	DATABLOCK_CUSTOM_TYPE(CRC32,uint32)
	DATABLOCK_CUSTOM_TYPE(CompressedSize,uint32)
	DATABLOCK_CUSTOM_TYPE(UncompressedSize,uint32)
	DATABLOCK_CUSTOM_TYPE(FilenameLength,uint16)
	DATABLOCK_CUSTOM_TYPE(ExtraFieldLength,uint16)

	string filename()const;//infer by FilenameLength
	DataBlock extraField()const;//infer by ExtraFieldLength
	DataBlock compressedData()const;//infer by CompressedSize

	enum CompressionMethod{
		Stored,
		Shrunk,
		ReducedWithCompressionFactor1,
		ReducedWithCompressionFactor2,
		ReducedWithCompressionFactor3,
		ReducedWithCompressionFactor4,
		Imploded,
		ReservedForTokenizingCompressionAlgorithm,
		Deflated
	};
};

struct FileZIP_DataDescriptor:public DataBlock{
	DATABLOCK_CUSTOM_TYPE(Signature,uint32)
	DATABLOCK_CUSTOM_TYPE(CRC32,uint32)
	DATABLOCK_CUSTOM_TYPE(CompressedSize,uint32)
	DATABLOCK_CUSTOM_TYPE(UncompressedSize,uint32)
};

struct FileZIP_CentralDirectoryFileHeader:public DataBlock{
	DATABLOCK_CUSTOM_TYPE(Signature,uint32)
	DATABLOCK_CUSTOM_TYPE(VersionMade,uint32)
	DATABLOCK_CUSTOM_TYPE(VersionExtract,uint32)
	DATABLOCK_CUSTOM_TYPE(GeneralPurpose,uint16)
	DATABLOCK_CUSTOM_TYPE(CompressionMethod,uint16)
	DATABLOCK_CUSTOM_TYPE(LastModFileTime,uint16)
	DATABLOCK_CUSTOM_TYPE(LastModFileDate,uint16)
	DATABLOCK_CUSTOM_TYPE(CRC32,uint32)
	DATABLOCK_CUSTOM_TYPE(CompressedSize,uint32)
	DATABLOCK_CUSTOM_TYPE(UncompressedSize,uint32)
	DATABLOCK_CUSTOM_TYPE(FilenameLength,uint16)
	DATABLOCK_CUSTOM_TYPE(ExtraFieldLength,uint16)
	DATABLOCK_CUSTOM_TYPE(FileCommentLength,uint16)
	DATABLOCK_CUSTOM_TYPE(DiskNumberStart,uint16)
	DATABLOCK_CUSTOM_TYPE(InternalFileAttributes,uint16)
	DATABLOCK_CUSTOM_TYPE(ExternalFileAttributes,uint32)
	DATABLOCK_CUSTOM_TYPE(RelativeOffsetOfLocalHeader,uint32)

	string filename()const;//infer by FilenameLength
	DataBlock extraField()const;//infer by ExtraFieldLength
	DataBlock fileComment()const;//infer by FileCommentLength
};
struct FileZIP_EndOfCentralDirectoryRecord:public DataBlock{
	DATABLOCK_CUSTOM_TYPE(Signature,uint32)
	DATABLOCK_CUSTOM_TYPE(DiskNumber,uint16)
	DATABLOCK_CUSTOM_TYPE(DiskNumberWithStartOfCentralDirectory,uint16)
	DATABLOCK_CUSTOM_TYPE(EntriesTotalNumInCentralDir_OnThisDisk,uint16)
	DATABLOCK_CUSTOM_TYPE(EntriesTotalNumInCentralDir,uint16)
	DATABLOCK_CUSTOM_TYPE(CentralDirectorySize,uint32)
	DATABLOCK_CUSTOM_TYPE(CentralDirectoryStartOffset_RelativeToArchiveStart,uint32)
	DATABLOCK_CUSTOM_TYPE(ZipFileCommentLength,uint16)
	string zipFileComment()const;
};

class FileZIP:public DataBlock{};