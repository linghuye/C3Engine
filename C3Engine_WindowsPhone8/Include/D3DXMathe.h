#ifndef __D3DXMATHE_H__
#define __D3DXMATHE_H__

#include "DXPrimitive.h"

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

extern "C" C3_CORE_DLL_API D3DXVECTOR3* D3DXVec3RigidTransformCoord(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);

#endif//__D3DXMATHE_H__