#include"FileTTF_cmap.h"

//cmap
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap,TableVersionNumber,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap,NumberOfEncodingTables,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::EncodingRecord,PlatformID,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::EncodingRecord,EncodingID,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::EncodingRecord,Offset,uint32,4,false)
bool FileTTF_cmap::EncodingRecord::isUnicode(uint16 platformID,uint16 encodingID){
	switch(platformID){
		case PlatformID_Unicode:case PlatformID_ISO:return true;
		case PlatformID_Microsoft:
			switch(encodingID){
				case Microsoft_UNICODE_CS:case Microsoft_UCS_4:return true;
				default:return false;
			}
		break;
		default:return false;
	}
}
bool FileTTF_cmap::EncodingRecord::isUnicode()const{
	uint16 platform,encoding;
	return getPlatformID(platform)&&getEncodingID(encoding)&&isUnicode(platform,encoding);
}
bool FileTTF_cmap::encodingRecord(uint16 index, EncodingRecord &record)const{
	return subDataBlock(4+index*8,8,record);
}

DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable,Format,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable,Length,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable,Language,uint16,4,false)
bool FileTTF_cmap::SubTable::glyphIndex(uint32,uint32&)const{return false;}

//format 0
DataBlock FileTTF_cmap::SubTable_Format0::glyphIndexArray()const{
	return subDataBlock(6,512);
}
bool FileTTF_cmap::SubTable_Format0::glyphIndex(uint32 charCode, uint32 &glyphIndex)const{
	uint8 u8;
	if(glyphIndexArray().get_uint8(charCode,u8)){
		glyphIndex=u8;
		return true;
	}return false;
}
//Format 2
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format2::SubHeader,FirstCode,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format2::SubHeader,EntryCount,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format2::SubHeader,IdDelta,int16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format2::SubHeader,IdRangeOffset,uint16,12,false)
DataBlock FileTTF_cmap::SubTable_Format2::subHeaderKeys()const{
	return subDataBlock(6,512);
}
bool FileTTF_cmap::SubTable_Format2::subHeaders(uint16 index, SubHeader &header)const{
	return subDataBlock(518+index*8,8,header);
}
bool FileTTF_cmap::SubTable_Format2::glyphIndex(uint32 charCode, uint32 &glyphIndex)const{
	if(charCode>0xFFFF)return false;
	uint16 u16;
	SubHeader subHeader;
	bool hasHeader=false;
	if(charCode<=0xFF){
		if(subHeaderKeys().get_uint16(charCode*2,u16) && u16==0){
			hasHeader=subHeaders(u16,subHeader);
		}
	}else{
		if(subHeaderKeys().get_uint16((charCode>>8)*2,u16)){
			reverseByte(u16);
			u16/=8;
			if(u16){
				hasHeader=subHeaders(u16,subHeader);
			}
		}
	}
	if(hasHeader){
		uint16 firstCode,entryCount,idRangeOffset;
		int16 idDelta;
		subHeader.getFirstCode(firstCode);
		subHeader.getEntryCount(entryCount);
		subHeader.getIdDelta(idDelta);
		subHeader.getIdRangeOffset(idRangeOffset);
		uint16 idx = charCode & 0xFF;
		//search
		idx -= firstCode;
		if (idx < entryCount && idRangeOffset)
		{
			uint p=6+512+u16*8-2;
			p += idRangeOffset + 2 * idx;
			if(get_uint16(p,idx)&&idx){
				reverseByte(idx);
				glyphIndex=((int)idx + idDelta) & 0xFFFFU;
				return true;
			}
		}
	}return false;
}

