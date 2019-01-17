#ifndef MINMAX_H
#define MINMAX_H

#include"typedef.h"

#define MINMAX_DECLARATION(type)\
type min##type(uint amount,...);\
type max##type(uint amount,...);

MINMAX_DECLARATION(Int8)
MINMAX_DECLARATION(Int16)
MINMAX_DECLARATION(Int32)

MINMAX_DECLARATION(Uint8)
MINMAX_DECLARATION(Uint16)
MINMAX_DECLARATION(Uint32)

#endif