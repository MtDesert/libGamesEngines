#ifndef DATABLOCK_DEFLATE
#define DATABLOCK_DEFLATE

#include"DataBlock.h"
#include"List.h"

struct DataBlock_deflate:public DataBlock{
	static DataBlock deflate(const DataBlock &dataBlock);
	static DataBlock deflate(const List<DataBlock> &blockList);
	/**解压blockList,里面存储的数据可以是内存不连续的,解压过程会自动拼接*/
	static DataBlock inflate(const List<DataBlock> &blockList);
};
#endif