#ifndef __GAME_DATA_SET_INTERNAL_H__
#define __GAME_DATA_SET_INTERNAL_H__

#include "GameDataSet.h"

struct CResourceInfo
{
	char szFile[128];
};

struct C3DObjInfo
{
	OBJID	id;
	C3DObj*	p3DObj;
	C3DMotion* pDefaultMotion;			// Note:只有使用权,不得管理这个C3DMotion*
	DWORD	timeBirth;
	DWORD	timeLife;
	C3DObjInfo() : id(0), timeBirth(0), timeLife(0), p3DObj(NULL), pDefaultMotion(NULL) {}
	~C3DObjInfo();
};

struct C3DMotionInfo
{
	int64_t	id;
	C3DMotion* p3DMotion;
	DWORD	timeBirth;
	DWORD	timeLife;
	C3DMotionInfo() : id(0), p3DMotion(NULL), timeBirth(0), timeLife(0) {}
	~C3DMotionInfo();
};

struct C3DTextureInfo
{
	OBJID	id;
	C3DTexture* p3DTexture;
	DWORD	timeBirth;
	DWORD	timeLife;
	C3DTextureInfo() : id(0), timeBirth(0), timeLife(0), p3DTexture(NULL) {}
	~C3DTextureInfo();
};

struct C3DEffectInfo
{
	OBJID	id;
	C3DEffect* p3DEffect;
	DWORD	timeBirth;
	DWORD	timeLife;
	C3DEffectInfo() : id(0), timeBirth(0), timeLife(0), p3DEffect(NULL) {}
	~C3DEffectInfo();
};

struct C3DSceneRef				// 对实际资源的引用对象
{	
	OBJID	  id;
	C3DScene* p3DScene;
	DWORD	  dwTimeBirth;
	DWORD	  dwTimeLife;
	C3DSceneRef() : id(0), dwTimeBirth(0), dwTimeLife(0), p3DScene(NULL) {}
	~C3DSceneRef();
};

struct CAniRef					// 对实际资源的引用对象
{								
	OBJID	id;
	CAni*	pAni;
	DWORD	dwTimeBirth;
	DWORD	dwTimeLife;
	bool	bTryLoaded;			// 不论是否成功,已尝试加载过.
	CAniRef() : bTryLoaded(false), id(0), dwTimeBirth(0), dwTimeLife(0), pAni(NULL) {}
	~CAniRef();
};

#endif//__GAME_DATA_SET_INTERNAL_H__