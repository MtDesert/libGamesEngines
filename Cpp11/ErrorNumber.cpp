#include "ErrorNumber.h"
#include<stdio.h>
#include<errno.h>

#ifdef __MINGW32__
#define ENOTBLK 15
#endif

#define ERROR_NUMBER_STRING(MACRO,chinese) \
errorString[MACRO]=#chinese;

#ifdef __MINGW32__
#define MAX_STRING_AMOUNT 140
#else
#define MAX_STRING_AMOUNT 134
#endif
static const char* errorString[MAX_STRING_AMOUNT]={NULL};

void ErrorNumber::init(){
	//error-base.h
	ERROR_NUMBER_STRING(0,无错误)
	ERROR_NUMBER_STRING(EPERM,操作不允许)
	ERROR_NUMBER_STRING(ENOENT,无此文件或目录)
	ERROR_NUMBER_STRING(ESRCH,无此进程)
	ERROR_NUMBER_STRING(EINTR,中断系统调用)
	ERROR_NUMBER_STRING(EIO,输入输出错误)
	ERROR_NUMBER_STRING(ENXIO,无此设备或地址)
	ERROR_NUMBER_STRING(E2BIG,参数表太长)
	ERROR_NUMBER_STRING(ENOEXEC,执行格式错误)
	ERROR_NUMBER_STRING(EBADF,坏文件号)
	ERROR_NUMBER_STRING(ECHILD,无子进程)
	ERROR_NUMBER_STRING(EAGAIN,再试一次)
	ERROR_NUMBER_STRING(ENOMEM,内存不足)
	ERROR_NUMBER_STRING(EACCES,授权拒绝)
	ERROR_NUMBER_STRING(EFAULT,坏地址)
	ERROR_NUMBER_STRING(ENOTBLK,块设备要求)
	ERROR_NUMBER_STRING(EBUSY,设备或资源忙)
	ERROR_NUMBER_STRING(EEXIST,文件存在)
	ERROR_NUMBER_STRING(EXDEV,交叉设备链接)
	ERROR_NUMBER_STRING(ENODEV,无此设备)
	ERROR_NUMBER_STRING(ENOTDIR,非目录)
	ERROR_NUMBER_STRING(EISDIR,是目录)
	ERROR_NUMBER_STRING(EINVAL,无效参数)
	ERROR_NUMBER_STRING(ENFILE,文件表溢出)
	ERROR_NUMBER_STRING(EMFILE,打开文件过多)
	ERROR_NUMBER_STRING(ENOTTY,非打字机)
#ifndef __i386
	ERROR_NUMBER_STRING(ETXTBSY,文本文件忙)
#endif
	ERROR_NUMBER_STRING(EFBIG,文件过大)
	ERROR_NUMBER_STRING(ENOSPC,设备无剩余空间)
	ERROR_NUMBER_STRING(ESPIPE,非法搜寻)
	ERROR_NUMBER_STRING(EROFS,只读文件系统)
	ERROR_NUMBER_STRING(EMLINK,链接过多)
	ERROR_NUMBER_STRING(EPIPE,损坏的管道)
	ERROR_NUMBER_STRING(EDOM,函数域外的数学参数)
	ERROR_NUMBER_STRING(ERANGE,数学结果无法表示)
	//errno.h
	ERROR_NUMBER_STRING(EDEADLK,资源会发生死锁)
	ERROR_NUMBER_STRING(ENAMETOOLONG,文件名太长)
	ERROR_NUMBER_STRING(ENOLCK,无记录锁可用)
	ERROR_NUMBER_STRING(ENOSYS,无效系统调用数)
	ERROR_NUMBER_STRING(ENOTEMPTY,目录非空)
#ifdef __MINGW32__
	ERROR_NUMBER_STRING(EILSEQ,非法字节序列)
#else
	ERROR_NUMBER_STRING(ELOOP,遇到太多符号链接)
	ERROR_NUMBER_STRING(41,操作会阻塞)
	ERROR_NUMBER_STRING(ENOMSG,无所需类型消息)
	ERROR_NUMBER_STRING(EIDRM,标识符已删除)
	ERROR_NUMBER_STRING(ECHRNG,通道数超出范围)
	ERROR_NUMBER_STRING(EL2NSYNC,2级不同步)
	ERROR_NUMBER_STRING(EL3HLT,3级挂机)
	ERROR_NUMBER_STRING(EL3RST,3级重置)
	ERROR_NUMBER_STRING(ELNRNG,链接数超出范围)
	ERROR_NUMBER_STRING(EUNATCH,协议驱动未附加)
	ERROR_NUMBER_STRING(ENOCSI,无CSI结构可用)
	ERROR_NUMBER_STRING(EL2HLT,2级挂机)
	ERROR_NUMBER_STRING(EBADE,无效交换)
	ERROR_NUMBER_STRING(EBADR,无效请求描述符)
	ERROR_NUMBER_STRING(EXFULL,交换满)
	ERROR_NUMBER_STRING(ENOANO,无阳极)
	ERROR_NUMBER_STRING(EBADRQC,无效请求码)
	ERROR_NUMBER_STRING(EBADSLT,无效槽)
	ERROR_NUMBER_STRING(58,资源会发生死锁)
	ERROR_NUMBER_STRING(EBFONT,坏字体文件格式)
	ERROR_NUMBER_STRING(ENOSTR,设备非流)
	ERROR_NUMBER_STRING(ENODATA,无数据可用)
	ERROR_NUMBER_STRING(ETIME,计时器超时)
	ERROR_NUMBER_STRING(ENOSR,缺乏流资源)
	ERROR_NUMBER_STRING(ENONET,机器不在网络上)
	ERROR_NUMBER_STRING(ENOPKG,包未安装)
	ERROR_NUMBER_STRING(EREMOTE,远程对象)
	ERROR_NUMBER_STRING(ENOLINK,链路已断开)
	ERROR_NUMBER_STRING(EADV,广告错误)
	ERROR_NUMBER_STRING(ESRMNT,SR挂载错误)
	ERROR_NUMBER_STRING(ECOMM,发送时通讯错误)
	ERROR_NUMBER_STRING(EPROTO,协议错误)
	ERROR_NUMBER_STRING(EMULTIHOP,尝试多次反射)
	ERROR_NUMBER_STRING(EDOTDOT,RFS特定错误)
	ERROR_NUMBER_STRING(EBADMSG,非数据消息)
	ERROR_NUMBER_STRING(EOVERFLOW,值对于定义的数据类型太大)
	ERROR_NUMBER_STRING(ENOTUNIQ,在网络上名字非唯一)
	ERROR_NUMBER_STRING(EBADFD,文件描述符处于不良状态)
	ERROR_NUMBER_STRING(EREMCHG,远程地址变更)
	ERROR_NUMBER_STRING(ELIBACC,无法访问所需的共享库)
	ERROR_NUMBER_STRING(ELIBBAD,访问损坏的共享库)
	ERROR_NUMBER_STRING(ELIBSCN,a.out的.lib部分已损坏)
	ERROR_NUMBER_STRING(ELIBMAX,试图链接太多共享库)
	ERROR_NUMBER_STRING(ELIBEXEC,无法直接执行共享库)
	ERROR_NUMBER_STRING(EILSEQ,非法字节序列)
	ERROR_NUMBER_STRING(ERESTART,应重启被中断的系统调用)
	ERROR_NUMBER_STRING(ESTRPIPE,流管道错误)
	ERROR_NUMBER_STRING(EUSERS,用户过多)
	ERROR_NUMBER_STRING(ENOTSOCK,对非套接字进行套接字操作)
	ERROR_NUMBER_STRING(EDESTADDRREQ,需要目标地址)
	ERROR_NUMBER_STRING(EMSGSIZE,消息太长)
	ERROR_NUMBER_STRING(EPROTOTYPE,套接字的协议类型错误)
	ERROR_NUMBER_STRING(ENOPROTOOPT,协议不可用)
	ERROR_NUMBER_STRING(EPROTONOSUPPORT,协议不支持)
	ERROR_NUMBER_STRING(ESOCKTNOSUPPORT,套接字类型不支持)
	ERROR_NUMBER_STRING(EOPNOTSUPP,传输终点不支持操作)
	ERROR_NUMBER_STRING(EPFNOSUPPORT,协议家族不支持)
	ERROR_NUMBER_STRING(EAFNOSUPPORT,地址家族不被协议支持)
	ERROR_NUMBER_STRING(EADDRINUSE,地址已被使用)
	ERROR_NUMBER_STRING(EADDRNOTAVAIL,无法关联请求地址)
	ERROR_NUMBER_STRING(ENETDOWN,网络中断)
	ERROR_NUMBER_STRING(ENETUNREACH,网络不可达)
	ERROR_NUMBER_STRING(ENETRESET,网络因重置而断开连接)
	ERROR_NUMBER_STRING(ECONNABORTED,软件引起的连接终止)
	ERROR_NUMBER_STRING(ECONNRESET,对等端重置连接)
	ERROR_NUMBER_STRING(ENOBUFS,无缓存空间可用)
	ERROR_NUMBER_STRING(EISCONN,传输终点已连接)
	ERROR_NUMBER_STRING(ENOTCONN,传输终点未连接)
	ERROR_NUMBER_STRING(ESHUTDOWN,传输终点关闭后无法发送)
	ERROR_NUMBER_STRING(ETOOMANYREFS,引用太多无法拼接)
	ERROR_NUMBER_STRING(ETIMEDOUT,连接超时)
	ERROR_NUMBER_STRING(ECONNREFUSED,连接被拒绝)
	ERROR_NUMBER_STRING(EHOSTDOWN,主机已关闭)
	ERROR_NUMBER_STRING(EHOSTUNREACH,无到主机的路由)
	ERROR_NUMBER_STRING(EALREADY,操作已经在进行)
	ERROR_NUMBER_STRING(EINPROGRESS,操作现在在进行)
	ERROR_NUMBER_STRING(ESTALE,过时的文件句柄)
	ERROR_NUMBER_STRING(EUCLEAN,结构需要清理)
	ERROR_NUMBER_STRING(ENOTNAM,非Xenix命名的类型文件)
	ERROR_NUMBER_STRING(ENAVAIL,无Xenix信号量可用)
	ERROR_NUMBER_STRING(EISNAM,是命名类型文件)
	ERROR_NUMBER_STRING(EREMOTEIO,远程输入输出错误)
	ERROR_NUMBER_STRING(EDQUOT,超出配额)
	ERROR_NUMBER_STRING(ENOMEDIUM,未找到媒介)
	ERROR_NUMBER_STRING(EMEDIUMTYPE,错误媒介类型)
	ERROR_NUMBER_STRING(ECANCELED,操作取消)
	ERROR_NUMBER_STRING(ENOKEY,所需密钥不可用)
	ERROR_NUMBER_STRING(EKEYEXPIRED,密钥超时)
	ERROR_NUMBER_STRING(EKEYREVOKED,密钥已废除)
	ERROR_NUMBER_STRING(EKEYREJECTED,密钥被服务拒绝)
	ERROR_NUMBER_STRING(EOWNERDEAD,所有者死亡)
	ERROR_NUMBER_STRING(ENOTRECOVERABLE,状态不可恢复)
	ERROR_NUMBER_STRING(ERFKILL,由于rfkill而不可操作)
	ERROR_NUMBER_STRING(EHWPOISON,内存页存在硬件错误)
#endif
}

const char* ErrorNumber::getErrorString(int errorNumber){
	if(errorNumber>=0 && errorNumber<MAX_STRING_AMOUNT)return errorString[errorNumber];
	else return "未定义的错误";
}