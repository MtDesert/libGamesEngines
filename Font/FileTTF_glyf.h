#ifndef FILETTF_GLYF_H
#define FILETTF_GLYF_H

#include"FileTTF.h"
#include"Point.h"

struct FileTTF_glyf:public FileTTF_Table{//Glyph data
	FileTTF_glyf();
	SizeType parseData();

	DATABLOCK_CUSTOM_TYPE(NumberOfCoutours,int16)
	DATABLOCK_CUSTOM_TYPE(XMin,int16)
	DATABLOCK_CUSTOM_TYPE(YMin,int16)
	DATABLOCK_CUSTOM_TYPE(XMax,int16)
	DATABLOCK_CUSTOM_TYPE(YMax,int16)

	//Simple Glyph Description(if NumberOfCoutours>=0)
	DataBlock endPtsOfContours()const;//uint16 * NumberOfCoutours
	DATABLOCK_CUSTOM_TYPE(InstructionLength,uint16)
	DataBlock instructions()const;//uint8 * InstructionLength

	bool flagAmount(uint16 &amount)const;//infer by endPtsOfContours
	DataBlock flags(bool decode=false)const;//if decode==true,return decode data(don't forget call deleteDataPointer()),else return raw data
	DataBlock xCoordinates(const DataBlock &flags,bool decode=false)const;//return raw data
	DataBlock yCoordinates(const DataBlock &flags,bool decode=false)const;//return raw data

	enum Flags{
		Flag_OnCurve=0x01,
		Flag_XShortVector=0x02,
		Flag_YShortVector=0x04,
		Flag_Repeat=0x08,
		Flag_XSame=0x10,
		Flag_YSame=0x20
	};
private:
	uint pos_insLen;
	uint pos_flags;
	uint pos_xCoordinate;
	uint pos_yCoordinate;
};
#endif