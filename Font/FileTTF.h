#ifndef FILETTF_H
#define FILETTF_H

#include"DataBlock.h"
#include"List.h"

#define FILETTF_TABLE_MACRO(macro)\
	macro(head)\
	macro(hhea)\
	macro(hmtx)\
	macro(maxp)\
	macro(name)\
	macro(cmap)\
	macro(post)\
	macro(glyf)\
	macro(loca)\
	\
	macro(cvt)\
	macro(fpgm)\
	macro(prep)\
	macro(gasp)\
	\
	macro(CFF)\
	macro(CFF2)\
	macro(VORG)\
	\
	macro(SVG)\
	\
	macro(EBDT)\
	macro(EBLC)\
	macro(EBSC)\
	macro(CBDT)\
	macro(CBLC)\
	macro(sbix)\
	\
	macro(BASE)\
	macro(GDEF)\
	macro(GPOS)\
	macro(GSUB)\
	macro(JSTF)\
	macro(MATH)\
	\
	macro(avar)\
	macro(cvar)\
	macro(fvar)\
	macro(gvar)\
	macro(HVAR)\
	macro(MVAR)\
	macro(VVAR)\
	\
	macro(COLR)\
	macro(CPAL)\
	\
	macro(DSIG)\
	macro(hdmx)\
	macro(kern)\
	macro(LTSH)\
	macro(MERG)\
	macro(meta)\
	macro(STAT)\
	macro(PCLT)\
	macro(VDMX)\
	macro(vhea)\
	macro(vmtx)

