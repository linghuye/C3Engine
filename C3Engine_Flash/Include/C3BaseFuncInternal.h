#ifndef _C3_BASE_FUNC_INTERNAL_H_
#define _C3_BASE_FUNC_INTERNAL_H_

#include "C3BaseFunc.h"

#ifndef C3_CORE_DLL_API
#if defined(_WIN32)
#if defined(C3_CORE_DLL_EXPORTS) || defined(GRAPHIC_EXPORTS)
#define C3_CORE_DLL_API __declspec(dllexport)
#else
#define C3_CORE_DLL_API __declspec(dllimport)
#endif
#else// 其他平台都是静态库
#define C3_CORE_DLL_API
#endif
#endif

extern const char* g_pszLogFolderPath;

// 字符串哈希
C3_CORE_DLL_API DWORD Str2ID(const char* pszString);

// 取当前程序运行时间
C3_CORE_DLL_API DWORD TimeGetTime();

// Log
C3_CORE_DLL_API extern "C" void DebugMsg(const char* fmt, ...);
C3_CORE_DLL_API extern "C" void LogMsg(const char* fmt, ...);

#ifndef MYASSERT
#ifdef _DEBUG
#define	MYASSERT(x) assert(x)
#else
#define	MYASSERT(x)	{if(!(x)) LogMsg("ASSERT(" #x ") in %d line of %s", __LINE__, __FILE__); }
#endif
#endif

#ifndef IF_SUC
#ifdef _DEBUG
#define	IF_SUC(x)  if( ((x)) ? true : ( assert(!("IF_SUC: " #x)), false ) )
#else
#define	IF_SUC(x)  if( ((x)) ? true : ( LogMsg("IF_SUC(%s) failed in %s, %d", #x, __FILE__, __LINE__), false ) )
#endif
#endif

#define IF_TRUE	IF_SUC
#define IF_YES	IF_SUC
#define IF_OK	IF_SUC

#ifndef IF_NOT
#ifdef _DEBUG
#define	IF_NOT(x)  if( (!(x)) ? ( assert(!("IF_NOT: " #x)),1 ) : 0 )
#else
#define	IF_NOT(x)  if( (!(x)) ? ( LogMsg("IF_NOT(%s) in %s, %d", #x, __FILE__, __LINE__),1 ) : 0 )
#endif
#endif

#endif//_C3_BASE_FUNC_INTERNAL_H_