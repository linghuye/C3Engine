#ifndef __DX_PRIMITIVES_H__
#define __DX_PRIMITIVES_H__

#pragma once
#include "C3Mathe.h"

/******************************************************************
**  自定义enum类型
******************************************************************/
typedef enum 
{
	D3DLIGHT_POINT          = 1,
	D3DLIGHT_SPOT           = 2,
	D3DLIGHT_DIRECTIONAL    = 3,
}	D3DLIGHTTYPE;

typedef enum 
{
       D3DBLEND_ZERO               = 1,
	D3DBLEND_ONE                = 2,
	D3DBLEND_SRCCOLOR           = 3,
	D3DBLEND_INVSRCCOLOR        = 4,
	D3DBLEND_SRCALPHA           = 5,
	D3DBLEND_INVSRCALPHA        = 6,
	D3DBLEND_DESTALPHA          = 7,
	D3DBLEND_INVDESTALPHA       = 8,
	D3DBLEND_DESTCOLOR          = 9,
	D3DBLEND_INVDESTCOLOR       = 10,
}	D3DBLEND;

/***********************************************************
**  自定义数据结构以兼容原有DX的数据结构方式,以减少代码变动.
***********************************************************/
struct D3DVECTOR
{
	float x, y, z;
};

struct D3DXVECTOR3 : public D3DVECTOR
{
	D3DXVECTOR3()
	{
	}

	D3DXVECTOR3(float x, float y, float z)
	{
		set(x, y, z);
	}

	D3DXVECTOR3(const D3DVECTOR& v)
	{
		set(v.x, v.y, v.z);
	}

	void set(float ix, float iy, float iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}
	
	void set3(const float *pv)
	{
		set(pv[0], pv[1], pv[2]);
	}
	
	void set_zero3()
	{
		x = y = z = 0;
	}
		
    D3DXVECTOR3& normalize3()
	{	
		operator *= (length3_inv());
		return *this;
  	}	
	
	void normalize()
	{
		normalize3();
	}
	
	D3DXVECTOR3& operator=(float v)
	{
		x = y = z = v;
		return *this;
	}

	bool IsfloatEqual(const D3DXVECTOR3& v3) const
	{
		return (fabs(x - v3.x) < FLT_EPSILON && 
				fabs(y - v3.y) < FLT_EPSILON && 
				fabs(z - v3.z) < FLT_EPSILON);
	}
		
	void set_mul3(float r, const D3DXVECTOR3& v3)
	{
		x = r * v3.x;
		y = r * v3.y;
		z = r * v3.z;
	}
	
	void add_mul3(float r, const D3DXVECTOR3& v3)
	{
		x += r * v3.x;
		y += r * v3.y;
		z += r * v3.z;
	}

