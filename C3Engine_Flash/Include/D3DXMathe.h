#ifndef __D3DXMATHE_H__
#define __D3DXMATHE_H__

#include "DXPrimitive.h"

/**************************************************************
**  Code From M$'s d3dx8.inl
**  Note: 注意D3DXMATRIX按DX习惯是行主矩阵,注意到OpenGL的矩阵
**        在内存中的规范实际上是行主存放,两者在内存排列上等价.
**************************************************************/
extern "C" inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pOut)
{
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
	pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
	pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
    pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
    return pOut;
}

extern "C" inline D3DXVECTOR3* D3DXVec3Subtract(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
{
	pOut->x = pV1->x - pV2->x;
    pOut->y = pV1->y - pV2->y;
    pOut->z = pV1->z - pV2->z;
    return pOut;
}

extern "C" inline D3DXVECTOR3* D3DXVec3Add(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
{
	pOut->x = pV1->x + pV2->x;
    pOut->y = pV1->y + pV2->y;
    pOut->z = pV1->z + pV2->z;
    return pOut;
}

extern "C" inline D3DXVECTOR3* D3DXVec3Lerp(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2, float s)
{
	pOut->x = pV1->x + s * (pV2->x - pV1->x);
    pOut->y = pV1->y + s * (pV2->y - pV1->y);
    pOut->z = pV1->z + s * (pV2->z - pV1->z);
    return pOut;
}

extern "C" inline D3DXVECTOR3* D3DXVec3Normalize(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV)
{
	float f = 1.0f / sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
	pOut->x = pV->x * f;
	pOut->y = pV->y * f;
	pOut->z = pV->z * f;
	return pOut;
}

extern "C" inline D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2* pOut, const D3DXVECTOR2* pV)
{
	float f = 1.0f / sqrtf(pV->x * pV->x + pV->y * pV->y );
	pOut->x = pV->x * f;
	pOut->y = pV->y * f;
	return pOut;
}


extern "C" inline D3DXVECTOR3* D3DXVec3Cross(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
{
    D3DXVECTOR3 v;
    v.x = pV1->y * pV2->z - pV1->z * pV2->y;
    v.y = pV1->z * pV2->x - pV1->x * pV2->z;
    v.z = pV1->x * pV2->y - pV1->y * pV2->x;
    *pOut = v;
    return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* pOut, float x, float y, float z)
{
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
	pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
	pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] = 0.0f;
    pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
	pOut->m[3][0] = x;
	pOut->m[3][1] = y;
	pOut->m[3][2] = z;
    return pOut;
}

extern "C" inline float D3DXVec3Dot(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

extern "C" inline float D3DXVec4Dot(const D3DXVECTOR4* pV1, const D3DXVECTOR4* pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
}

extern "C" inline D3DXVECTOR4* D3DXVec4Normalize(D3DXVECTOR4* pOut, const D3DXVECTOR4* pV)
{
	float f = 1.0f / sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
	pOut->x = pV->x * f;
	pOut->y = pV->y * f;
	pOut->z = pV->z * f;
	pOut->w = pV->w * f;
	return pOut;
}

extern "C" inline float D3DXVec3Length(const D3DXVECTOR3 *pV)
{
	return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

extern "C" inline D3DXQUATERNION* D3DXQuaternionSlerp(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float t)
{
	float cosa = D3DXVec4Dot((const D3DXVECTOR4*)pQ1, (const D3DXVECTOR4*)pQ2);
	
	// If B is on the oposite hemisphere use -B instead of B
	float sign_of_t1 = 1.0f;
	if(cosa < 0.0f)
	{	
		cosa = -cosa;
		sign_of_t1 = -1.0f;
	}
	
	if(cosa < 1.0f - MATHE_EPSILON)
	{	
		float a = mathe::acos(cosa);
		float isa = mathe::sqrt_inv(1.0f - cosa * cosa);
		float ta = t * a;
		
		float t0 = mathe::sin(a - ta) * isa;
		float t1 = mathe::sin(ta) * isa;
		
		pOut->x = pQ1->x * t0 + pQ2->x * t1 * sign_of_t1;
		pOut->y = pQ1->y * t0 + pQ2->y * t1 * sign_of_t1;
		pOut->z = pQ1->z * t0 + pQ2->z * t1 * sign_of_t1;
		pOut->w = pQ1->w * t0 + pQ2->w * t1 * sign_of_t1;
	}	
	else
	{	
		// If q0 is nearly the same as q1 we just linearly interpolate
		float t0 = 1.0f - t;
		pOut->x = pQ1->x * t0 + pQ2->x * t * sign_of_t1;
		pOut->y = pQ1->y * t0 + pQ2->y * t * sign_of_t1;
		pOut->z = pQ1->z * t0 + pQ2->z * t * sign_of_t1;
		pOut->w = pQ1->w * t0 + pQ2->w * t * sign_of_t1;
	}
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX* pOut, float Angle)
{
	float sa, ca;
	mathe::sincos(Angle, &sa, &ca);
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] = 
	pOut->m[1][0] = pOut->m[1][3] = 
	pOut->m[2][0] = pOut->m[2][3] = 
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;

    pOut->m[1][1] = pOut->m[2][2] = ca;
	pOut->m[1][2] = sa;
	pOut->m[2][1] =-sa;
	pOut->m[0][0] = pOut->m[3][3] = 1.0f;
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX* pOut, float Angle)
{
	float sa, ca;
	mathe::sincos(Angle, &sa, &ca);
    pOut->m[0][1] = pOut->m[0][3] = 
	pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] = 
	pOut->m[2][1] = pOut->m[2][3] = 
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
	
    pOut->m[0][0] = pOut->m[2][2] = ca;
	pOut->m[0][2] = -sa;
	pOut->m[2][0] =  sa;
	pOut->m[1][1] = pOut->m[3][3] = 1.0f;
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX* pOut, float Angle)
{	
	float sa, ca;
	mathe::sincos(Angle, &sa, &ca);
    pOut->m[0][2] = pOut->m[0][3] = 
	pOut->m[1][2] = pOut->m[1][3] = 
	pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
	
    pOut->m[0][0] = pOut->m[1][1] = ca;
	pOut->m[0][1] = sa;
	pOut->m[1][0] =-sa;
	pOut->m[2][2] = pOut->m[3][3] = 1.0f;
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz)
{
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
	pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
	pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
    pOut->m[0][0] = sx;
	pOut->m[1][1] = sy;
	pOut->m[2][2] = sz;
	pOut->m[3][3] = 1.0f;
    return pOut;
}

extern "C" inline D3DXQUATERNION* D3DXQuaternionRotationAxis(D3DXQUATERNION* pOut, const D3DXVECTOR3* pV, float Angle)
{
	D3DXVECTOR3 vAxis;
	::D3DXVec3Normalize(&vAxis, pV); 
	
	float sa, ca;
	mathe::sincos(0.5f * Angle, &sa, &ca);
	pOut->x =  sa * vAxis.x;
	pOut->y =  sa * vAxis.y;
	pOut->z =  sa * vAxis.z;
	pOut->w =  ca;
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixRotationQuaternion(D3DXMATRIX *pOut, const D3DXQUATERNION* pQ)
{
	float x2  = pQ->x*2;	float y2  = pQ->y*2;	float z2  = pQ->z*2; 
	float xx2 = x2*pQ->x;	float yy2 = y2*pQ->y;	float zz2 = z2*pQ->z;
	float xy2 = x2*pQ->y;	float zw2 = z2*pQ->w;	float xz2 = x2*pQ->z;
	float yw2 = y2*pQ->w;	float yz2 = y2*pQ->z;	float xw2 = x2*pQ->w;
	
	pOut->m[0][0] = 1-yy2-zz2;
	pOut->m[1][0] =  xy2-zw2;
	pOut->m[2][0] =  xz2+yw2;	
	
	pOut->m[0][1] = xy2+zw2;
	pOut->m[1][1] = 1-xx2-zz2;
	pOut->m[2][1] = yz2-xw2;	 

	pOut->m[0][2] = xz2-yw2;
	pOut->m[1][2] = yz2+xw2;
	pOut->m[2][2] = 1-xx2-yy2;

	pOut->m[0][3] = pOut->m[1][3] = pOut->m[2][3] = 
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
	pOut->m[3][3] = 1.0;
	return pOut;
}

extern "C" inline D3DXQUATERNION* D3DXQuaternionRotationMatrix(D3DXQUATERNION* pOut, const D3DXMATRIX* pM)
{
	float fRoot;
	float fTrace = pM->m[0][0] + pM->m[1][1] + pM->m[2][2];
	if(fTrace > 0)
	{	
		fRoot = sqrtf(fTrace + 1.0f);
		pOut->w	 = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		pOut->x = (pM->m[1][2] - pM->m[2][1]) * fRoot;
		pOut->y = (pM->m[2][0] - pM->m[0][2]) * fRoot;
		pOut->z = (pM->m[0][1] - pM->m[1][0]) * fRoot;
	}
	else// |w| <= 1/2
	{	
		const int iNext[3] = { 1, 2, 0 };
		
		int i = 0;
		float* fMat = (float*)pM;
		if(pM->m[1][1] > pM->m[0][0])	i = 1;
		if(pM->m[2][2] > fMat[i * 5])	i = 2;

		int j = iNext[i];
		int k = iNext[j];
		
		fRoot = sqrtf(fMat[i*4] - fMat[j*4] - fMat[k*4] + 1);
		float* aq[3] = { &pOut->x, &pOut->y, &pOut->z };
		
		*aq[i] = -0.5f * fRoot;

		fRoot  =  0.5f / fRoot;
		*aq[j] = -(fMat[j*3+i] + fMat[i*3+j]) * fRoot;
		*aq[k] = -(fMat[k*3+i] + fMat[i*3+k]) * fRoot;
		pOut->w=  (fMat[k*3+j] - fMat[j*3+k]) * fRoot;
    }	
	return pOut;
}

extern "C" inline D3DXMATRIX* D3DXMatrixRotationAxis(D3DXMATRIX* pOut, const D3DXVECTOR3* pAxis, float fAngle)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, pAxis, fAngle);
	D3DXMatrixRotationQuaternion(pOut, &q);
	return pOut;
}

