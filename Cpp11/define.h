#ifndef DEFINE_H
#define DEFINE_H

#include"ErrorNumber.h"
#include<errno.h>
#include<string.h>

//报错宏,以字符串str报错(通过whenError回调函数)
#define WHEN_ERROR(str) if(whenError)whenError(str);

//报错宏,当代码code值为假时,以errorString报错
#define WHEN_CODE_ERROR(code,errorString)\
if(!(code)){\
	WHEN_ERROR(errorString)\
}

//断言宏,如果code为假,则以errorString报错
#define ASSERT(code,errorString)\
if(!(code)){\
	WHEN_ERROR(errorString)\
}

//断言宏,如果code为假,则以errno报错
//此宏主要用于code中调用系统函数的场合
#define ASSERT_ERRNO(code,errStr) ASSERT(code,errStr + ErrorNumber::getErrorString(errno))

#endif