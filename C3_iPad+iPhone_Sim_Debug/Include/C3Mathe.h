#ifndef __C3_MATHE_H__
#define __C3_MATHE_H__

/***************************************************************************
**  C3简易数学工具
***************************************************************************/
#include <C3BaseType.h>
#include <math.h>

// Windows 定义了min/max宏冲突
#if defined(_WIN32)
#undef min
#undef max
#endif//_WIN32

#ifndef MATHE_PI
#define MATHE_PI        3.14159265358979323846f	/* pi */
#endif

#ifndef MATHE_PI_2
#define MATHE_PI_2      1.57079632679489661923f	/* pi/2 */
#endif

#ifndef MATHE_PI_180
#define MATHE_PI_180	  0.01745329251994329577f	/* pi/180*/
#endif

#ifndef MATHE_180_PI
#define MATHE_180_PI	  57.2957795130823208768f	/* 180/pi*/
#endif

#ifndef MATHE_E
#define MATHE_E		  2.71828182845904f
#endif

#ifndef MATHE_EPSILON
#define MATHE_EPSILON	  1e-6f
#endif

#ifndef MATHE_ZERO_TOLERANCE
#define MATHE_ZERO_TOLERANCE	  1e-6f
#endif

#ifndef RAD2DEG
#define RAD2DEG(x)  (MATHE_180_PI * (x))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  (MATHE_PI_180 * (x))
#endif

struct mathe
{	
public:	
	inline static float sqrt(float r) { return ::sqrtf(r); }

	inline static float sqrt_inv(float r) { return 1.0f/::sqrtf(r); }
		
	inline static float tan(float r) { return tanf(r); }
		
	inline static float sin(float r) { return sinf(r); }
		
	inline static float cos(float r) { return cosf(r); }
		
	inline static float atan2(float a, float b)  { return atan2f(a,b); }
		
	inline static float asin(float r) { return asinf(r); }
		
	inline static float acos(float r) { return acosf(r); }
	
	inline static float max(float a, float b) { return a>b ? a : b; }
	
	inline static float min(float a, float b) { return a<b ? a : b; }
		
	inline static float pow(float x, float y) { return powf(x, y); };
		
	inline static float fabs(float x) { return fabsf(x); };

	inline static void sincos(float radians, float* sine, float* cosine)
	{
		*sine = sinf(radians);
		*cosine = cosf(radians);
	}
};

#endif//__C3_MATHE_H__