#ifndef __GRAPHIC_INTERNAL_H__
#define __GRAPHIC_INTERNAL_H__

/**********************************************************************************
** 1.graphic_internal.h包含那些仅供内部模块(graphic,graphicdata,Role3D模块)使用的接口,
**   以避免外部误用接口方法.
** 2.DOWRD颜色值格式为0xARGB
**********************************************************************************/
#include "graphic.h"
#include "C3Primitive_internal.h"

extern "C" GRAPHIC_API CAni* AniCreate(const char* pszFile, const char* pszIndex);

extern "C" GRAPHIC_API C3DTexture* TextureCreate(const char* pszFile);

extern "C" GRAPHIC_API C3DMotion* MotionCreate(const char* pszFile, bool bBufferMotion = false);

extern "C" GRAPHIC_API C3DObj* MeshCreate(const char* pszFile, IPhyLoadConfig& phyLoadConfig);

extern "C" GRAPHIC_API C3DEffect* EffectCreate(const char* pszFile);

extern "C" GRAPHIC_API C3DScene* Scene3DCreate(const char* pszC3File);

extern "C" GRAPHIC_API void DestroyShapeInfo(HSHAPEANIMTRACKER& hShapeAnimTracker, int nShapes);

extern "C" GRAPHIC_API void DestroyPtclInfo(HPTCLMOTIONTRACKER& info);

extern "C" GRAPHIC_API void DestroyPtcl3Info(HPTCL3MOTIONTRACKER& info);

// 创建一个C3DObj空壳
extern "C" GRAPHIC_API C3DObj* ProcedureMeshCreate();

extern "C" GRAPHIC_API void ClearAniFileSet();

extern "C" GRAPHIC_API C3DSkeleton* SkeletonCreate(const char* pszFile);

extern "C" GRAPHIC_API C3DDynamicSkeletonInstance* DynamicSkeletonCreate(C3DSkeleton* pSkeleton);

extern "C" GRAPHIC_API void CalculateVirtualMotionMatrix(HC3MOTIONTRACKER hMotionTracker, D3DXMATRIX& matVirtualPhy);

extern "C" GRAPHIC_API void CalculateSkeletonBoneMatrix(C3Motion* pMotion, C3Motion* pBlendMotion, int nBondId, D3DXMATRIX& matBone);

extern "C" GRAPHIC_API C3Camera& GetC3Camera(bool bProject);

extern "C" GRAPHIC_API int AniGetFileName(const char* pszAniFile, const char* pszIndex, const char** arrFileNames, int nNamePointerCount);

extern "C" GRAPHIC_API bool SetRenderThreadContext(DWORD dwAppThreadID);

extern "C" GRAPHIC_API bool ResetRenderThreadContext();

extern "C" GRAPHIC_API void EnableReturnDefaultAni(bool bEnable);
	
#endif//__GRAPHIC_INTERNAL_H__
