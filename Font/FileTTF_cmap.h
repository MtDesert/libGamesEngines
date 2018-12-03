#ifndef FILETTF_CMAP_H
#define FILETTF_CMAP_H

#include"FileTTF.h"

struct FileTTF_cmap:public FileTTF_Table{//Character to glyph mapping
	DATABLOCK_CUSTOM_TYPE(TableVersionNumber,uint16)
	DATABLOCK_CUSTOM_TYPE(NumberOfEncodingTables,uint16)

	struct EncodingRecord:public DataBlock{
		enum PlatformID{
			PlatformID_Unicode=0,
			PlatformID_Macintosh=1,
			PlatformID_ISO=2,
			PlatformID_Microsoft=3,
			PlatformID_Custom=4,
			PlatformID_Adobe=7
		};
		enum EncodingID_Unicode{
			Unicode_Default,//Default semantics
			Unicode_1_1,//Unicode 1.1 semantics
			Unicode_ISO_10646,//Unicode 1.1 semantics
			Unicode_2_0,//Unicode 2.0 or later semantics (BMP only)
			Unicode_2_0x,//Unicode 2.0 or later semantics (non-BMP characters allowed)
			Unicode_Variant,//Unicode Variation Sequences
			Unicode_Full,//Full Unicode coverage (used with type 13.0 cmaps by OpenType)
		};
		enum EncodingID_Macintosh{
			Mac_ROMAN=0,
			Mac_JAPANESE=1,
			Mac_TRADITIONAL_CHINESE=2,
			Mac_KOREAN=3,
			Mac_ARABIC=4,
			Mac_HEBREW=5,
			Mac_GREEK=6,
			Mac_RUSSIAN=7,
			Mac_RSYMBOL=8,
			Mac_DEVANAGARI=9,
			Mac_GURMUKHI=10,
			Mac_GUJARATI=11,
			Mac_ORIYA=12,
			Mac_BENGALI=13,
			Mac_TAMIL=14,
			Mac_TELUGU=15,
			Mac_KANNADA=16,
			Mac_MALAYALAM=17,
			Mac_SINHALESE=18,
			Mac_BURMESE=19,
			Mac_KHMER=20,
			Mac_THAI=21,
			Mac_LAOTIAN=22,
			Mac_GEORGIAN=23,
			Mac_ARMENIAN=24,
			Mac_MALDIVIAN=25,
			Mac_SIMPLIFIED_CHINESE=25,
			Mac_TIBETAN=26,
			Mac_MONGOLIAN=27,
			Mac_GEEZ=28,
			Mac_SLAVIC=29,
			Mac_VIETNAMESE=30,
			Mac_SINDHI=31,
			Mac_UNINTERP=32
		};
		enum EncodingID_ISO{
			ISO_7bit_ASCII=0,
			ISO_10646=1,
			ISO_8859_1=2
		};
		enum EncodingID_Microsoft{
			Microsoft_SYMBOL_CS=0,
			Microsoft_UNICODE_CS=1,
			Microsoft_SJIS=2,
			Microsoft_GB2312=3,
			Microsoft_BIG_5=4,
			Microsoft_WANSUNG=5,
			Microsoft_JOHAB=6,
			Microsoft_UCS_4=10
		};
		enum EncodingID_Adobe{
			Adobe_STANDARD=0,
			Adobe_EXPERT=1,
			Adobe_CUSTOM=2,
			Adobe_LATIN_1=3
		};
		DATABLOCK_CUSTOM_TYPE(PlatformID,uint16)
		DATABLOCK_CUSTOM_TYPE(EncodingID,uint16)
		DATABLOCK_CUSTOM_TYPE(Offset,uint32)
		
		//infer by platformID,encodingID
		static bool isUnicode(uint16 platformID,uint16 encodingID);
		bool isUnicode()const;
	};
	bool encodingRecord(uint16 index,EncodingRecord &record)const;

	struct SubTable:public DataBlock{
		enum Format{
			ByteEncodingTable=0,
			HighByteMappingThroughTable=2,
			SegmentMappingTheDeltaValues=4,
			TrimmedTableMapping=6,
			Mixed16bitAnd32bitCoverage=8,
			TrimmedArray=10,
			SegmentedCoverage=12,
			ManyToOneRangeMappings=13,
			UnicodeVariationSequences=14
		};
		DATABLOCK_CUSTOM_TYPE(Format,uint16)
		DATABLOCK_CUSTOM_TYPE(Length,uint16)
		DATABLOCK_CUSTOM_TYPE(Language,uint16)
		virtual bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format0:public SubTable{
		DataBlock glyphIndexArray()const;//uint8[256]
		//override
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format2:public SubTable{
		struct SubHeader:public DataBlock{
			DATABLOCK_CUSTOM_TYPE(FirstCode,uint16)
			DATABLOCK_CUSTOM_TYPE(EntryCount,uint16)
			DATABLOCK_CUSTOM_TYPE(IdDelta,int16)
			DATABLOCK_CUSTOM_TYPE(IdRangeOffset,uint16)
		};
		DataBlock subHeaderKeys()const;//uint16[256]
		bool subHeaders(uint16 index,SubHeader &header)const;//SubHeader[]
		//override
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format4:public SubTable{
		DATABLOCK_CUSTOM_TYPE(SegCountX2,uint16)
		DATABLOCK_CUSTOM_TYPE(SearchRange,uint16)
		DATABLOCK_CUSTOM_TYPE(EntrySelector,uint16)
		DATABLOCK_CUSTOM_TYPE(RangeShift,uint16)
		DataBlock endCount()const;//uint16[SegCount]
		DATABLOCK_CUSTOM_TYPE(ReversePad,uint16)
		DataBlock startCount()const;//uint16[SegCount]
		DataBlock idDelta()const;//uint16[SegCount]
		DataBlock idRangeOffset()const;//uint16[SegCount]
		DataBlock glyphIndexArray()const;
		//override
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format6:public SubTable{
		DATABLOCK_CUSTOM_TYPE(FirstCode,uint16)
		DATABLOCK_CUSTOM_TYPE(EntryCount,uint16)
		DataBlock glyphIndexArray()const;//uint16[EntryCount]
		//override
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format8:public SubTable{
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format10:public SubTable{
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format12:public SubTable{
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format13:public SubTable{
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	struct SubTable_Format14:public SubTable{
		bool glyphIndex(uint32 charCode,uint32 &glyphIndex)const;
	};
	bool subTable(uint16 index,SubTable &table)const;
	bool subTable(const EncodingRecord &record,SubTable &table)const;
	bool subTable_Unicode(SubTable &table)const;//find first unicode sub table,return whether success
	//new table(Deletable)
	SubTable* newSubTable(uint16 index)const;
	SubTable* newSubTable(const EncodingRecord &record)const;
	SubTable* newSubTable(const SubTable &table)const;
	SubTable* newSubTable_Unicode()const;
};

#endif