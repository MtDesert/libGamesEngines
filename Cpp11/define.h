#ifndef DEFINE_H
#define DEFINE_H

#include<errno.h>
#include<string.h>

//报错宏,以字符串形式报错(通过whenError回调函数)
#define WHEN_ERROR(str) if(whenError)whenError(str);

//断言宏,如果code为假,则以errorString报错
#define ASSERT(code,errorString)\
if(!(code)){\
	WHEN_ERROR(errorString)\
	return false;\
}

//断言宏,如果code为假,则以errno报错
#define ASSERT_ERRNO(code) ASSERT(code,strerror(errno))

#endif