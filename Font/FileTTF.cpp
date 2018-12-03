#include"FileTTF.h"

DataBlock::SizeType FileTTF_TableDirectory::parseData(){
	if(dataLength>12)dataLength=12;
	return dataLength;
}
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,VersionInt,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,VersionFloat,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,NumTables,uint16,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,SearchRange,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,EntrySelector,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableDirectory,RangeShift,uint16,10,false)

DataBlock::SizeType FileTTF_TableEntry::parseData(){
	if(dataLength>16)dataLength=16;
	return dataLength;
}
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableEntry,Tag,uint32,0,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableEntry,Checksum,uint32,4,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableEntry,Offset,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_TableEntry,Length,uint32,12,false)
string FileTTF_TableEntry::tagName()const{
	uint32 tag;
	if(getTag(tag)){
		string ret((char*)&tag,4);
		if(ret[3]==' ')ret=ret.substr(0,3);
		return ret;
	}
	return "";
}

uint32
CalcTableChecksum(uint32 *Table, uint32 Length)
{
	uint32 Sum = 0L;
	uint32 *EndPtr = Table+((Length+3) & ~3) / sizeof(uint32);
	while (Table < EndPtr)
		Sum += *Table++;
	return Sum;
}

uint32 FileTTF_Table::checksum()const{
	uint32 ret = 0L;
	uint32 u32 = 0;
	uint8 u8=0;
	uint pos=0,remain=dataLength;
	while(pos<dataLength){
		remain=dataLength-pos;
		if(remain>=4 && get_uint32(pos,u32)){
			ret+=u32;
			pos+=4;
		}else{
			for(uint i=0;i<remain;++i){
				if(get_uint8(pos+i,u8)){
					u32=(uint32)u8;
					u32<<=(i*8);
					ret += u32;
				}
			}
			pos=dataLength;
		}
	}
	return ret;
}

//head
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,VersionInt,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,VersionFloat,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,FontRevisionInt,uint16,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,FontRevisionFloat,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,CheckSumAdjustment,uint32,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,MagicNumber,uint32,12,false)

DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag0_YvalueOfBaseLine,16,0)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag1_XvalueOfLeftSide,16,1)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag2_PointSizeWillDiffer,16,2)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag3_UseIntegerScaling,16,3)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag4_TrueTypeScaler,16,4)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag5_LaidOutVertically,16,5)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag6_Must0,16,6)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag7_CorrectLinguisticRendering,16,7)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag8_MetamorphosisEffectsDesignated,16,8)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag9_ContainStrongRightToLeftGlyphs,16,9)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag10_ContainsIndicStyleRearrangementEffects,16,10)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag11_DefinedByAdobe,16,11)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag12_DefinedByAdobe,16,12)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag13_DefinedByAdobe,16,13)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,flag14_GlyphsSimplyGenericSymbolsForCodePointRanges,16,14)

DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,UnitsPerEm,uint16,18,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,CreatedDateTime,uint64,20,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,ModifiedDateTime,uint64,28,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,XMin,int16,36,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,YMin,int16,38,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,XMax,int16,40,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,YMax,int16,42,false)

DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Bold,44,0)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Italic,44,1)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Underline,44,2)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Outline,44,3)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Shadow,44,4)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Condensed,44,5)
DATABLOCK_CUSTOM_BOOL_CPP(FileTTF_head,Extended,44,6)

DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,LowestRecPPEM,uint16,46,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,FontDirectionHint,int16,48,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,IndexToLocFormat,int16,50,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_head,GlyphDataFormat,int16,52,false)

//hhea
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,VersionInt,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,VersionFloat,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Ascender,int16,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Descender,int16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,LineGap,int16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,AdvanceWidthMax,uint16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,MinLeftSideBearing,int16,12,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,MinRightSideBearing,int16,14,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,XMaxExtent,int16,16,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,CaretSlopeRise,int16,18,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,CaretSlopeRun,int16,20,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,CaretOffset,int16,22,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Reserved0,int16,24,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Reserved1,int16,26,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Reserved2,int16,28,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,Reserved3,int16,30,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,MetricDataFormat,int16,32,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hhea,NumberOfHMetrics,uint16,34,false)

//hmtx
FileTTF_hmtx::FileTTF_hmtx():numberOfHMetrics(0){}
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hmtx::HoriMetric,AdvanceWidth,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_hmtx::HoriMetric,LSB,int16,2,false)
FileTTF_hmtx::HoriMetric FileTTF_hmtx::horiMetric(int index)const{
	HoriMetric ret;
	subDataBlock(index*4,4,ret);
	return ret;
}
bool FileTTF_hmtx::getLeftSideBearing(uint index,int16 &value)const{
	return get_int16(numberOfHMetrics+index*2,value);
}
bool FileTTF_hmtx::setLeftSideBearing(uint index,const int16 &value){
	return set_int16(numberOfHMetrics+index*2,value);
}

bool FileTTF_hmtx::horizontalMetrics(uint32 glyphIndex,int16 &leftBearing,uint16 &advanceWidth)const{
	if(numberOfHMetrics==0)return false;
	if(glyphIndex<numberOfHMetrics){
		auto metric=horiMetric(glyphIndex);
		return metric.getAdvanceWidth(advanceWidth) && metric.getLSB(leftBearing);
	}else{
		auto metric=horiMetric(numberOfHMetrics-1);
		return metric.getAdvanceWidth(advanceWidth) && getLeftSideBearing(glyphIndex-numberOfHMetrics,leftBearing);
	}
	return true;
}