struct FileTTF_TableDirectory:public DataBlock{
	SizeType parseData();
	DATABLOCK_CUSTOM_TYPE(VersionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(VersionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(NumTables,uint16)
	DATABLOCK_CUSTOM_TYPE(SearchRange,uint16)
	DATABLOCK_CUSTOM_TYPE(EntrySelector,uint16)
	DATABLOCK_CUSTOM_TYPE(RangeShift,uint16)
};
struct FileTTF_TableEntry:public DataBlock{//Describe FileTTF_Table
	SizeType parseData();
	DATABLOCK_CUSTOM_TYPE(Tag,uint32)
	DATABLOCK_CUSTOM_TYPE(Checksum,uint32)
	DATABLOCK_CUSTOM_TYPE(Offset,uint32)
	DATABLOCK_CUSTOM_TYPE(Length,uint32)

	string tagName()const;
};
struct FileTTF_Table:public DataBlock{
	uint32 checksum()const;
};

//Required Tables
struct FileTTF_head:public FileTTF_Table{//Font header
	DATABLOCK_CUSTOM_TYPE(VersionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(VersionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(FontRevisionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(FontRevisionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(CheckSumAdjustment,uint32)
	DATABLOCK_CUSTOM_TYPE(MagicNumber,uint32)
	//flags(16 bits)
	DATABLOCK_CUSTOM_BOOL(flag0_YvalueOfBaseLine)
	DATABLOCK_CUSTOM_BOOL(flag1_XvalueOfLeftSide)
	DATABLOCK_CUSTOM_BOOL(flag2_PointSizeWillDiffer)
	DATABLOCK_CUSTOM_BOOL(flag3_UseIntegerScaling)
	DATABLOCK_CUSTOM_BOOL(flag4_TrueTypeScaler)
	DATABLOCK_CUSTOM_BOOL(flag5_LaidOutVertically)
	DATABLOCK_CUSTOM_BOOL(flag6_Must0)
	DATABLOCK_CUSTOM_BOOL(flag7_CorrectLinguisticRendering)
	DATABLOCK_CUSTOM_BOOL(flag8_MetamorphosisEffectsDesignated)
	DATABLOCK_CUSTOM_BOOL(flag9_ContainStrongRightToLeftGlyphs)
	DATABLOCK_CUSTOM_BOOL(flag10_ContainsIndicStyleRearrangementEffects)
	DATABLOCK_CUSTOM_BOOL(flag11_DefinedByAdobe)
	DATABLOCK_CUSTOM_BOOL(flag12_DefinedByAdobe)
	DATABLOCK_CUSTOM_BOOL(flag13_DefinedByAdobe)
	DATABLOCK_CUSTOM_BOOL(flag14_GlyphsSimplyGenericSymbolsForCodePointRanges)

	/*
	bit 0 - y value of 0 specifies baseline
	bit 1 - x position of left most black bit is LSB
	bit 2 - scaled point size and actual point size will differ (i.e. 24 point glyph differs from 12 point glyph scaled by factor of 2)
	bit 3 - use integer scaling instead of fractional
	bit 4 - (used by the Microsoft implementation of the TrueType scaler)
	bit 5 - This bit should be set in fonts that are intended to e laid out vertically, and in which the glyphs have been drawn such that an x-coordinate of 0 corresponds to the desired vertical baseline.
	bit 6 - This bit must be set to zero.
	bit 7 - This bit should be set if the font requires layout for correct linguistic rendering (e.g. Arabic fonts).
	bit 8 - This bit should be set for an AAT font which has one or more metamorphosis effects designated as happening by default.
	bit 9 - This bit should be set if the font contains any strong right-to-left glyphs.
	bit 10 - This bit should be set if the font contains Indic-style rearrangement effects.
	bits 11-13 - Defined by Adobe.
	bit 14 - This bit should be set if the glyphs in the font are simply generic symbols for code point ranges, such as for a last resort font.
*/

	DATABLOCK_CUSTOM_TYPE(UnitsPerEm,uint16)
	DATABLOCK_CUSTOM_TYPE(CreatedDateTime,uint64)
	DATABLOCK_CUSTOM_TYPE(ModifiedDateTime,uint64)
	DATABLOCK_CUSTOM_TYPE(XMin,int16)
	DATABLOCK_CUSTOM_TYPE(YMin,int16)
	DATABLOCK_CUSTOM_TYPE(XMax,int16)
	DATABLOCK_CUSTOM_TYPE(YMax,int16)
	//mac style
	DATABLOCK_CUSTOM_BOOL(Bold)
	DATABLOCK_CUSTOM_BOOL(Italic)
	DATABLOCK_CUSTOM_BOOL(Underline)
	DATABLOCK_CUSTOM_BOOL(Outline)
	DATABLOCK_CUSTOM_BOOL(Shadow)
	DATABLOCK_CUSTOM_BOOL(Condensed)
	DATABLOCK_CUSTOM_BOOL(Extended)

	DATABLOCK_CUSTOM_TYPE(LowestRecPPEM,uint16)
	DATABLOCK_CUSTOM_TYPE(FontDirectionHint,int16)
	DATABLOCK_CUSTOM_TYPE(IndexToLocFormat,int16)
	DATABLOCK_CUSTOM_TYPE(GlyphDataFormat,int16)
};//Font header
struct FileTTF_hhea:public FileTTF_Table{//Horizontal header
	DATABLOCK_CUSTOM_TYPE(VersionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(VersionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(Ascender,int16)
	DATABLOCK_CUSTOM_TYPE(Descender,int16)
	DATABLOCK_CUSTOM_TYPE(LineGap,int16)
	DATABLOCK_CUSTOM_TYPE(AdvanceWidthMax,uint16)
	DATABLOCK_CUSTOM_TYPE(MinLeftSideBearing,int16)
	DATABLOCK_CUSTOM_TYPE(MinRightSideBearing,int16)
	DATABLOCK_CUSTOM_TYPE(XMaxExtent,int16)
	DATABLOCK_CUSTOM_TYPE(CaretSlopeRise,int16)
	DATABLOCK_CUSTOM_TYPE(CaretSlopeRun,int16)
	DATABLOCK_CUSTOM_TYPE(CaretOffset,int16)
	DATABLOCK_CUSTOM_TYPE(Reserved0,int16)
	DATABLOCK_CUSTOM_TYPE(Reserved1,int16)
	DATABLOCK_CUSTOM_TYPE(Reserved2,int16)
	DATABLOCK_CUSTOM_TYPE(Reserved3,int16)
	DATABLOCK_CUSTOM_TYPE(MetricDataFormat,int16)
	DATABLOCK_CUSTOM_TYPE(NumberOfHMetrics,uint16)
};
struct FileTTF_hmtx:public FileTTF_Table{//Horizontal metrics
	FileTTF_hmtx();
	struct HoriMetric:public DataBlock{
		DATABLOCK_CUSTOM_TYPE(AdvanceWidth,uint16)
		DATABLOCK_CUSTOM_TYPE(LSB,int16)
	};
	HoriMetric horiMetric(int index)const;
	bool getLeftSideBearing(uint index,int16 &value)const;
	bool setLeftSideBearing(uint index,const int16 &value);

	uint16 numberOfHMetrics;
	bool horizontalMetrics(uint32 glyphIndex, int16 &leftBearing, uint16 &advanceWidth)const;
};
struct FileTTF_maxp:public FileTTF_Table{//Maximum profile
	DATABLOCK_CUSTOM_TYPE(VersionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(VersionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(NumGlyphs,uint16)
	//when Version=1.0
	DATABLOCK_CUSTOM_TYPE(MaxPoints,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxContours,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxCompositePoints,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxCompositeContours,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxZones,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxTwilightPoints,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxStorage,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxFunctionDefs,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxInstructionDefs,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxStackElements,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxSizeOfInstructions,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxComponentElements,uint16)
	DATABLOCK_CUSTOM_TYPE(MaxComponentDepth,uint16)
};
struct FileTTF_name:public FileTTF_Table{//Naming table
	struct NameRecord:public DataBlock{
		DATABLOCK_CUSTOM_TYPE(PlatformID,uint16)
		DATABLOCK_CUSTOM_TYPE(EncodingID,uint16)
		DATABLOCK_CUSTOM_TYPE(LanguageID,uint16)
		DATABLOCK_CUSTOM_TYPE(NameID,uint16)
		DATABLOCK_CUSTOM_TYPE(StringLength,uint16)
		DATABLOCK_CUSTOM_TYPE(StringOffset,uint16)
	};
	struct LangTagRecord:public DataBlock{
		DATABLOCK_CUSTOM_TYPE(StringLength,uint16)
		DATABLOCK_CUSTOM_TYPE(StringOffset,uint16)
	};

	DATABLOCK_CUSTOM_TYPE(Format,uint16)
	DATABLOCK_CUSTOM_TYPE(Count,uint16)
	DATABLOCK_CUSTOM_TYPE(StringOffset,uint16)
	NameRecord nameRecord(uint index)const;

	//when Format==1
	DATABLOCK_CUSTOM_TYPE(LangTagCount,uint16)
	LangTagRecord langTagRecord(uint index)const;
	//when Format==1

	DataBlock stringData()const;
};
struct FileTTF_OS_2:public FileTTF_Table{};//OS/2 and Windows specific metrics
struct FileTTF_post:public FileTTF_Table{//PostScript information
	DATABLOCK_CUSTOM_TYPE(VersionInt,uint16)
	DATABLOCK_CUSTOM_TYPE(VersionFloat,uint16)
	DATABLOCK_CUSTOM_TYPE(ItalicAngle,int32)
	DATABLOCK_CUSTOM_TYPE(UnderlinePosition,int16)
	DATABLOCK_CUSTOM_TYPE(UnderlineThickness,int16)
	DATABLOCK_CUSTOM_TYPE(IsFixedPitch,uint32)
	DATABLOCK_CUSTOM_TYPE(MinMemType42,uint32)
	DATABLOCK_CUSTOM_TYPE(MaxMemType42,uint32)
	DATABLOCK_CUSTOM_TYPE(MinMemType1,uint32)
	DATABLOCK_CUSTOM_TYPE(MaxMemType1,uint32)
};
struct FileTTF_loca:public FileTTF_Table{
	FileTTF_loca();
	int16 version;//0:ushort,1:ulong
	uint32 glyfDataLength;
	bool glyphLocation(uint32 glyphIndex,uint32 &off,uint32 &len)const;
};//Index to location

//Tables Related to TrueType Outlines
struct FileTTF_cvt:public FileTTF_Table{};//Control Value Table (optional table)
struct FileTTF_fpgm:public FileTTF_Table{};//Font program (optional table)
struct FileTTF_prep:public FileTTF_Table{};//CVT Program (optional table)
struct FileTTF_gasp:public FileTTF_Table{};//Grid-fitting/Scan-conversion (optional table)

//Tables Related to PostScript Outlines
struct FileTTF_CFF:public FileTTF_Table{}; //Compact Font Format 1.0
struct FileTTF_CFF2:public FileTTF_Table{};//Compact Font Format 2.0
struct FileTTF_VORG:public FileTTF_Table{};//Vertical Origin (optional table)

//Table related to SVG outlines
struct FileTTF_SVG:public FileTTF_Table{};//The SVG (Scalable Vector Graphics) table

//Tables Related to Bitmap Glyphs
struct FileTTF_EBDT:public FileTTF_Table{};//Embedded bitmap data
struct FileTTF_EBLC:public FileTTF_Table{};//Embedded bitmap location data
struct FileTTF_EBSC:public FileTTF_Table{};//Embedded bitmap scaling data
struct FileTTF_CBDT:public FileTTF_Table{};//Color bitmap data
struct FileTTF_CBLC:public FileTTF_Table{};//Color bitmap location data
struct FileTTF_sbix:public FileTTF_Table{};//Standard bitmap graphics

//Advanced Typographic Tables
struct FileTTF_BASE:public FileTTF_Table{};//Baseline data
struct FileTTF_GDEF:public FileTTF_Table{};//Glyph definition data
struct FileTTF_GPOS:public FileTTF_Table{};//Glyph positioning data
struct FileTTF_GSUB:public FileTTF_Table{};//Glyph substitution data
struct FileTTF_JSTF:public FileTTF_Table{};//Justification data
struct FileTTF_MATH:public FileTTF_Table{};//Math layout data

//Tables used for OpenType Font Variations
struct FileTTF_avar:public FileTTF_Table{};//Axis variations
struct FileTTF_cvar:public FileTTF_Table{};//CVT variations (TrueType outlines only)
struct FileTTF_fvar:public FileTTF_Table{};//Font variations
struct FileTTF_gvar:public FileTTF_Table{};//Glyph variations (TrueType outlines only)
struct FileTTF_HVAR:public FileTTF_Table{};//Horizontal metrics variations
struct FileTTF_MVAR:public FileTTF_Table{};//Metrics variations
struct FileTTF_VVAR:public FileTTF_Table{};//Vertical metrics variations

//Tables Related to Color Fonts
struct FileTTF_COLR:public FileTTF_Table{};//Color table
struct FileTTF_CPAL:public FileTTF_Table{};//Color palette table

//Other OpenType Tables
struct FileTTF_DSIG:public FileTTF_Table{};//Digital signature
struct FileTTF_hdmx:public FileTTF_Table{};//Horizontal device metrics
struct FileTTF_kern:public FileTTF_Table{};//Kerning
struct FileTTF_LTSH:public FileTTF_Table{};//Linear threshold data
struct FileTTF_MERG:public FileTTF_Table{};//Merge
struct FileTTF_meta:public FileTTF_Table{};//Metadata
struct FileTTF_STAT:public FileTTF_Table{};//Style attributes
struct FileTTF_PCLT:public FileTTF_Table{};//PCL 5 data
struct FileTTF_VDMX:public FileTTF_Table{};//Vertical device metrics
struct FileTTF_vhea:public FileTTF_Table{};//Vertical Metrics header
struct FileTTF_vmtx:public FileTTF_Table{};//Vertical Metrics

class FileTTF:public DataBlock{
public:
	SizeType parseData();

	FileTTF_TableDirectory directory;
	bool tableEntry(uint16 index,FileTTF_TableEntry &entry)const;
	bool table(uint16 index,FileTTF_Table &table)const;
	bool table(const string &name,FileTTF_Table &table)const;
	bool table(const FileTTF_TableEntry &entry,FileTTF_Table &table)const;
};

#endif