#ifndef DATABLOCK_DEFLATE
#define DATABLOCK_DEFLATE

#include"BitBlock.h"
#include"List.h"

struct DataBlock_deflate:public BitBlock{
	DataBlock deflate()const;
	DataBlock inflate()const;

	/**解压blockList,里面存储的数据可以是内存不连续的,解压过程会自动拼接*/
	static DataBlock inflate(const List<DataBlock> &blockList);
};
#endif