	void set_cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		float a  = v1(1)*v2(2);
		float a2 = v2(1)*v1(2);
		float b  = v1(2)*v2(0);	
		float b2 = v2(2)*v1(0);	a -= a2;
		float c  = v1(0)*v2(1);
		float c2 = v2(0)*v1(1);	b -= b2;
		x = a;
		y = b;					c -= c2;
		z = c;  
	}
	
	void set_interpolate(const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, float t)
	{	
		//线性插值
		float s = 1.0f - t;
		x = s * v0.x + t * v1.x;
		y = s * v0.y + t * v1.y;
		z = s * v0.z + t * v1.z;
	}
	
	//数学获取
    float& operator[](int i)
	{ 
		return (&x)[i];
	}

	const float& operator[](int i) const
	{ 
		return (&x)[i];
	}
	
	float& operator()(int i)
	{
		return (&x)[i];
	}
	
	const float& operator()(int i) const
	{ 
		return (&x)[i];
	}

	operator const float*() const 
	{
		return &x;
	}
	
	operator float*()
	{
		return &x;
	}
			
	float Z()
	{
		return z;
	}
	
   	//数学计算
    void operator+=(const D3DXVECTOR3& v3)
	{ 
		x += v3.x;
		y += v3.y;
		z += v3.z;
	}
		
	void operator-=(const D3DXVECTOR3& v3)
	{ 
		x -= v3.x;
		y -= v3.y;
		z -= v3.z;
	}
	
	void operator*=(float s)
	{ 
		x *= s;
		y *= s;
		z *= s;
	}

	void operator*=(const D3DXVECTOR3& v3)
	{ 
		x *= v3.x;
		y *= v3.y;
		z *= v3.z;
	}

	D3DXVECTOR3 operator/=(const D3DXVECTOR3& v3) const
	{ 
		return D3DXVECTOR3(x/v3.x, y/v3.y, z/v3.z);
	}

    D3DXVECTOR3 operator+(const D3DXVECTOR3& v3) const
	{ 
		return D3DXVECTOR3(x + v3.x, y + v3.y, z + v3.z); 
	}
	
    D3DXVECTOR3 operator-(const D3DXVECTOR3& v3) const
	{ 
		return D3DXVECTOR3(x - v3.x, y - v3.y, z - v3.z);
	}
	
	D3DXVECTOR3 operator*(float s) const
	{ 
		return D3DXVECTOR3(x * s, y * s, z * s);
	}
	
	D3DXVECTOR3 operator/(float s) const
	{ 
		return D3DXVECTOR3(x/s, y/s, z/s);
	}
	
	float operator*(const D3DXVECTOR3& v3) const
	{ 
		return (x * v3.x + y * v3.y + z * v3.z);
	} 
	
	D3DXVECTOR3 operator~() const
	{ 
		return D3DXVECTOR3(-x, -y, -z); 
	}

	D3DXVECTOR3 operator-() const
	{
		return D3DXVECTOR3(-x, -y, -z);
	}
		
    D3DXVECTOR3 operator^(const D3DXVECTOR3& v3) const
	{	
		//叉积
		return D3DXVECTOR3(y*v3.z - v3.y*z, -x*v3.z + v3.x*z, x*v3.y - v3.x*y);
	}	
		
	//数学求值
	float dot3(const D3DXVECTOR3& v) const
	{	
		//点积
		//公式: ||u|| * ||v|| * cos(q)
		//Dot product describes the relation between two vectors in scalar form.
		return (x*v.x + y*v.y + z*v.z);
	}	

    D3DXVECTOR3 cross3(const D3DXVECTOR3& v3) const
	{		
		//叉积
		//返回的结果向量与两个乘数向量都垂直.
		return D3DXVECTOR3(y*v3.z - v3.y*z, -x*v3.z + v3.x*z, x*v3.y - v3.x*y);
	}	
	
	D3DXVECTOR3 cross_unit(const D3DXVECTOR3& v3) const
	{	
		D3DXVECTOR3 r(y*v3.z - v3.y*z, -x*v3.z + v3.x*z, x*v3.y - v3.x*y);
		r.normalize3();
		return r;
	}
	
	// 投影到任意向量v
	D3DXVECTOR3 project(const D3DXVECTOR3& v) const
	{	
		return v * (v.dot3(*this) / v.square3());
	}
	
	float length3() const
	{	
		return magnitude3();
	}	

	float length3_inv() const
	{
		return 1.0f / sqrtf(x * x + y * y + z * z);
	}
	
	float magnitude3() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	
	float magnitude3_inv() const
	{
		return float(1)/magnitude3();
	}
	
	float square3() const
	{
		return (x*x + y*y + z*z);
	}
	
	float length_squared() const
	{
		return square3();
	}
	
	float distance_squared_to(const D3DXVECTOR3& v) const
	{
		float a = x - v.x;
		float b = y - v.y;
		float c = z - v.z;
		return (a*a + b*b + c*c);
	}	
	
	float distance_to(const D3DXVECTOR3& v) const
	{
		return mathe::sqrt(distance_squared_to(v));
	}

	float magnitude(const D3DXVECTOR3& v) const
	{
		return (*this - v).magnitude3();
	}
};

struct D3DXVECTOR2
{
	D3DXVECTOR2()
	{
	}

	D3DXVECTOR2(float fx, float fy) : x(fx), y(fy)
	{
	}

	float x, y;
};

struct D3DXVECTOR4
{
	D3DXVECTOR4()
	{
	}

	D3DXVECTOR4(float x, float y, float z, float w = 1.0f)
	{
		set(x, y, z, w);
	}
	
	void set(float ix, float iy, float iz, float iw)
	{
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
	
	void set4(const float* fv)
	{
		x = fv[0];
		y = fv[1];
		z = fv[2];
		w = fv[3];
	}
	
	operator const float*() const 
	{
		return &x;
	}

	D3DXVECTOR4 operator-() const
	{
		return D3DXVECTOR4(-x, -y, -z, -w);
	}
	
	float x;
	float y;
	float z;
	float w;
};

struct D3DCOLORVALUE 
{
    float r;
    float g;
    float b;
    float a;

	operator const float*() const
	{
		return &r;
	}
	
	D3DCOLORVALUE operator*(const D3DCOLORVALUE& s) const
	{
		D3DCOLORVALUE t;
		t.r = r * s.r;
		t.g = g * s.g;
		t.b = b * s.b;
		t.a = a * s.a;
		return t;
	}
};

struct D3DXCOLOR : public D3DCOLORVALUE
{
	D3DXCOLOR()
	{
	}

	D3DXCOLOR(float fr, float fg, float fb, float fa)
	{
		r = fr;
		g = fg;
		b = fb;
		a = fa;
	}

	D3DXCOLOR(DWORD dw)
	{
		const float f = 1.0f / 255.0f;
		r = f * (float) (unsigned char) (dw >> 16);
		g = f * (float) (unsigned char) (dw >>  8);
		b = f * (float) (unsigned char) (dw >>  0);
		a = f * (float) (unsigned char) (dw >> 24);
	}
	
	D3DXCOLOR(const D3DCOLORVALUE& s)
	{
		r = s.r;
		g = s.g;
		b = s.b;
		a = s.a;
	}
	
