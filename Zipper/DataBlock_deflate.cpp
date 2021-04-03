#include"DataBlock_deflate.h"
#include"zlib.h"
#include"string.h"

static DataBlock outBuffer;//解压缓冲
static const auto sizeIncrement=65536;//解压缓冲增长量

DataBlock DataBlock_deflate::deflate(const DataBlock &dataBlock){
	List<DataBlock> blockList;
	blockList.push_back(dataBlock);
	return deflate(blockList);
}
DataBlock DataBlock_deflate::deflate(const List<DataBlock> &blockList){
	DataBlock outBlock;
	z_stream stream;
	::memset(&stream,0,sizeof(stream));
	auto err=::deflateInit(&stream,Z_BEST_COMPRESSION);
	stream.next_out=outBuffer.dataPointer;
	stream.avail_out=outBuffer.dataLength;
	//循环压缩所有数据块
	for(auto &block:blockList){
		//设定输入源
		stream.next_in=block.dataPointer;
		stream.avail_in=block.dataLength;
		//
		//开始解压
		do{
			//检查缓冲
			bool needAlloc = (stream.avail_in>0 && stream.avail_out==0);
			if(needAlloc){//申请缓冲
				outBuffer.memoryAllocate(outBuffer.dataLength+sizeIncrement);
				stream.next_out=&outBuffer.dataPointer[outBuffer.dataLength-sizeIncrement];
				stream.avail_out=sizeIncrement;
			}else{//解压
				err=::deflate(&stream,Z_FULL_FLUSH);
				if(err!=Z_OK)break;//解压出错
			}
		}while(err!=Z_STREAM_END);
	}
	//压缩结束
	err=::deflate(&stream,Z_FINISH);
	outBlock.set(outBuffer.dataPointer,stream.total_out);
	err=deflateEnd(&stream);
	return outBlock;
}

DataBlock DataBlock_deflate::inflate(const List<DataBlock> &blockList){
	DataBlock outBlock;
	z_stream stream;//解压用的流
	::memset(&stream,0,sizeof(stream));
	auto err=::inflateInit(&stream);//初始化
	stream.next_out=outBuffer.dataPointer;
	stream.avail_out=outBuffer.dataLength;
	//循环解压所有数据块
	for(auto &block:blockList){
		//设定输入源
		stream.next_in=block.dataPointer;
		stream.avail_in=block.dataLength;
		//开始解压
		do{
			//检查缓冲
			bool needAlloc = (stream.avail_in>0 && stream.avail_out==0);
			if(needAlloc){//申请缓冲
				outBuffer.memoryAllocate(outBuffer.dataLength+sizeIncrement);
				stream.next_out=&outBuffer.dataPointer[outBuffer.dataLength-sizeIncrement];
				stream.avail_out=sizeIncrement;
			}else{//解压
				err=::inflate(&stream,Z_NO_FLUSH);
				if(err!=Z_OK)break;//解压出错
			}
		}while(err!=Z_STREAM_END);
	}
	//解压结束
	outBlock.set(outBuffer.dataPointer,stream.total_out);
	inflateEnd(&stream);
	return outBlock;
}