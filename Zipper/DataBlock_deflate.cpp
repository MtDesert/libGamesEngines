#include"DataBlock_deflate.h"
#include"zlib.h"

DataBlock DataBlock_deflate::deflate()const{
	return DataBlock();
}
DataBlock DataBlock_deflate::inflate()const{
	DataBlock srcBlock,destBlock;
	if(subDataBlock(0,dataLength,srcBlock,dataFile)){//copy to srcBlock if data in file
		z_stream stream;
		stream.next_in = srcBlock.dataPointer;
		stream.avail_in = (uInt)srcBlock.dataLength;
		int err;
		err = inflateInit2_(&stream,0,ZLIB_VERSION,stream.avail_in);

		//uncompress
		auto sizeIncrement=1024;
		do{//if destBlock's buffer is too short, increase it
			destBlock.dataLength+=sizeIncrement;
			if(destBlock.memoryReallocate(destBlock.dataLength)){
				stream.next_out = &destBlock.dataPointer[destBlock.dataLength-sizeIncrement];
				stream.avail_out = sizeIncrement;
				err = ::inflate(&stream, Z_FINISH);
				printf("inflate()==%d\n",err);
			}
		}while(err!=Z_STREAM_END);
		err = inflateEnd(&stream);
		destBlock.memoryReallocate(stream.total_out);
	}
	return destBlock;
}