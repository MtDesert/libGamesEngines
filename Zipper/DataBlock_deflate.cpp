#include"DataBlock_deflate.h"
#include"zlib.h"

DataBlock DataBlock_deflate::deflate(const List<DataBlock> &blockList){
	DataBlock outBlock;
	z_stream stream;
	auto sizeIncrement=1024;//解压缓存增量
	auto err=::deflateInit(&stream,Z_BEST_COMPRESSION);
	for(auto &block:blockList){
		//设定输入源
		stream.next_in=block.dataPointer;
		stream.avail_in=block.dataLength;
		do{
			if(outBlock.memoryReallocate(outBlock.dataLength+sizeIncrement)){//先申请空间,后压缩数据
				//申请好空间后,设定输出源
				stream.next_out=&outBlock.dataPointer[outBlock.dataLength-sizeIncrement];
				stream.avail_out=sizeIncrement;
				//压缩
				err=::deflate(&stream,Z_NO_FLUSH);
				if(err!=Z_OK)break;//解压出错
			}else break;//空间申请失败
		}while(err!=Z_STREAM_END);
	}
	//压缩结束
	outBlock.memoryReallocate(stream.total_out);//回收多余的空间
	deflateEnd(&stream);
	return outBlock;
}

DataBlock DataBlock_deflate::inflate(const List<DataBlock> &blockList){
	DataBlock outBlock;
	z_stream stream;//解压用的流
	auto sizeIncrement=1024;//解压缓存增量
	auto err=::inflateInit(&stream);//初始化
	//循环解压所有数据块
	for(auto &block:blockList){
		//设定输入源
		stream.next_in=block.dataPointer;
		stream.avail_in=block.dataLength;
		//开始解压
		do{
			if(outBlock.memoryReallocate(outBlock.dataLength+sizeIncrement)){//先申请空间,后解压数据
				//申请好空间后,设定输出源
				stream.next_out=&outBlock.dataPointer[outBlock.dataLength-sizeIncrement];
				stream.avail_out=sizeIncrement;
				//解压
				err=::inflate(&stream,Z_NO_FLUSH);
				if(err!=Z_OK)break;//解压出错
			}else break;//空间申请失败
		}while(err!=Z_STREAM_END);
	}
	//解压结束
	outBlock.memoryReallocate(stream.total_out);//回收多余的空间
	inflateEnd(&stream);
	return outBlock;
}