//Format 4
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format4,SegCountX2,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format4,SearchRange,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format4,EntrySelector,uint16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format4,RangeShift,uint16,12,false)
DataBlock FileTTF_cmap::SubTable_Format4::endCount()const{
	uint16 segCountX2;
	if(getSegCountX2(segCountX2)){
		return subDataBlock(14,segCountX2);
	}
	return DataBlock();
}
bool FileTTF_cmap::SubTable_Format4::getReversePad(uint16 &value)const{
	uint16 segCountX2;
	if(getSegCountX2(segCountX2) && get_uint16(14+segCountX2,value)){
		reverseByte(value);
		return true;
	}else return false;
}
bool FileTTF_cmap::SubTable_Format4::setReversePad(const uint16 &value){
	uint16 segCountX2;
	if(getSegCountX2(segCountX2)){
		uint16 u16=value;
		reverseByte(u16);
		return set_uint16(14+segCountX2,u16);
	}else return false;
}
DataBlock FileTTF_cmap::SubTable_Format4::startCount()const{
	uint16 segCountX2;
	if(getSegCountX2(segCountX2)){
		return subDataBlock(14+segCountX2+2,segCountX2);
	}
	return DataBlock();
}
DataBlock FileTTF_cmap::SubTable_Format4::idDelta()const{
	uint16 segCountX2;
	if(getSegCountX2(segCountX2)){
		return subDataBlock(14+segCountX2+2+segCountX2,segCountX2);
	}
	return DataBlock();
}
DataBlock FileTTF_cmap::SubTable_Format4::idRangeOffset()const{
	uint16 segCountX2;
	if(getSegCountX2(segCountX2)){
		return subDataBlock(14+segCountX2+2+segCountX2*2,segCountX2);
	}
	return DataBlock();
}
DataBlock FileTTF_cmap::SubTable_Format4::glyphIndexArray()const{
	uint16 segCount;
	if(getSegCountX2(segCount)){
		segCount/=2;
		auto pos=sizeof(uint16)*(7+segCount+1+segCount+segCount+segCount);
		if(pos<dataLength)return subDataBlock(pos,dataLength-pos);
	}return DataBlock();
}
bool FileTTF_cmap::SubTable_Format4::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{
	uint16 segCountX2,u16,end,start,off;
	int16 delta;
	if(getSegCountX2(segCountX2)){
		auto endCount=this->endCount();
		auto startCount=this->startCount();
		uint segCount=segCountX2/2,min=0,max=segCount,mid;
		while(min<max){//binary search
			mid=(min+max)/2;
			if(endCount.get_uint16(mid*2,end)&&startCount.get_uint16(mid*2,start)){
				reverseByte(end);
				reverseByte(start);
				if(charCode<start)max=mid;
				else if(charCode>end)min=mid+1;
				else{//found it
					auto idDelta=this->idDelta();
					auto idRangeOffset=this->idRangeOffset();
					if(idDelta.get_int16(mid*2,delta)&&idRangeOffset.get_uint16(mid*2,off)){
						reverseByte(delta);
						reverseByte(off);
						if ( mid >= segCount - 1 && start == 0xFFFF && end == 0xFFFF ){
							if ( off && mid*2 + off + 2 > idRangeOffset.dataLength ){
								delta = 1;
								off = 0;
							}
						}
						if ( off == 0xFFFFU )break;
						if ( off ){
							uint p = mid*2 + off + ( charCode - start ) * 2 - idRangeOffset.dataLength;
							if(glyphIndexArray().get_uint16(p,u16)){
								reverseByte(u16);
								if (u16)glyphIndex=(u16 + delta)&0xFFFFU;
								return true;
							}
						}else{
							glyphIndex = ((int)charCode+delta)&0xFFFFU;
							return true;
						}
						break;
					}
				}
			}
		}
	}return false;
}
//Format 6
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format6,FirstCode,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_cmap::SubTable_Format6,EntryCount,uint16,8,false)
DataBlock FileTTF_cmap::SubTable_Format6::glyphIndexArray()const{
	uint16 entryCount;
	if(getEntryCount(entryCount)){
		return subDataBlock(10,entryCount*2);
	}return DataBlock();
}
bool FileTTF_cmap::SubTable_Format6::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{
	uint16 firstCode,entryCount,u16;
	if(getFirstCode(firstCode)&&getEntryCount(entryCount)){
		uint idx=charCode-firstCode;
		if(idx<entryCount && glyphIndexArray().get_uint16(idx*2,u16)){
			reverseByte(u16);
			glyphIndex=u16;
			return true;
		}
	}return false;
}
bool FileTTF_cmap::SubTable_Format8::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{return false;}
bool FileTTF_cmap::SubTable_Format10::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{return false;}
bool FileTTF_cmap::SubTable_Format12::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{return false;}
bool FileTTF_cmap::SubTable_Format13::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{return false;}
bool FileTTF_cmap::SubTable_Format14::glyphIndex(uint32 charCode,uint32 &glyphIndex)const{return false;}

bool FileTTF_cmap::subTable(uint16 index,SubTable &table)const{
	EncodingRecord record;
	return encodingRecord(index,record) && subTable(record,table);
}
bool FileTTF_cmap::subTable(const EncodingRecord &record,SubTable &table)const{
	uint32 off;
	uint16 len;
	if(record.getOffset(off) && subDataBlock(off,dataLength-off,table)){
		if(table.getLength(len)){
			table.subDataBlock(0,len,table);
			return true;
		}
	}return false;
}
bool FileTTF_cmap::subTable_Unicode(SubTable &table)const{
	uint16 tableNum;
	if(!getNumberOfEncodingTables(tableNum))return false;
	for(uint16 i=0;i<tableNum;++i){
		FileTTF_cmap::EncodingRecord record;
		if(encodingRecord(i,record) && record.isUnicode()){
			return subTable(record,table);
		}
	}return false;
}

FileTTF_cmap::SubTable* FileTTF_cmap::newSubTable(uint16 index)const{
	EncodingRecord record;
	if(encodingRecord(index,record)){
		return newSubTable(record);
	}return nullptr;
}
FileTTF_cmap::SubTable* FileTTF_cmap::newSubTable(const EncodingRecord &record)const{
	SubTable table;
	if(subTable(record,table)){
		return newSubTable(table);
	}return nullptr;
}
FileTTF_cmap::SubTable* FileTTF_cmap::newSubTable(const SubTable &table)const{
	uint16 u16;
	SubTable *ret=nullptr;
	if(table.getFormat(u16)){
		switch(u16){
			case 0:ret=new SubTable_Format0();break;
			case 2:ret=new SubTable_Format2();break;
			case 4:ret=new SubTable_Format4();break;
			case 6:ret=new SubTable_Format6();break;
			case 8:ret=new SubTable_Format8();break;
			case 10:ret=new SubTable_Format10();break;
			case 12:ret=new SubTable_Format12();break;
			case 13:ret=new SubTable_Format13();break;
			case 14:ret=new SubTable_Format14();break;
		}
	}
	if(ret){
		*ret=table;
	}
	return ret;
}
FileTTF_cmap::SubTable* FileTTF_cmap::newSubTable_Unicode()const{
	SubTable table;
	if(subTable_Unicode(table)){
		return newSubTable(table);
	}return nullptr;
}
