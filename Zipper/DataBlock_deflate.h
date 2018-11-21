#ifndef DATABLOCK_DEFLATE
#define DATABLOCK_DEFLATE

#include"BitBlock.h"
#include<list>

/*struct DeflateSubBlock:public BitBlock{
	DeflateSubBlock();
	DATABLOCK_CUSTOM_BOOL(IsLastBlock)
	BITBLOCK_CUSTOM_BITS(BlockType,8)
	enum BlockType{
		BlockType_StoredBlock,
		BlockType_FixedCodesBlock,
		BlockType_DynamicCodesBlock,
		BlockType_InvalidBlock
	};
	virtual unsigned encodeFrom(uchar *buffer,uint length);
	virtual unsigned decodeTo(uchar *buffer);
	virtual unsigned decodeToFile(FILE *file);//if you care about adler,make *adler non-null
	unsigned destSize;//infer that how many bytes will be decoded
protected:
	unsigned decodePos;//support for decodeTo()
};

struct StoredBlock:public DeflateSubBlock{
	StoredBlock(const DeflateSubBlock &block);
	unsigned parseData();//return bitLength
	unsigned encodeFrom(uchar *buffer,uint16 length);
	unsigned decodeTo(uchar *buffer);

	DATABLOCK_CUSTOM_TYPE(StoredLength,uint16)
	DATABLOCK_CUSTOM_TYPE(StoredLengthXor,uint16)//StoredLengthXor + Storedlength == 0xFFFF
	DataBlock storedData()const;
};

typedef struct {
	unsigned char op;//operation, extra bits, table bits
	unsigned char bits;//bits in this part of the code
	unsigned short val;//offset in table or code value
} Code;*/
/* op values as set by inflate_table():
	00000000 - literal
	0000tttt - table link, tttt != 0 is the number of table index bits
	0001eeee - length or distance, eeee is the number of extra bits
	01100000 - end of block
	01000000 - invalid code
 */

/*transplant from zlib
#define ENOUGH_LENS 852
#define ENOUGH_DISTS 592
#define ENOUGH (ENOUGH_LENS+ENOUGH_DISTS)

struct FixedCodesBlock:public DeflateSubBlock{
	FixedCodesBlock(const DeflateSubBlock &block);
	unsigned parseData();
	unsigned decodeTo(uchar *buffer);
	DataBlock dictionary;
protected:
	//from zlib
	const Code *lengthCode;//starting table for length/literal codes
	const Code *distanceCode;//starting table for distance codes
	unsigned lengthBits;//index bits for lencode
	unsigned distanceBits;//index bits for distcode
	//decode
	virtual void makeTable();//fixed table
};
struct DynamicCodesBlock:public FixedCodesBlock{
	DynamicCodesBlock(const DeflateSubBlock &block);

	BITBLOCK_CUSTOM_BITS(NumberOfLength,16)//should be less-equal 286
	BITBLOCK_CUSTOM_BITS(NumberOfDistance,8)//should be less-equal 30
	BITBLOCK_CUSTOM_BITS(NumberOfCode,8)

	//CODES
	uint8 getCodeByIndex(uint index)const;
	void setCodeByIndex(uint index,uint val);
	uint8 getCodeByOrder(uint order)const;
	void setCodeByOrder(uint order,uint val);
protected:
	static uint8 findIndexByOrder(uint order);
	static const ushort tableOrder[19];//permutation of code lengths
	Code codes[ENOUGH];// space for code tables
	//decode
	void makeTable();//dynamic table
};

struct DataBlock_inflateDictionary:public DataBlock{};*/

struct DataBlock_deflate:public BitBlock{
	DataBlock deflate()const;
	DataBlock inflate()const;
};
#endif