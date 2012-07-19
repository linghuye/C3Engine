// ***************************************************************
//  File Name:	IDataThread.h
//  Create By:	Yuhang.Huang
//	Create On:	2006/10/01/19:01:36
//	Describ:	Todo
//	Note:		Todo
//  Copyright (C) 2006 Tqdigital - All Rights Reserved
// ***************************************************************
#ifndef IDataThread_h__Program_Once
#define IDataThread_h__Program_Once
// ***************************************************************
#ifdef DATATHREAD_EXPORTS
	#define DATATHREAD_API __declspec(dllexport)
#else
	#define DATATHREAD_API __declspec(dllimport)
// ***************************************************************
#endif
#include <stdio.h>
#include <windows.h>
// ***************************************************************
enum
{
	DATA_THREAD_EXIGENCE_NORMAL = 0,
	DATA_THREAD_EXIGENCE_IMMEDIATE = 1,	
};
// ***************************************************************
class	IDataObj
{
public:
	virtual	__int64	GetID(void)					=	0;
	virtual	void	Load(void)					=	0;
	virtual	ULONG	GetExigence(void)			=	0;
	virtual void	DataThreadRelease(void)		=	0;
};

struct IThreadExitCallback
{
	virtual void OnThreadExit() = 0;
};

// ***************************************************************
class	IDataThread
{
public:
	virtual	ULONG		Release(void)								=	0;
	virtual	void		SetThreadPriority(ULONG	uData)				=	0;
	virtual	void		SetImmediateExigence(bool bImmediate)		=	0;
	virtual	IDataObj*	PopUpDataObj(void)							=	0;	
	virtual	IDataObj*	PushBackDataObj(IDataObj*	pObj)			=	0;
	virtual	BOOL		IsFree(void)								=	0;
	virtual	BOOL		Process(void)								=	0;
	virtual int			TestDataObjLoadStatus(__int64 id)			=	0;		// 仅用于测试对象加载所处状态
	virtual void		SetThreadExitCallback(IThreadExitCallback& callback)  = 0;
};

// ** 支持多个独立IDataThread实例创建 ****************************
DATATHREAD_API IDataThread* DataThreadCreateNew(DWORD dwTlsIndex, DWORD dwThreadAppID);
// ***************************************************************
#endif // IDataThread_h__Program_Once