	D3DXCOLOR& operator=(const D3DCOLORVALUE& s)
	{
		r = s.r;
		g = s.g;
		b = s.b;
		a = s.a;
		return *this;
	}
	
	D3DXCOLOR operator*(const D3DXCOLOR& s)
	{
		return D3DXCOLOR(r * s.r, g * s.g, b * s.b, a * s.a);
	}
	
	D3DXCOLOR operator*(const D3DCOLORVALUE& s)
	{
		return D3DXCOLOR(r * s.r, g * s.g, b * s.b, a * s.a);
	}
	
	D3DXCOLOR& operator+=(const D3DCOLORVALUE& s)
	{
		r += s.r;
		g += s.g;
		b += s.b;
		return *this;
	}
	

	operator const float*() const 
	{
		return &r;
	}
	
	operator float*()
	{
		return &r;
	}
	
	operator DWORD() const
	{
		return (DWORD(a * 255) << 24) | (DWORD(b * 255) << 16) | (DWORD(g * 255) << 8) | (DWORD(r * 255));
	}
	
	inline void Set(float fr, float fg, float fb, float fa)
	{
		r = fr;
		g = fg;
		b = fb;
		a = fa;
	}
};

//////////////////////////////////////////////////////////////////////////////////
struct D3DXMATRIX
{
public:
	D3DXMATRIX()
	{
	}
	
	D3DXMATRIX( float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24,
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44 )
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	}

	operator float* ()
	{
		return &m[0][0];
	}

    operator const float* () const
	{
		return &m[0][0];
	}
			
    D3DXMATRIX& operator += ( const D3DXMATRIX& mat)
	{
		_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
		_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
		_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
		_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
		return *this;
	}

    D3DXMATRIX& operator -= ( const D3DXMATRIX& mat)
	{
		_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
		_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
		_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
		_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
		return *this;
	}

    D3DXMATRIX& operator *= ( float f)
	{
		_11 *= f; _12 *= f; _13 *= f; _14 *= f;
		_21 *= f; _22 *= f; _23 *= f; _24 *= f;
		_31 *= f; _32 *= f; _33 *= f; _34 *= f;
		_41 *= f; _42 *= f; _43 *= f; _44 *= f;
		return *this;
	}

    D3DXMATRIX& operator /= ( float f)
	{
		float fInv = 1.0f / f;
		_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
		_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
		_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
		_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
		return *this;
	}
	
    inline D3DXMATRIX operator + (const D3DXMATRIX& mat) const
	{
		return D3DXMATRIX(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
						  _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
						  _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
						  _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
	}
	
    inline D3DXMATRIX operator - (const D3DXMATRIX& mat) const
	{
		return D3DXMATRIX(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
						  _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
						  _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
						  _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
	}

    D3DXMATRIX operator * (float f) const
	{
		return D3DXMATRIX(_11 * f, _12 * f, _13 * f, _14 * f,
						  _21 * f, _22 * f, _23 * f, _24 * f,
						  _31 * f, _32 * f, _33 * f, _34 * f,
						  _41 * f, _42 * f, _43 * f, _44 * f);
	}

    D3DXMATRIX operator / (float f) const
	{
		float fInv = 1.0f / f;
		return D3DXMATRIX(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
						  _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
						  _31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
						  _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
	}

	D3DXMATRIX& operator *= (const D3DXMATRIX& mat)
	{
		D3DXMATRIX dst;
		
		dst.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
		dst.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
		dst.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
		dst.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];
		
		dst.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
		dst.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
		dst.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
		dst.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];
		
		dst.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
		dst.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
		dst.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
		dst.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];
		
		dst.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
		dst.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
		dst.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
		dst.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];
		
		*this = dst;
		return *this;
	}

    // binary operators
    D3DXMATRIX operator * (const D3DXMATRIX& mat) const
	{
		D3DXMATRIX dst;
		dst.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
		dst.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
		dst.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
		dst.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];
		
		dst.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
		dst.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
		dst.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
		dst.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];
		
		dst.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
		dst.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
		dst.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
		dst.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];
		
		dst.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
		dst.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
		dst.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
		dst.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];
		return dst;
	}

public:
	union 
	{
		struct 
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
    };
};

struct D3DMATERIAL
{
	D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
    D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
    D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
    D3DCOLORVALUE   Emissive;       /* Emissive color RGB */
    float           Power;          /* Sharpness if specular highlight */
};

struct D3DXQUATERNION
{
	D3DXQUATERNION()
	{
	}

	D3DXQUATERNION(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw)
	{
	}

	float x, y, z, w;
};

/******************************************************************
**  自定义数据以兼容DirectX的数据结构
******************************************************************/
#define D3DX_PI    ((float)  3.141592654f)
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
#define D3DCOLOR_ARGB(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_ABGR(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
#define ARGB2ABGR(x) (((x)&0xff000000) | (((x)&0xff0000)>>16) | ((x)&0xff00) | ((x)&0xff)<<16)

#endif//__DX_PRIMITIVES_H__