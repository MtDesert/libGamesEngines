#include"FileTTF_glyf.h"

FileTTF_glyf::FileTTF_glyf():pos_insLen(10),pos_flags(0),pos_xCoordinate(0),pos_yCoordinate(0){}
DataBlock::SizeType FileTTF_glyf::parseData(){
	int16 nCoutours;
	uint16 u16;
	if(getNumberOfCoutours(nCoutours)){
		pos_insLen=10+nCoutours*sizeof(uint16);
		if(getInstructionLength(u16)){
			pos_flags=pos_insLen+2+u16;
			pos_xCoordinate=pos_flags+flags(false).dataLength;
			auto flagBlock=flags(true);
			auto xBlock=xCoordinates(flagBlock);
			pos_yCoordinate=pos_xCoordinate+xBlock.dataLength;
			flagBlock.deleteDataPointer();
		}
	}
	return dataLength;
}
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,NumberOfCoutours,int16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,XMin,int16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,YMin,int16,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,XMax,int16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,YMax,int16,8,false)
//Simple Glyph Description(if NumberOfCoutours>0)
DataBlock FileTTF_glyf::endPtsOfContours()const{
	int16 nCoutours;
	if(getNumberOfCoutours(nCoutours) && nCoutours>=0){
		return subDataBlock(10,nCoutours*sizeof(uint16));
	}
	return DataBlock();
}
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_glyf,InstructionLength,uint16,pos_insLen,false)
DataBlock FileTTF_glyf::instructions()const{
	uint16 len;
	if(getInstructionLength(len)){
		return subDataBlock(pos_insLen+2,len);
	}
	return DataBlock();
}
bool FileTTF_glyf::flagAmount(uint16 &amount)const{
	int16 num;
	if(getNumberOfCoutours(num) && num>0){
		auto block=endPtsOfContours();
		uint16 u16;
		if(block.get_uint16((num-1)*2,u16)){
			reverseByte(u16);
			amount=u16+1;
			return true;
		}
	}
	return false;
}

DataBlock FileTTF_glyf::flags(bool decode)const{
	DataBlock block;
	uint8 val,count;
	uint16 readPos=0,writePos=0,amount;
	if(flagAmount(amount)){
		if(decode)block.newDataPointer(amount);
		//caculate the length
		for(;writePos<amount;++writePos){
			//read
			if(!get_uint8(pos_flags+readPos,val))break;
			++readPos;

			//check repeat
			if(val&Flag_Repeat){
				if(!get_uint8(pos_flags+readPos,count))break;
				++readPos;
				for(;count>0;--count){//do repeat
					if(decode)block.set_uint8(writePos,val);
					++writePos;
				}
			}else{//not repeat
				if(decode)block.set_uint8(writePos,val);
			}
		}
		if(!decode){//we return raw data, the length is readPos
			block=subDataBlock(pos_flags,readPos);
		}
	}
	return block;
}
DataBlock FileTTF_glyf::xCoordinates(const DataBlock &flags,bool decode)const{
	DataBlock block;
	uint8 flag,u8;
	int16 i16;
	uint16 readPos=0,writePos=0,amount;
	if(flagAmount(amount)){
		if(decode)block.newDataPointer(amount*4);//long type
		//read read
		int32 x=0,val;
		for(int i=0;i<amount;++i){
			if(!flags.get_uint8(i,flag))break;
			val=0;
			if(flag&Flag_XShortVector){//x is uint8
				if(!get_uint8(pos_xCoordinate+readPos,u8))break;
				++readPos;
				val=u8;
				if((flag&Flag_XSame)==0){//x is negative
					val=-val;
				}
			}else if((flag&Flag_XSame)==0){//x not keep same as prev
				if(!get_int16(pos_xCoordinate+readPos,i16))break;
				readPos+=2;
				reverseByte(i16);
				val=i16;
			}
			x+=val;
			if(decode){
				block.set_int32(writePos*4,x);
				++writePos;
			}
		}
		if(!decode){
			block=subDataBlock(pos_xCoordinate,readPos);
		}
	}
	return block;
}
DataBlock FileTTF_glyf::yCoordinates(const DataBlock &flags,bool decode)const{
	DataBlock block;
	uint8 flag,u8;
	int16 i16;
	uint16 readPos=0,writePos=0,amount;
	if(flagAmount(amount)){
		if(decode)block.newDataPointer(amount*4);//long type
		//read read
		int32 y=0,val;
		for(int i=0;i<amount;++i){
			if(!flags.get_uint8(i,flag))break;
			val=0;
			if(flag&Flag_YShortVector){//y is uint8
				if(!get_uint8(pos_yCoordinate+readPos,u8))break;
				++readPos;
				val=u8;
				if((flag&Flag_YSame)==0){//y is negative
					val=-val;
				}
			}else if((flag&Flag_YSame)==0){//y not keep same as prev
				if(!get_int16(pos_yCoordinate+readPos,i16))break;
				readPos+=2;
				reverseByte(i16);
				val=i16;
			}
			y+=val;
			if(decode){
				block.set_int32(writePos*4,y);
				++writePos;
			}
		}
		if(!decode){
			block=subDataBlock(pos_xCoordinate,readPos);
		}
	}
	return block;
}
