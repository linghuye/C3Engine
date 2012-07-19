#ifndef __IDestructable2D_H__
#define __IDestructable2D_H__

struct C3DSTRayCastResult
{
	bool m_bHit;
	float m_vPos[2];
	float m_vNorm[2];
};

class IDestructable2D
{
public:
	virtual void Release() = 0;
	
	//------------------------------------------------------------------------------
	//Destruct
	//------------------------------------------------------------------------------
	//x = [0,DispWidth] left to right
	//y = [0,DispHeight] up to bottom
	//fAngle : in Degree
	virtual void DestructHole(float x, float y, float fRotateDegree
		, C3Texture* pBrushTex
		, C3Texture* pBrushDecalTex
		, float fBrushScale = 1.0f) = 0;
	
	//------------------------------------------------------------------------------
	//Collision Query
	//------------------------------------------------------------------------------
	//x = [0,DispWidth] left to right     
	//y = [0,DispHeight] up to bottom
	virtual void RayCast(C3DSTRayCastResult* pResult, float fTexX, float fTexY) = 0;
	
	//------------------------------------------------------------------------------
	//Display
	//------------------------------------------------------------------------------
	//Edge size in pixel
	virtual void	SetEdgeSize(float fSize) = 0;
	
	virtual float	GetEdgeSize() const = 0;

	virtual void	ResetMap() = 0;

	//Default [DispWidth,DispHeight] = BGTex size
	virtual void	SetDisplayWidth(int nWidth) = 0;
	
	virtual void	SetDisplayHeight(int nHeight) = 0;
	
	virtual int		GetDisplayWidth() const = 0;
	
	virtual int		GetDisplayHeight() const = 0;
	
	virtual void	Draw(float xOffset = 0, float yOffset = 0) = 0;
};

extern "C" GRAPHIC_API IDestructable2D* Destructable2DCreate(C3Texture* pBGTex, C3Texture* pMaskTex, int nDSTColWidth = 0, int nDSTColHeight = 0,float fEdgeSize = 1.0f);

#endif //__IDestructable2D_H__