extern "C" inline D3DXVECTOR2* D3DXVec2TransformCoord(D3DXVECTOR2* pOut, const D3DXVECTOR2* pV, const D3DXMATRIX* pM)
{
	float x = pM->m[0][0] * pV->x + pM->m[1][0] * pV->y + pM->m[3][0];
	float y = pM->m[0][1] * pV->x + pM->m[1][1] * pV->y + pM->m[3][1];
	pOut->x = x;
	pOut->y = y;
	return pOut;
}

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

extern "C" C3_CORE_DLL_API D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2);
extern "C" C3_CORE_DLL_API float D3DXMatrixfDeterminant(const D3DXMATRIX* pM);
extern "C" C3_CORE_DLL_API D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* pOut, float *pDeterminant, const D3DXMATRIX* pM);
extern "C" C3_CORE_DLL_API D3DXVECTOR4* D3DXVec3Transform(D3DXVECTOR4* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX *pM);
extern "C" C3_CORE_DLL_API D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
extern "C" C3_CORE_DLL_API D3DXVECTOR3* D3DXVec3RigidTransformCoord(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
extern "C" C3_CORE_DLL_API D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
extern "C" C3_CORE_DLL_API D3DXMATRIX* D3DXMatrixTransformation(D3DXMATRIX* pOut, const D3DXVECTOR3* pScalingCenter, const D3DXQUATERNION* pScalingRotation, const D3DXVECTOR3* pScaling, const D3DXVECTOR3* pRotationCenter, const D3DXQUATERNION* pRotation, const D3DXVECTOR3* pTranslation);

#endif//__D3DXMATHE_H__