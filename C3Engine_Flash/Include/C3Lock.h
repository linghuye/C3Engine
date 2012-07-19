#ifndef _C3_LOCK_H
#define _C3_LOCK_H
#include "EngineBuildOption.h"

class CMyCs
{
public:
	CMyCs();
	~CMyCs();
	void Lock();
	void Unlock();
	void Destroy();
	
public:
	void* m_ptrNSLock;
};

class CMyLock
{
public:
	CMyLock(CMyCs& cs) : m_cs(cs) 
	{
		m_cs.Lock(); 
	}
	
	~CMyLock() 
	{
		m_cs.Unlock(); 
	}

private:
	CMyCs& m_cs;
};

#endif//_C3_LOCK_H