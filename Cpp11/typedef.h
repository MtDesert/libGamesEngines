#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef char Char,int8,Int8;
typedef short Short,int16,Int16;
typedef int Int,int32,Int32;
typedef long Long;//int32;
typedef long long LongLong,int64,Int64;

typedef unsigned char UnsignedChar,UnsignedInt8;
typedef unsigned short UnsignedShort,UnsignedInt16;
typedef unsigned int UnsignedInt,UnsignedInt32;
typedef unsigned long UnsignedLong;//uint32
typedef unsigned long long UnsignedLongLong,UnsignedInt64;

typedef unsigned char uchar,uChar,Uchar,UChar;
typedef unsigned short ushort,uShort,Ushort,UShort;
typedef unsigned int uint,uInt,Uint,UInt;
typedef unsigned long ulong,uLong,Ulong,ULong;
typedef unsigned long long ulonglong,uLongLong,Ulonglong,ULongLong;

typedef unsigned char uint8,uInt8,Uint8,UInt8;
typedef unsigned short uint16,uInt16,Uint16,UInt16;
typedef unsigned int uint32,uInt32,Uint32,UInt32;
//typedef unsigned long uint32
typedef unsigned long long uint64,uInt64,Uint64,UInt64;

typedef wchar_t WChar_t,Wchar_t,wChar_t;
typedef float Float;
typedef double Double;
typedef long double LongDouble;

#include<stddef.h>
typedef size_t SizeType;

#include<string>
#include<functional>
using namespace std;
typedef void (*WhenErrorString)(const string &errMsg);//错误回调函数指针,用于指向错误处理的回调函数,errMsg带有具体的错误描述

#endif