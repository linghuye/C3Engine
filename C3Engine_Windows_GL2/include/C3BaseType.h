#ifndef __C3_BASE_TYPE_H__
#define __C3_BASE_TYPE_H__

#ifdef _WIN32
#pragma warning(disable : 4786)
#include <Windows.h>
#include <float.h>
#include <MMSystem.h>
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else// unix

#include <inttypes.h>
#include <float.h>

typedef long LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;

#ifndef NULL
#define NULL 0
#endif//NULL

#endif//_WIN32

#endif//__C3_BASE_TYPE_H__