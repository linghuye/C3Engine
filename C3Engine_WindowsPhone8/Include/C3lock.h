#ifndef _C3_LOCK_H
#define _C3_LOCK_H

#include <windows.h>

class CMyCs
{	
public:
	CMyCs()
	{	
		::InitializeCriticalSection(&m_cs); 
	}	
	
	~CMyCs()
	{
		::DeleteCriticalSection(&m_cs);
	}
	
	void Lock()
	{
		::EnterCriticalSection(&m_cs); 
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_cs); 
	}
	
private:
	CRITICAL_SECTION	m_cs;
};

class CMyLock
{	
public:
	CMyLock(CMyCs& cs) : m_cs(cs) { m_cs.Lock(); }
	~CMyLock() { m_cs.Unlock(); }
	
private:
	CMyCs&	m_cs;
};

#endif//_C3_LOCK_H