//maxp
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,VersionInt,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,VersionFloat,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,NumGlyphs,uint16,4,false)
//when Version=1.0
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxPoints,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxContours,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxCompositePoints,uint16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxCompositeContours,uint16,12,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxZones,uint16,14,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxTwilightPoints,uint16,16,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxStorage,uint16,18,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxFunctionDefs,uint16,20,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxInstructionDefs,uint16,22,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxStackElements,uint16,24,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxSizeOfInstructions,uint16,26,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxComponentElements,uint16,28,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_maxp,MaxComponentDepth,uint16,30,false)

//name
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,PlatformID,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,EncodingID,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,LanguageID,uint16,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,NameID,uint16,6,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,StringLength,uint16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::NameRecord,StringOffset,uint16,10,false)

DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::LangTagRecord,StringLength,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name::LangTagRecord,StringOffset,uint16,2,false)

DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name,Format,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name,Count,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_name,StringOffset,uint16,4,false)
FileTTF_name::NameRecord FileTTF_name::nameRecord(uint index)const{
	NameRecord ret;
	if(subDataBlock(6+index*12,12,ret)){
		ret.parseData();
	}
	return ret;
}
bool FileTTF_name::getLangTagCount(uint16 &value)const{
	uint16 count;
	if(getCount(count) && get_uint16(6+count*12,value)){
		reverseByte(value);
		return true;
	}
	return false;
}
bool FileTTF_name::setLangTagCount(const uint16 &value){
	uint16 count;
	if(getCount(count)){
		uint16 u16=value;
		reverseByte(u16);
		return set_uint16(6+count*12,value);
	}
	return false;
}

FileTTF_name::LangTagRecord FileTTF_name::langTagRecord(uint index)const{
	LangTagRecord ret;
	if(subDataBlock(6+index*4,4,ret)){
		ret.parseData();
	}
	return ret;
}
DataBlock FileTTF_name::stringData()const{
	DataBlock ret;
	uint16 count,format,langTagCount;
	if(getCount(count) && getFormat(format) && getLangTagCount(langTagCount)){
		uint pos=6+count*12;
		if(format==1)pos+=langTagCount*4;
		ret=subDataBlock(pos,dataLength-pos);
	}
	return ret;
}

//post
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,VersionInt,uint16,0,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,VersionFloat,uint16,2,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,ItalicAngle,int32,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,UnderlinePosition,int16,8,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,UnderlineThickness,int16,10,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,IsFixedPitch,uint32,12,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,MinMemType42,uint32,16,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,MaxMemType42,uint32,20,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,MinMemType1,uint32,24,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileTTF_post,MaxMemType1,uint32,28,false)

//loca
FileTTF_loca::FileTTF_loca():version(0),glyfDataLength(0){}
bool FileTTF_loca::glyphLocation(uint32 glyphIndex,uint32 &off,uint32 &len)const{
	uint16 u16;
	uint32 nextOff=0;
	if (glyphIndex < dataLength/(version?4:2)){
		if(version){
			if(get_uint32(glyphIndex*4,off))nextOff=off;
			get_uint32((glyphIndex+1)*4,nextOff);
			reverseByte(off);
			reverseByte(nextOff);
		}else{
			if(get_uint16(glyphIndex*2,u16)){
				reverseByte(u16);
				off=nextOff=u16;
			}
			if(get_uint16((glyphIndex+1)*2,u16)){
				reverseByte(u16);
				nextOff=u16;
			}
			off<<=1;
			nextOff<<=1;
		}
	}
	//check whether error
	if(off>glyfDataLength)return false;
	//ok, return the len
	if (nextOff > glyfDataLength)nextOff=glyfDataLength;
	if (nextOff>=off){
		len=nextOff-off;
	}else{
		len=glyfDataLength-off;
	}
	return true;
}

//FileTTF
DataBlock::SizeType FileTTF::parseData(){
	subDataBlock(0,dataLength,directory);
	directory.parseData();
	return dataLength;
}
bool FileTTF::tableEntry(uint16 index,FileTTF_TableEntry &entry)const{
	uint16 numTable;
	if(directory.getNumTables(numTable) && index < numTable){
		return subDataBlock(directory.dataLength+16*index,16,entry);
	}return false;
}
bool FileTTF::table(uint16 index,FileTTF_Table &table)const{
	FileTTF_TableEntry entry;
	return tableEntry(index,entry) && this->table(entry,table);
}
bool FileTTF::table(const string &name,FileTTF_Table &table)const{
	uint16 numTable;
	FileTTF_TableEntry entry;
	if(directory.getNumTables(numTable)){
		for(uint16 i=0;i<numTable;++i){
			if(tableEntry(i,entry)&&entry.tagName()==name){
				return this->table(entry,table);
			}
		}
	}return false;
}
bool FileTTF::table(const FileTTF_TableEntry &entry,FileTTF_Table &table)const{
	uint32 off,len;
	if(entry.getOffset(off) && entry.getLength(len)){
		return subDataBlock(off,len,table);
	}return false;
}