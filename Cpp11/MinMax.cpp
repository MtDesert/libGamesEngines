#include"MinMax.h"
#include<stdarg.h>
#include<limits.h>

#define MINMAX_DEFINITION(MinOrMax,type,initValue,cmp,type32or64)\
type MinOrMax##type(uint amount,...){\
	type ret=initValue;\
	va_list vaList;\
	va_start(vaList,amount);\
	for(uint i=0;i<amount;++i){\
		type value=va_arg(vaList,type32or64);\
		if(value cmp ret)ret=value;\
	}\
	va_end(vaList);\
	return ret;\
}

#define MINMAX_MIN_DEF(type,initValue,type32or64)\
	MINMAX_DEFINITION(min,type,initValue,<,type32or64)
#define MINMAX_MAX_DEF(type,initValue,type32or64)\
	MINMAX_DEFINITION(max,type,initValue,>,type32or64)

#define MINMAX_DEF(type,minValue,maxValue,type32or64)\
MINMAX_MIN_DEF(type,maxValue,type32or64)\
MINMAX_MAX_DEF(type,minValue,type32or64)

MINMAX_DEF(Int8,CHAR_MIN,CHAR_MAX,int32)
MINMAX_DEF(Int16,SHRT_MIN,SHRT_MAX,int32)
MINMAX_DEF(Int32,INT_MIN,INT_MAX,int32)

MINMAX_DEF(Uint8,0,UCHAR_MAX,uint32)
MINMAX_DEF(Uint16,0,USHRT_MAX,uint32)
MINMAX_DEF(Uint32,0,UINT_MAX,uint32)