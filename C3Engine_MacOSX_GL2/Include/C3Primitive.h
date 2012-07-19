#ifndef __C3_PRIMITIVE_H__
#define __C3_PRIMITIVE_H__

#include "C3BaseType.h"	// System data typedef
#include "DXPrimitive.h"	// C3 depend on DX structs

typedef void* HEAGLDRAWABLE;
typedef void* HNATIVEWINDOW; 

// 双层纹理混合方式
typedef enum 
{ 
	TEXMIX_NONE					= 2,	// D3DTOP_SELECTARG1,		
	TEXMIX_MODULATE				= 4,	// D3DTOP_MODULATE,				
	TEXMIX_DARK_MAP				= 4,	// D3DTOP_MODULATE
	TEXMIX_MODULATE2X			= 5,	// D3DTOP_MODULATE2X,		
	TEXMIX_MODULATE4X			= 6,	// D3DTOP_MODULATE4X,		
	TEXMIX_ADD					= 7,	// D3DTOP_ADD,					
	TEXMIX_ADDSIGNED			= 8,	// D3DTOP_ADDSIGNED,				
	TEXMIX_ADDSIGNED2X			= 9,	// D3DTOP_ADDSIGNED2X,
	TEXMIX_SUBTRACT				= 10,	// D3DTOP_SUBTRACT,				
	TEXMIX_ADDSMOOTH			= 11,	// D3DTOP_ADDSMOOTH,			
	TEXMIX_DECAL_MAP			= 13,	// D3DTOP_BLENDTEXTUREALPHA,	
	TEXMIX_BLEND_FACTOR_ALPHA	= 14,	// D3DTOP_BLENDFACTORALPHA,		
	TEXMIX_BLEND_PRE_ALPHA		= 15,	// D3DTOP_BLENDTEXTUREALPHAPM,	
	TEXMIX_BLEND_CUR_ALPHA		= 16,	// D3DTOP_BLENDCURRENTALPHA,	
	TEXMIX_REFLECT				= 17,	// 反射贴图
	TEXMIX_REF_BASEALPHA		= 18,
	TEXMIX_SILHOUETTE			= 19,	// 渲染剪影
	TEXMIX_CARTOON_SILHOUETTE	= 20,	// 卡通勾边
	TEXMIX_CARTOON_SHADOW		= 21,	// 卡通色调阴影
	TEXMIX_CARTOON_FULLSTYLE	= 22,	// 卡通勾边+卡通色调阴影
	TEXMIX_FLUID_REFLECT		= 23,	// 流动式反射贴图
	TEXMIX_USE_FIRST_RGB_SECOND_ALPHA = 25,// 使用第1张纹理的RGB,但是取第2张纹理的Alpha.
	TEXMIX_USE_FIRST_RGB_MODULATE_SECOND_ALPHA = 26,// 使用第1张纹理的RGB,但是取第1张纹理和第2张纹理的Alpha相乘值.		
	TEXMIX_BLEND_CLIENT_ALPHA	= 27,	// 在两张贴图间使用Alpha值变化混合,制作渐变效果.
	TEXMIX_DOT3_NORMALMAP		= 28,
	TEXMIX_ENV_SPHEREMAP		= 29,
	TEXMIX_ENV_PROJECTIVESHADOW = 30,
	TEXMIX_ENV_SHADOWMAP		= 31,
}	C3_TEXMIX_TYPE;

// 文字风格
typedef enum 
{
	RENDER_TEXT_SIMPLE = 0, 
	RENDER_TEXT_SHADOW = 1,			// 阴影效果
	RENDER_TEXT_SILHOUETTE = 2,		// 勾边效果
	RENDER_TEXT_ENGRAVE_BLOCK = 3,	// 雕刻效果
	RENDER_TEXT_COLORFUL = 4,		// 过渡颜色字体
}	RENDER_TEXT_STYLE;

// 光照类型
typedef enum 
{
	C3_RENDER_LIGHT_POINT          = 1,
	C3_RENDER_LIGHT_SPOT           = 2,
	C3_RENDER_LIGHT_DIRECTIONAL    = 3,
}	C3_RENDER_LIGHT_TYPE;

// 阴影模式
typedef enum
{
	C3_SHADOW_INVALID				= 0,
	C3_SHADOW_NONE					= 1,
	C3_SHADOW_25D_POINT_PLANAR		= 2,	// 2.5D兼容模式(假设Z轴向上),设置相对模型原点的点光源位置.
	C3_SHADOW_3D_POINT_PLANAR		= 3,	// 全3D,设置相对模型原点(即模型坐标系)的点光源位置
	C3_SHADOW_3D_DIRECTION_PLANAR	= 4,	// 设置平行光的方向
	C3_SHADOW_TYPE_MAX				= 5,
}	C3_SHADOW_TYPE;

// 画面质量
enum 
{
	C3_HINT_GRAPHIC_FLAGS			= 0xFFFFFFF0,
	C3_HINT_USE_STENCILBUFFER		= 0x80000000,
};

// 图像显示方式
enum
{
	_SHOWWAY_NORMAL	  = 0,
	_SHOWWAY_ADDITIVE = 1,
};

// 颜色值
enum 
{	
	_COLOR_WHITE	=	0x00FFFFFF,
	_COLOR_GRAY		=	0x007F7F7F,
	_COLOR_DARKGRAY	=	0x003F3F3F,
	_COLOR_BLACK	=	0x00000000,
	_COLOR_RED		=	0x00FF0000,
	_COLOR_GREEN	=	0x0000FF00,
	_COLOR_BLUE		=	0x000000FF,
	_COLOR_YELLOW	=	0x00FFFF00,
	_COLOR_TRANS	=	0x00FF00FF,
};

typedef unsigned int OBJID;
const OBJID	ID_NONE = 0;

typedef struct 
{
	int	x;
	int   y;
}	C3_POS;

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
}	C3_RECT;

typedef struct 
{
	float fHorizontal;
	float fVertical;
}	C3_DIR;

typedef struct 
{
	int  iWidth;
	int  iHeight;
}	C3_SIZE;

#endif//__C3_PRIMITIVE_H__