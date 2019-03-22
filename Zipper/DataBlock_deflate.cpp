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
		int err = ::inflateInit2_(&stream,0,ZLIB_VERSION,sizeof(z_stream));

		//uncompress
		auto sizeIncrement=1024;
		do{//if destBlock's buffer is too short, increase it
			destBlock.dataLength += sizeIncrement;
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

DataBlock DataBlock_deflate::inflate(const List<DataBlock> &blockList){
	DataBlock outBlock;
	z_stream stream;//解压用的流
	auto sizeIncrement=1024;//解压缓存增量
	auto err=::inflateInit2_(&stream,0,ZLIB_VERSION,sizeof(z_stream));//初始化,必备
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