#ifndef EXTERN_H
#define EXTERN_H

#include"Texture.h"
#include"ShapeRenderer.h"

//所有要用到的外部变量,主要在于在各种模块中当临时变量用
//语句块的变量都存在栈中,在用的时候申请内存,用完后释放
//如果将经常用的变量放入静态存储区中,可以避免频繁的申请和释放
#define ALL_EXTERN_VARIABLE(MACRO)\
MACRO(Point2D<float>,point2D)\
MACRO(Point2D<float>,size2D)\
MACRO(Rectangle2D<float>,rect)\
MACRO(Texture,texture)\
MACRO(ShapeRenderer,shapeRenderer)

#define EXTERN_DECLARATION(type,name) extern type name;

ALL_EXTERN_VARIABLE(EXTERN_DECLARATION)

#undef EXTERN_DECLARATION

#endif // EXTERN_H