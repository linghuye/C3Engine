// IRoleView.h: interface for the IRoleView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_)
#define AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ROLEVIEW_EXPORTS
#define ROLEVIEW_API __declspec(dllexport)
#else
#define ROLEVIEW_API __declspec(dllimport)
#endif

enum {
	_ROLE_VIEW_NONE			= 0,
	_ROLE_VIEW_ROLE			= 1,
	_ROLE_VIEW_STATUARY		= 2,
	_ROLE_VIEW_TARGET		= 3,
	_ROLE_VIEW_TASKNPC		= 4,
	_ROLE_VIEW_SYNSTONE		= 5,
	_ROLE_VIEW_BOOTH		= 6,
	_ROLE_VIEW_MAPMAGICOBJ	= 7,
	_ROLE_VIEW_DICENPC		= 8,
	_ROLE_VIEW_TERRAIN		= 9,
	_ROLE_VIEW_TASKNPCEX	= 10,
};

enum {
	_VAR_LOOK				= 0,
	_VAR_HAIR,
	_VAR_HEAD,
	_VAR_TRANSFORM,
	_VAR_ARMOR,
	_VAR_ARMET,
	_VAR_RWEAPON,
	_VAR_LWEAPON,
	_VAR_MOUNT,
	_VAR_MISC,
	_VAR_FRAME,
	_VAR_ROTATEX,
	_VAR_RWEAPON_ID,
	_VAR_LWEAPON_ID,
	_VAR_ARMET_ID,
	_VAR_ARMOR_ID,
	_VAR_MOUNT_ID,
	_VAR_MISC_ID,
	_VAR_WEAPON_ACTION_DATA,
	_VAR_DIR,
	_VAR_DIRECT_DIR,
	_VAR_MAX_LIFE,
	_VAR_LIFE,
	_VAR_ITEM0,
	_VAR_ITEM1,
	_VAR_LOOKCLONE,
	_VAR_MOMENT,
	_VAR_DICE0,
	_VAR_DICE1,
	_VAR_DICE2,
	_VAR_ACTION_DATA,
	_VAR_ALPAH_BLEND,
	_VAR_TEX_MIXOPT,
	_VAR_ROTATEY,
	_VAR_ROTATEZ,
};

class IRoleView
{
public:
	virtual unsigned long Release() = 0;
	virtual BOOL	SetVariable(int nVariableType, DWORD dwData)	= 0;
	virtual int		SetAction(int nActionType, BOOL bResetMotion= true, BOOL bPlayerSound = false, int nWorldX=0, int nWorldY=0, BOOL bSetEffect = false)	= 0;
	virtual void	SetPos ( float nX, float nY, float nHeight, int nRotate, float fScale )	= 0;
	virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b ) = 0;
	virtual void	SetRGBA ( float alpha, float red, float green, float blue ) = 0;
	virtual void	AddEffect ( int nPart, const char* pszEffect ) = 0;
	virtual void	ClrEffect ( int nPart, const char* pszEffect ) = 0;
	virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY ) = 0;
	virtual BOOL	Draw ( int nMapX, int nMapY ) = 0;
	virtual DWORD	GetVariable(int nVariableType) = 0;
};

extern "C" ROLEVIEW_API IRoleView*	RoleViewCreate	( int nType );

#endif // !defined(AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_)
