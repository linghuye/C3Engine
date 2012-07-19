#ifndef _GRAPHIC_H
#define _GRAPHIC_H
 
#include "C3Primitive.h"
#include "DXPrimitive.h"

#ifndef GRAPHIC_API
#if defined(_WIN32)
#ifdef GRAPHIC_EXPORTS
#define GRAPHIC_API __declspec(dllexport)
#else
#define GRAPHIC_API __declspec(dllimport)
#endif
#else// 其他平台都是静态库
#define GRAPHIC_API
#endif
#endif

/*********************************************************************
**  Must Read
**  C3标准: 涉及返回指针的接口方法,凡是接口方法名是Create的,返回指针由
**  客户端负责其生命周期维护,即Release. 凡是接口方法名是Get的,返回指针
**  由底层维护其生命周期,上层不得Release,不得保留指针做长期使用.
*********************************************************************/

//////////////////////////////////////////////////////////////////////
struct	C3Motion;
struct	C3Texture;
typedef void* HC3MOTIONTRACKER;

enum 
{ 
	CAMERA_EYE	= 0x1, 
	CAMERA_TO	= 0x2, 
	CAMERA_BOTH = CAMERA_EYE | CAMERA_TO,
};

struct IDataIcon
{
	virtual class CAni* GetIconAni(int idxIcon) = 0;
};

/***********************************************************************
** Note: 必须在调用完CMyBitmap::Init3DEx后才能调用其他CMyBitmap::函数.
***********************************************************************/
class GRAPHIC_API CMyBitmap
{	
public:
	virtual unsigned long Release() = 0;
	
	// Note: 使用了非0的nAlpha后,下次再指定0,表示使用之前指定的非0的值.
	// 恢复原有Alpha需使用SetColor(255, 255, 255, 255)
	virtual void Show(int x, int y, int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;
	virtual void ShowEx(int x, int y, C3_RECT *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;
	
	virtual void ShowBlendEx(int x, int y, C3_RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, 
			int nAlpha = 0, int nRotateAngle = 0, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;

	virtual void GetSize(int& nWidth, int& nHeight) const = 0;
	virtual DWORD GetSize() const = 0;

	virtual int	GetWidth() const = 0;
	virtual int	GetHeight() const = 0;

	virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b) = 0;

	// 透明格式判断
	virtual int	IsTransparentFormat() const = 0;

	// 透明点判断
	// Note: 判断透明需要配置msk资源.
	// msk掩码资源由<msk生成工具>根据对应图片生成,并与对应图片同名,配置到图片所在的同级目录.
	// 在Ani里(或MyBitmapCreate传入的文件名)将图片文件名后缀改为.msk,通知底层在读取图片的同时读取关联的掩码数据.
	virtual bool IsTransparent(C3_POS posPoint) const = 0;
	
	virtual C3Texture* GetC3Texture() = 0;
	
	virtual void ShowLinear(int x, int y, DWORD dwWidth, DWORD dwHeight, C3_RECT* pTexSrc = NULL, 
		int nAlpha = 0, int nRotateAngle = 0, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;

	// 2D图浮点精度位置的渲染
	virtual void DrawEx(float x, float y, C3_RECT* pRcTex, float fWidth, float fHeight, int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;
	
	// 该接口将pPixelData指向的内存数据写入到底层纹理
	// rcTex: 表明将 一段像素数据 写到贴图的某个矩形范围内.
	// Note: 1.该接口直接操作底层纹理数据,开销大,不能频繁调用,一定要按规定格式传入,要特别小心.
	// 2.只能用在CMyBitmap 2D函数上,不能和其他3D的东西混用.
	// 3.数据内存必须以RGBA的内存字节顺序给出(注意DWORD序和字节序的差别!)
	// 4.数据内存长度必须是 nWidth * nHeight * 4!
	virtual bool SetImagePixelData2D(C3_RECT rcTex, const void* pPixelData) = 0;
	
public:	// font
	// 创建底层的默认字体(Note: 必须配置ini/font.ini)
	// 若pszDefaultFontFileName==NULL,取ini/font.ini配置的字体,反之则取传入的值.
	// 若dwFontSize==0,取ini/font.ini配置的字体大小,反之则取传入的值.
	static bool	GameFontCreate(const char* pszDefaultFontFileName = NULL, DWORD dwFontSize = 0);
	static void GameFontDestroy();

	// 取默认配置(ini/font.ini)的字体大小
	static DWORD GetFontSize(void);
	static void GetFontSize(C3_SIZE& infoSize);

	// 探测字符串中某个字符是否DBCS的第一个字符
	static bool IsDBCSLeadByte(const char* buf, int nOffset);

	// 计算字符串的显示长度(以字符串个数为单位)
	// 已废弃函数,请使用CalcuTextExtent.
	static int	GetStrShowLen(const char* pszStr, int nBegin, int nEnd);

	// 检查非法(无图形)字符
	static bool	CheckStringPixels(const char* pszString, DWORD uMinPixels, const char* pszFont, int nFontSize);
	
	// 显示字符串(支持回车)
	// pszFont:     字体名或TTF/TTC字体文件名
	// bAntialias:	使用抗锯齿字体
	// style:		字体风格
	// secondColor: 阴影/镶边/雕刻颜色
	// ptOffset:	阴影/雕刻偏移量
	static void	ShowString(int iPosX, int iPosY, DWORD color, const char *pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	// 显示表情字符串(支持表情图像)
	static void ShowEmotionString(int iPosX, int iPosY, DWORD color, const char* pszString, IDataIcon* pDataIcon,
									 const char* pszFont = NULL, int nFontSize = 14,
									 int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
									 RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
		
public: // camera
	static bool GameCameraCreate();
	
	// 切记,不要调用GameCameraDestroy!!!
	static void GameCameraDestroy();
	
	// 镜头尺寸应该符合当前的Orientation.
	static void GameCameraBuild(int nWidth, int nHeight);
	static void GameCameraBuild();
	static void GameCameraSet(float fX, float fZ, float fY = -1000.0f, DWORD dwOpt = CAMERA_BOTH);
	static void GameCameraGetPos(float& x, float& y, float& z, DWORD dwOpt = CAMERA_EYE);
	static void GameCameraMove(bool bForward = true);
	static void GameCameraRotate(int nAngle);
	static void GameCameraZoom(bool bZoomIn = true);
	static void GameCameraSetFov(float fFovDegree);
	static void GameCameraSetType(bool bProject);
	static bool GameCameraIsProject();
	static void GameCameraCircum(D3DXVECTOR3 vCenter, float fAngleDegree);
	static void GameCameraPitch(D3DXVECTOR3 vCenter, float fAngleDegree);
	
	// 自动根据当前OOrientation修改镜头的设置
	static bool GameCameraSetOrientation(int nOrientationMode);
	static void GameCameraLook(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vViewAt, const D3DXVECTOR3& vUp);
	
	// Note: fAspect为0,则使用画布设备的长宽比例,比例应该符合当前PortraitOrientation
	static void GameCameraPerspectiveLook(float fFovDegree, float fAspect, float zNear, float zFar);
	static void GameCameraOrthoLook(float fCameraWidth, float fCameraHeight, float zNear, float zFar);
	
	// 自行设置3D镜头的近平面和远平面,避免美术出的场景数值过大,超出镜头的远平面范围.
	static void GameCameraSetNearFarPlane(float fNear, float fFar);
	
public: // init and destory
	// 初始化,传入EAGLDrawable指针
	static bool Init3DEx(HWND hWnd, DWORD dwWidth, DWORD dwHeight, const char* pszResourcePath);

	// 中止3d
	static void Quit3D();
	
	// 探测3d设备
	static int	IfDeviceLost();
	
	// 重置3d设备
	static bool ResetDevice();
	
	// 设置画面细节质量
	// 1.若返回值为true,表示需要重启程序才能得到设置效果,上层应用在下次启动,创建3D设备前,调用该方法.
	// 2.不要在Begin3D和End3D之间调用该方法
	enum { GRAPHIC_DETAIL_FAST = 0, GRAPHIC_DETAIL_NORMAL = 1, GRAPHIC_DETAIL_GOOD = 2 }; 
	static bool HintGraphicDetail(DWORD nGraphicDetailLevel);
	
public:	// 3d relative
	// 开始3d调用
	static bool Begin3D();
	
	// 结束3d调用
	static bool End3D();
	
	// 清除zbuffer和屏幕
	static bool ClearBuffer(bool bZBuffer, bool bTarget, DWORD color);
	
	// 将Begin3D/End3D之间的Draw到后台画布的画面,正式刷到窗口屏幕上.
	// hDestWindowOverride: 刷到哪个窗口上
	// pDestRect: 刷到窗口的指定矩形范围,NULL表示填充整个窗口.
	// pSourceRect: 使用源画布的指定矩形范围,NULL表示整个画面.
	static bool Flip(const C3_RECT* pSourceRect = NULL, const C3_RECT* pDestRect = NULL, HWND hDestWindowOverride = NULL);

	// 准备绘制精灵
	static void PrepareSprite();
	
	// 准备绘制混合精灵
	static void PrepareMixSprite();
	
	// 准备绘制线
	static void	PrepareLine();
	
	// 准备绘制3D线
	static void	Prepare3DLine();
	
	// 准备绘制线
	static void	PrepareModel();
	
	static void PrepareLinearSprite();
	
	// 设置第1盏灯光,x,y,z为光的世界性方向
	static void	SetLight(bool bEnable,  float x, float y, float z, DWORD dwColorLight, DWORD dwColorAmbient, float fSpecularPower);
	
	// 取屏幕视口大小
	static void	GetScreenSize(int& nWidth, int& nHeight);
	static int	GetScreenWidth();
	static int	GetScreenHeight();
	
	// 画线函数
	struct LineVertex
	{
		float		x, y;		// 坐标
		DWORD	color;
	};
	
	static void ShowLine(LineVertex* line, DWORD num);
	static void ShowLine(int nPosX1, int nPosY1, int nPosX2, int nPosY2, DWORD dwColor);
	
	// 画矩形框
	static void	ShowRect(int x1, int y1, int x2, int y2, DWORD color);
	
	// 画实心矩形
	static void ShowBlock(int x1, int y1, int x2, int y2, DWORD color);
	static void ShowBlock(int x1, int y1, int x2, int y2, DWORD color1, DWORD color2, DWORD color3, DWORD color4);	
	static void ShowBlock(C3_POS posA, C3_POS posB, C3_POS posC, C3_POS posD, DWORD color);	

	// 画3维盒
	static void Show3DBox(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, DWORD color, bool bWireLine);

	// 截屏幕为JPEG到ScreenShot目录
	static void	ScreenShot(const char* pszFileName);

	// 取当前帧绘制的三角型数量
	static DWORD GetShowTriTotal();

	// 取角色倾斜角度(相对x轴旋转)
	static int GetObliqueAngle();

	// 打包文件
	static void	PackFileOpen(const char* pszPackFile);
	static void	PackFileClose();

	// 通用函数
	static void Common_Translate(D3DXMATRIX* pMatrix, float x, float y, float z);	
	static void Common_Rotate(D3DXMATRIX* pMatrix, float xRadian, float yRadian, float zRadian);
	static void Common_Scale(D3DXMATRIX* pMatrix, float x, float y, float z);
	
	static void Motion_GetMatrix(C3Motion* pMotion, DWORD dwBone, D3DXMATRIX* pMatrix);
	static void Motion_GetMatrix_Blend(HC3MOTIONTRACKER hMotionTracker, DWORD dwBone, D3DXMATRIX* pMatrix);
	
	static void ScreenToWorld(int nScrX, int nScrY, float& x, float& y, float& z);
	static void WorldToScreen(float x, float y, float z, int& nScrX, int& nScrY);
	static void	ScrRayBuild(int nScrX, int nScrY, D3DXVECTOR3& vFrom, D3DXVECTOR3& vDir);

	static void SetReflectFluidSpeed(float fUChangePerSecond, float fVChangePerSecond);
	
	// 获取指定大小字体的等宽用于排版,返回值为单字节字符宽,汉字需乘2.
	static DWORD GetFontWidth(const char* pszFont = NULL, int nFontSize = 12);

	static bool ChangeDisplayResulotion(HEAGLDRAWABLE hEaglDrawable, DWORD dwWidth, DWORD dwHeight);
	
	// 重置视口大小,即最后在窗口中的成像大小.
	// bDeviceOrLogic: 
	// 为true,设置视口的物理大小
	// 为false,设置视口的逻辑大小,即视口物理大小不变的情况下,改变其逻辑大小(所有的C3渲染函数基于逻辑单位).
	// Note: 不能在CTargetBmp渲染过程中使用
	static bool ChangeViewportResolution(DWORD dwWidth, DWORD dwHeight, bool bDeviceOrLogic = true);

	// 不等宽字体支持
	// 返回值为渲染的文字范围的矩形大小
	static C3_SIZE ShowStringEx(int iPosX, int iPosY, DWORD color, const char *pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, 
								RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE ShowEmotionStringEx(int iPosX, int iPosY, DWORD color, const char* pszString, IDataIcon* pDataIcon,
									 const char* pszFont = NULL, int nFontSize = 14, bool bAntialias = false, 
									 int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
									 RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	// 计算字体文字在屏幕上显示的范围的矩形大小
	// Note: pDataIcon == NULL时,#64这类表情符号将按字符的显示长度解析.
	static C3_SIZE CalcuTextExtent(const char* pszString, const char* pszFont = NULL, int nFontSize = 0, IDataIcon* pDataIcon = NULL, int nImageSize = 0);
	
	static void ProbeRayBuild(int nScrX, int nScrY, D3DXVECTOR3& vFrom, D3DXVECTOR3& vDir);
	
	#define LINE_3DVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
	struct Line3DVertex
	{
		float	x, y, z;		// 坐标
		DWORD	color;			// ABGR Format
	};
	static void Show3DLine(Line3DVertex* pLineVertex, DWORD nLineCount);
	
	// FontDrawBegin/FontDrawEnd配对使用.
	// 在FontDrawBegin和FontDrawEnd之间的ShowString,ShowTString,ShowEmotionString
	// 不会马上提交文字显示,而是将顶点累积下来,直到FontDrawEnd时才真正提交渲染.
	// Note: 大量文字显示时请使用该接口!
	static void FontDrawBegin();
	static void FontDrawEnd();

	// 设置倾斜角度（相对x轴旋转)
	static void SetObliqueAngle(int nAngle);
	
	// 取默认配置(ini/font.ini)的字体名称.
	static const char* GetDefaultFontName();
	
	// Unicode文字显示支持
	static C3_SIZE ShowStringW(int iPosX, int iPosY, DWORD color, const wchar_t* pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, 
								RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE ShowEmotionStringW(int iPosX, int iPosY, DWORD color, const wchar_t* pszString, IDataIcon* pDataIcon,
		const char* pszFont = NULL, int nFontSize = 14, bool bAntialias = false, 
		int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
		RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE CalcuTextExtentW(const wchar_t* pszString, const char* pszFont = NULL, int nFontSize = 0, IDataIcon* pDataIcon = NULL, int nImageSize = 0);
	
	// 计算在nMaxPixelWidth屏幕宽度下最多可以渲染几个字节字符 
	static int CalcuShowStringRangeEx(const char* pszString, int nBytesLen, int nMaxPixelWidth, const char* pszFont, int nFontSize, bool bAntialias, IDataIcon* pDataIcon = NULL, int nImageSize = 0);

	// 打开关闭/光照
	// Note: 最多支持3盏灯光
	static void EnableLight(int nLightIndex, bool bEnable);
	
	// 还原到graphic.ini配置的光照参数
	// Note: 历史遗留原因,GameCameraSetType也会自动调用RestoreConfigLight还原光照参数.
	static void RestoreConfigLight(bool bProject);
	
	// 手动设置光照方向(世界坐标)
	static void SetLightDirection(int nLightIndex, float x, float y, float z, bool bProject = true);
	
	// 设置光的颜色
	static void SetLightDiffuseColor(int nLightIndex, DWORD dwColorLight);
	
	// 设置光的环境分量色
	static void SetLightAmbientColor(int nLightIndex, DWORD dwColorAmbiemt);
	
	// 雾的控制
	// 打开关闭雾,默认雾关闭
	// Note: 打开雾会影响后续所有的渲染操作.
	static void EnableFog(bool bEnable);
	
	// 设置雾环境参数
	// fFogStart/fFogEnd是相对镜头的距离.
	static void SetFogEnv(DWORD dwFogColor, float fFogStart, float fFogEnd);
	
	// Point & Spot Light
	// Note: 最多支持3盏灯光,非方向性光源应该在方向性光源之前.
	// Note: 只能按顺序使用索引灯光,不得零散使用!!!
	static void SetLightType(int nLightIndex, C3_RENDER_LIGHT_TYPE type);

	static void SetLightColor(int nLightIndex, DWORD dwColorDiffuse, DWORD dwColorAmbient);
	
	static void SetLightPosition(int nLightIndex, float x, float y, float z);
	
	static void SetPointLightAttenuation(int nLightIndex, float a, float b, float c);
	
	static void SetSpotLightFalloff(int nLightIndex, float fInnerAngleRadian, float fOuterAngleRadian, float fFalloff = 1.0f);

	static D3DXMATRIX GetCurrentViewMatrix();

	static D3DXMATRIX GetCurrentViewMatrixInverse();

	static D3DXMATRIX GetCurrentProjectMatrix();

	static D3DXVECTOR2 GetCameraSize(bool bProject);

	static void ImageToWorld(int nImageX, int nImageY, bool bNearPlane, DWORD nImageWidth, DWORD nImageHeight, const D3DXMATRIX& matView, const D3DXMATRIX& matProject, float& x, float& y, float& z);
	static void WorldToImage(float x, float y, float z, DWORD nImageWidth, DWORD nImageHeight, const D3DXMATRIX& matView, const D3DXMATRIX& matProject, int& nImageX, int& nImageY);

	// 缩放ShowString系列函数的显示结果,所有ShowString系列函数受此影响.
	static void SetShowStringScale(float fScale);
	
	// 屏幕裁减
	static bool BeginClip(C3_RECT& rect);
	static void EndClip();
	
	// 显示C3当前资源数据
	static void DrawC3ResourceInfo(int x, int y);
	
	// 渲染一个自定义的三角模型.
	// pVertices,pTexCoords,pVertexColors,pVertNormals: 顶点数据,除pVertices外其他数据可以为NULL.
	// dwVertCount: 顶点个数
	// nStride: 顶点数据结构大小
	// pIndices: 顶点索引,为NULL,则按每3个顶点组成1个三角形渲染.
	// dwTriCount: 三角形个数
	// pTex: C3纹理,不能为NULL,即使不使用纹理也应该传入一个4X4的白色纹理.
	// dwColor: 整体颜色,仅当pTex != NULL时有意义.
	// dwAsb,dwAdb: D3DBLEND_XXX,DX混合模式
	// pMatWorld: 世界矩阵,为NULL,则在原点.
	static void DrawTrianglesUp(D3DXVECTOR3* pVertices, D3DXVECTOR2* pTexCoords, DWORD* pVertexColors, D3DXVECTOR3* pVertNormals, 
								DWORD dwVertCount, int nStride, WORD* pIndices, DWORD dwTriCount, 
								C3Texture* pTex, DWORD dwColor, DWORD dwAsb, DWORD dwAdb, D3DXMATRIX* pMatWorld, bool bTwoSided = true);
	
	// 使用CPuzzleBlock在屏幕上假想地画一个遮罩,不透明的地方为遮罩.
	// Note: BeginDrawClipMask会清除上次DrawClipMask的效果.
	static void BeginDrawClipMask();
	static void EndDrawClipMask();

	// 使用遮罩,在BeginUseClipMask/EndUseClipMask之间的所有渲染被限制在遮罩内.
	static void BeginUseClipMask();
	static void EndUseClipMask();

	// 必须在Init3DEx之前调用
	// 设置底层在解析ShowStringEx这类API传入的显示字符串时使用的代码页.
	// Note:其他字符串(如资源路径)一律以英文字符串为准.
	static void SetCodePageForShowString(DWORD dwCodePage);

	// 计算阿拉伯混合字符串中,某个字符的作图偏移位置.
	// bCalcuCharRight: false时取字符左边位置,true时取字符右边位置.
	static int CalcuCharPosInString(const char* pszString, int nCharIndex, bool bCalcuCharRight = false, const char* pszFont = NULL, int nFontSize = 0, IDataIcon* pDataIcon = NULL, int nImageSize = 0);
};

// Note: 注意iPad/iPhone下屏幕可以旋转,屏幕的Width/Height相对于当前旋转时的概念.
#define _SCR_WIDTH	CMyBitmap::GetScreenWidth()
#define _SCR_HEIGHT CMyBitmap::GetScreenHeight()

class CAni
{	
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(const char* pszDataFile, const char* pszIndex) = 0;
	virtual bool Show(DWORD uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) const = 0;
	virtual bool ShowEx(DWORD uFrame, int x, int y, C3_RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;
	
	virtual CMyBitmap* GetFrame(DWORD uFrameIndex) const = 0;
	virtual int	GetFrameAmount() const = 0;
	
	virtual DWORD GetSize() const = 0;
	virtual C3_SIZE GetSize(DWORD uFrame) const = 0;
	
	virtual bool ShowLinear(DWORD uFrame, int x, int y, DWORD dwWidth = 0, DWORD dwHeight = 0, C3_RECT* pSrcTex = NULL, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;

	// 锁定资源生命周期,避免被底层内存管理策略销毁.
	// Note: 内部使用 引用计数 管理.
	virtual void LockLife(bool bLock) = 0;
	
	// 是否被锁.
	virtual bool IsLifeLocked() = 0;
};

class CPuzzleCell
{
public:
	virtual unsigned long Release() = 0;
	virtual void SetVertexColor(int x, int y, DWORD dwARGB, int nLayer = 1) = 0;
	virtual void Show(int x, int y, int nWidth, int nHeight, CMyBitmap* pBmp1, CMyBitmap* pBmp2 = NULL, CMyBitmap* pBmp3 = NULL, CMyBitmap* pBmp4 = NULL) = 0;
};

class CPuzzleTriangle
{
public:
	virtual unsigned long Release() = 0;

	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, DWORD dwColorOp) = 0;
	virtual void SetVertexData(int idxVertex, int x, int y, DWORD color, float u, float v) = 0;
	virtual void Show(CMyBitmap* pBmp) = 0;
};

typedef enum 
{
	C3_MIX_MODULATE	  = 4,
	C3_MIX_MODULATE2X = 5,
	C3_MIX_MODULATE4X = 6,
	C3_MIX_ADD        = 7
}	TEXTURE_SHADER_OPERATION;

typedef enum 
{
	C3_BLEND_ZERO               = 1,
    C3_BLEND_ONE                = 2,
    C3_BLEND_SRCCOLOR           = 3,
    C3_BLEND_INVSRCCOLOR        = 4,
    C3_BLEND_SRCALPHA           = 5,
    C3_BLEND_INVSRCALPHA        = 6,
	C3_BLEND_DESTALPHA          = 7,
    C3_BLEND_INVDESTALPHA       = 8,
    C3_BLEND_DESTCOLOR          = 9,
    C3_BLEND_INVDESTCOLOR       = 10,
}	FRAME_BLEND_OPERATION;

/*********************************************************************
**  用于提交显示一张被分割成矩阵方格的图像.
**  可以指定方格每个的点的颜色,典型用来做战雾实现.
*********************************************************************/
class CPuzzleBlock
{	
public:
	virtual unsigned long Release() = 0;
	
	// 设置目标矩形的大小
	// Note: 格子的横纵个数由PuzzelBlockCreate的参数指定.
	virtual void SetSize(DWORD dwWidth, DWORD dwHeight) = 0;
	
	// 设置混合方式
	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, DWORD dwColorOp) = 0;
	
	// 设置顶点颜色 x <= w, y <= h
	// Note: 默认颜色是0xFFFFFFFF(白色)
	virtual void SetVertexColor(int x, int y, DWORD color) = 0;
	
	// 设置顶点的纹理坐标 x <= w, y <= h
	// Note: 默认纹理坐标按格子平均分割.
	virtual void SetVertexUv(int x, int y, float u, float v) = 0;
	
	// 显示
	virtual void Show(int nPosX, int nPosY, CMyBitmap* pBmp) = 0;
	
	// 设置Alpha混合方式
	virtual void SetAlphaOperation(DWORD dwAlphaOpt) = 0;
};	
	
/*********************************************************************
**  用于一次提交显示同一张图像中多个不同的区域,或者将同一张图像显示到
**  屏幕的多个不同区域.
**  可用于显示颠倒,反转,不规则的图像区域.
*********************************************************************/
class CMultiBmp
{	
public:
	virtual	unsigned long Release() = 0;
	
	// 预知分配内存,可以优化内存.
	virtual void PresetPartCount(DWORD dwPartCount) = 0;
	
	virtual void AddPartEx(int nShowX, int nShowY, int nWidth, int nHeight, DWORD dwColor = 0xFFFFFFFF, 
						   float fU0 = 0.0f, float fV0 = 0.0f, float fU1 = 1.0f, float fV1 = 1.0f) = 0;
	
	// nShowX, nShowY, nWidth, nHeight: 定义显示到屏幕窗口的矩形范围(不支持nWidth/nHeight默认取0的假设).
	// [fU0,fV0] - [fU3,fV3] 定义源贴图中的一个四边形范围,依Z字型顺序定义. UV坐标取值范围为[0 - 1].
	virtual	void AddPart(int nShowX, int nShowY, int nWidth, int nHeight,
						 float fU0 = 0.0f, float fV0 = 0.0f, float fU1 = 1.0f, float fV1 = 0.0f,
						 float fU2 = 0.0f, float fV2 = 1.0f, float fU3 = 1.0f, float fV3 = 1.0f) =	0;
	
	// 清空AddPart定义的所有矩形
	virtual	void ClearPart() = 0;
	
	// 使用pBmp显示AddPart定义的所有贴图矩形.
	virtual	void Show(CMyBitmap* pBmp, D3DBLEND srcBlend = D3DBLEND_SRCALPHA, D3DBLEND dstBlend = D3DBLEND_INVSRCALPHA) = 0;

	virtual	void ShowEx(int x, int y, CMyBitmap* pBmp, D3DBLEND srcBlend = D3DBLEND_SRCALPHA, D3DBLEND dstBlend = D3DBLEND_INVSRCALPHA) = 0;
};	

/*********************************************************************
**  BeginScene/EndScene必须独立于游戏主图像循环运行 
**  Show应该在游戏主图像循环中运行
**  设备丢失时必须删除所有CTargetBmp对象 否则设备无法恢复
**  内建了一个5X5的顶点集合 用于做aoxpuzzle的Alpha重构
*********************************************************************/
class CTargetBmp
{
public:
	virtual	DWORD Release()	= 0;

	// 开始渲染到贴图
	virtual	bool BeginScene() =	0;

	// 结束渲染到贴图
	virtual	void EndScene()	= 0;
		
	// 显示TargetBmp
	virtual	void Show(int x, int y, int nWidth, int nHeight) = 0;
	virtual	void SetAlpha(BYTE uIndex, BYTE uAlpha) = 0;
	virtual	void ShowBlend(int x, int y, int nWidth, int nHeight, DWORD dwColor) = 0;

	// Note: 传出的C3Texture指针为内部对象,外部不得删除.
	virtual	C3Texture* QueryC3Texture()	= 0;

	virtual int	QueryTextureID() = 0;
	 
	// 使用深度缓冲,默认时没有深度缓冲和操作.
	virtual bool UseDepthSurface() = 0;

	// 根据当前RenderTarget生成一个Managed纹理.
	virtual C3Texture* MakeManagedTexture() = 0;
	
	virtual C3Texture* QueryDepthTexture() = 0;
};

/*********************************************************************
**  3D 矩形集, 多次添加, 一次提交, 优化效率用.
**  可以用来构建3D中的面片/三角等简单形体.
*********************************************************************/
class CMultiRect
{
public:
	virtual	unsigned long Release()	= 0;
	
	// 添加需要渲染的空间平面矩形(Note: 以Z字序给出矩形区域的4个顶点)
	virtual	void AddRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4]) = 0;
	
	// 清除AddRect的矩形
	virtual	void ClearRect() = 0;

	virtual	void Show(CMyBitmap* pBmp, const D3DMATERIAL* pMaterial, bool bTwoSide = false, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA) = 0;

	virtual	void Show(CTargetBmp* pBmp, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA) = 0;
	
	// 添加需要渲染的空间平面矩形(Note: 以逆时针顺序给出矩形区域的4个顶点)
	virtual bool AddRenderRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4], DWORD dwColor = 0xFFFFFFFF, const D3DXVECTOR3& normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f)) = 0;
	
	// 渲染
	virtual	void Draw(C3Texture* pC3Tex, const D3DMATERIAL* pMaterial, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA, bool bTwoSide = true, bool bWriteZBuffer = true) = 0;

	// 取当前已经添加的矩形个数
	virtual DWORD GetRectCount() = 0;
};

class C3DGridPlane
{
public:
	virtual	unsigned long Release()	= 0;
	
	// 取得横纵格子数(Note: 横纵顶点数为格子数加1)
	virtual int GetRowCount() = 0;
	
	virtual int GetColCount() = 0;
	
	// 设置需要渲染的空间平面矩形(Note: 以逆时针顺序给出矩形区域的4个顶点)
	virtual void SetRenderRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4], const D3DXVECTOR3& normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f)) = 0;
		
	virtual void SetTransform(const D3DXMATRIX& matWorld) = 0;
	
	virtual void SetColor(DWORD dwColor) = 0;
	
	// 设置光照图映射平面矩形(Note: 以逆时针顺序给出矩形区域的4个顶点)
	virtual	void SetLightMapRect(const D3DXVECTOR3 verts[4], D3DXVECTOR3 normal) = 0;
	
	// 以光照图方式显示
	virtual	void ShowLightMap() = 0;
	
	virtual	void Draw(C3Texture* pC3Tex, const D3DMATERIAL* pMaterial, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA, bool bTwoSide = true, bool bWriteZBuffer = true) = 0;
	
	virtual bool Probe(const D3DXVECTOR3& vOrigin, const D3DXVECTOR3& vDir, float& t) = 0;
	
	virtual void* GetStridePositionArray(int& nStride, int& nCount) = 0;
	
	virtual void* GetStrideTexCoordArray(int& nStride, int& nCount) = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Texture;
class C3DTexture
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(const char* pszFileName) = 0;
	virtual void Destroy() = 0;
	
	virtual int	GetTexID() = 0;

	// Note: 传出的C3Texture指针为内部对象,外部不得删除.
	virtual C3Texture* GetTexture() = 0;
	
	virtual bool Reload() = 0;
	
	virtual int GetWidth() = 0;
	
	virtual int GetHeght() = 0;
	
	virtual void Show(int x, int y, int nDestWidth = 0, int nDestHeight = 0, C3_RECT* rcTex = NULL, DWORD dwColor = 0xFFFFFFFF, DWORD dwShowWay = _SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;
	
	virtual void ShowBlend(int x, int y, DWORD dwWidth = 0, DWORD dwHeight = 0, C3_RECT* pSrcTex = NULL, DWORD dwColor = 0xFFFFFFFF, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;
};

/**************************************************************
**  C3骨架资源接口
**  Note: 注意C3DSkeleton是资源接口,不是世界实例接口.
**  资源接口表示该对象将被多个使用者所引用,它的变化将引起所有
**  使用者的变化.一个资源接口可以创建出实例接口.
**  C3DSkeleton和C3DMotion是美术制作的固定的动作数据.
**  C3DDynamicSkeletonInstance是由程序控制的动作数据,将
**  C3DDynamicSkeletonInstance赋给C3DMotion产生最终的美术+程序
**  联合控制的动作.
**************************************************************/
class C3DDynamicSkeletonInstance;
class C3DSkeleton
{
public:
	virtual void Release() = 0;
	
	// 根据名称查找某根骨骼
	virtual int FindBoneByName(const char* pszBoneName) = 0;
	
	// 取子动作个数,包含骨架动作和虚拟体绑点动作
	virtual int GetSubMotionCount() = 0;

	// 取骨架动作的索引位置
	virtual int GetBodyMotionIndex() = 0;

	// 取骨骼绑定点
	virtual D3DXVECTOR3 GetPivotPoint(DWORD dwBoneId) = 0;

	virtual int GetParentBoneID(DWORD dwBoneId) = 0;
	
	virtual int GetVirtualPhyBoneId(DWORD dwVirtualPhyIndex) = 0;
	
	// 创建动态调整的骨架实例
	virtual C3DDynamicSkeletonInstance* CreateDynamicSkeletonInstance() = 0;

	virtual const char* GetBoneName(int nBoneID) = 0;
	
	virtual void GetBoneReferencePose(int nBoneID, D3DXVECTOR3& vPivot, D3DXQUATERNION& qRotation) = 0;

	virtual void GetBoneReferencePose(int nBoneID, D3DXMATRIX& matBone) = 0;
	
	virtual int GetBoneCount() = 0;

	virtual int GetVirtualPhyCount() = 0;
};

/**************************************************************
**  C3动态骨架实例接口
**************************************************************/
class C3DDynamicSkeletonInstance
{	
public:
	virtual void Release() = 0;
	
	// 取出引用的实际骨架
	virtual C3DSkeleton* GetReferenceSkeleton() = 0;
	
	// 根据名称查找某根骨骼
	virtual int FindBoneByName(const char* pszBoneName) = 0;
	
	// 调整某根骨骼的变换矩阵
	virtual bool TransformBoneDynamically(int nBoneID, const D3DXMATRIX& matBoneTransform) = 0;
	
	// 调整某根骨骼的变换运动,接受骨骼名.
	virtual bool TransformBoneDynamically(const char* pszBoneName, const D3DXMATRIX& matBoneTransform) = 0;
	
	// 重置还原某根骨骼的变换运动,接受骨骼名.
	virtual bool ResetBoneTransform(const char* pszBoneName) = 0;

	// 根据ID查找骨骼名称
	virtual const char* GetBoneName(int nBoneID) = 0;
	
	// 清除所有之前的TransformBoneDynamically的设置
	virtual void ResetAllBoneState() = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Motion;
class C3DMotion
{
public:
	virtual unsigned long Release() = 0;

	virtual bool Create(const char *filename, bool bBufferMotion) = 0;
	virtual void Destroy() = 0;

	virtual DWORD GetFrameAmount() = 0;
	virtual	DWORD GetMotionAmount() = 0;

	virtual C3Motion* GetMotion(int idx) = 0;
	
	virtual DWORD GetMotionKeyFrames(int idxMotion) = 0;
	virtual const D3DXMATRIX& GetKeyFrameMatrix(int idxMotion, int idxKeyFrame) = 0;
	
	virtual void Reload(const char* pszFileName) = 0;
	
	virtual const char* GetDataBaseName() = 0;		// 仅用于资源监控版本
	
	virtual void LockLife(bool bLock) = 0;
	virtual bool IsLifeLocked() = 0;
	
	// 设置该动作所属的骨架
	virtual bool SetSkeleton(C3DSkeleton* pSkeleton) = 0;
	
	// 获得该动作的配置的骨架
	virtual C3DSkeleton* GetSkeleton() = 0;
	
	// 计算当前动作相对于基准动作p3DBasedMotion的偏移矩阵.
	virtual bool CalcuSkeletonOffsetMatrix(C3DMotion* p3DBasedMotion, D3DXMATRIX& matOffset) = 0;
	
	// 赋予动作以动态调整的骨架,使动作发生程序性的变换.
	virtual bool ApplyDynamicSkeleton(C3DDynamicSkeletonInstance* pDynamicSkeletonInst) = 0;
	
	virtual void ResetTrackerState() = 0;
	
	// 取当前帧(由SetFrame设置)下骨骼的相对模型原点的矩阵.
	virtual bool GetCurrentSkeletonBoneMatrix(int nBoneID, D3DXMATRIX& matBone) = 0;
	
	// 设置当前帧(由SetFrame设置)下骨骼的相对模型原点的矩阵.
	virtual bool SetCurrentSkeletonBoneMatrix(int nBoneID, const D3DXMATRIX& matBone) = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Phy;
struct IPhyLoadConfig;
class C3DObj
{
public:
	virtual unsigned long Release() = 0;

	// 实例是否有效
	virtual bool IsValid() = 0;

	// 是否虚拟体
	virtual bool IsDummy(int idx) const = 0;

	// 创建一个模型 *.c3
	virtual bool Create(const char* filename, IPhyLoadConfig& phyLoadConfig) = 0;

	// 释放
	virtual void Destroy() = 0;

	// 这里的 x 对应屏幕坐标的 x, z 对应屏幕坐标的 y, 1 个单位就是 1 个象素
	// 移动
	virtual void Move(float x, float y, float z) = 0;
	
	// 旋转, 参数必须是弧度
	virtual void Rotate(float x, float y, float z) = 0;

	// 缩放， 1.0 就是原大小
	virtual void Scale(float x, float y, float z) = 0;

	// 加乘运动矩阵
	virtual void Multiply(D3DXMATRIX& matrix) = 0;

	// 颜色，范围从 0.0f - 1.0f
	virtual void SetARGB(float alpha, float red, float green, float blue) = 0;

	// 通过物体名字得到物体索引
	virtual int GetIndexByName(const char* pszName) = 0;

	// 通过索引取物体名字
	virtual const char* GetNameByIndex(int idx) = 0;

	// 取phy数量
	virtual DWORD GetPhyAmount() const = 0;

	// 通过索引获得bounding box
	virtual void GetBoundingBox(int idx, D3DXVECTOR3& vctMin, D3DXVECTOR3& vctMax) = 0;

	// 设置动作
	virtual void SetMotion(C3DMotion* pMotion) = 0;
	virtual void SetBlendMotion(C3DMotion* pMotion, float fWeight) = 0;

	// 通过索引取得动作
	virtual C3Motion* GetMotion(int idx) = 0;
	virtual C3Motion* GetBlendMotion(int idx) = 0;

	// 通过索引取得世界动作数据
	virtual D3DXMATRIX&	GetWorldMatrix(int idxPhy) = 0;

	// 下一帧 nStep 步幅 负直为倒播
	virtual void NextFrame(int nStep) = 0;

	// 设置当前帧数
	virtual void SetFrame(DWORD dwFrame) = 0;
	virtual void SetBlendFrame(DWORD dwFrame) = 0;

	// 绘制
	virtual void Draw(int typeShadow, const D3DXVECTOR3& vLight, float fShadowAlpha, 
					  DWORD asb = D3DBLEND_SRCALPHA, DWORD adb = D3DBLEND_INVSRCALPHA, 
					  bool bIgnoreStaticMotion = false, bool bMirror = false) = 0;
	
	// 换贴图
	virtual void ChangeTexture(C3DTexture* pTexture, const char* pszPhyName = NULL) = 0;
	virtual void ChangeMixTex(C3DTexture* pTexture, const char* pszPhyName = NULL) = 0;

	// 设置贴图混合方式和混合参数
	virtual void SetMixOpt(int nOpt, DWORD dwData) = 0;

	// 设置材质
	virtual void SetMaterial(const D3DMATERIAL& mat) = 0;

	// 清除上次渲染该模型使用的运动矩阵和颜色
	virtual void ClearMatrix() = 0;

	// 设置转换运动矩阵
	// matInPlace: 原地矩阵,用于正确计算原地阴影
	// matTrans: 偏移矩阵
	virtual void SetTransform(const D3DXMATRIX& matInPlace, const D3DXMATRIX& matTrans, const D3DXMATRIX& matWorld) = 0;

	// 取最大帧数
	virtual DWORD GetMaxFrameCount() = 0;

	virtual bool ProbeIntersection(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vDir, float& t, bool bNeedMotion, int* pPhyIndex = NULL, int* pTriFaceIndex = NULL) = 0;

	virtual C3Phy* GetPhy(int idx) = 0;

	virtual bool AddProcPhy(DWORD dwTriangleCount, D3DXVECTOR3* pVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pNormals = NULL) = 0;

	virtual bool AddProcPhyEx(DWORD dwVertexCount, DWORD dwTriangleCount, int nStride, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals, DWORD* pVertColors, unsigned short* pIndices) = 0;
	
	virtual void Reload(const char* pszFileName, IPhyLoadConfig& phyLoadConfig) = 0;
	
	virtual bool LoadPhyFaceProperty(const char* pszModelFileName, const char* pszPropertyFileName) = 0;
	
	virtual unsigned char GetFaceProperty(int nPhyIndex, int nFaceIndex) = 0;

	virtual C3Motion* GetBodyMotion() = 0;

	virtual C3Motion* GetBodyBlendMotion() = 0;

	virtual C3DMotion* GetCurrent3DMotion() = 0;

	virtual C3_PUBLIC_MODEL_VERTEX* GetModelVertexData(int& nStride, int& nCount) = 0;

	virtual WORD* GetTriangleFaceData(int& nTriangleCount) = 0;
};

//////////////////////////////////////////////////////////////////////
typedef void* HSHAPEANIMTRACKER;
typedef void* HPTCLMOTIONTRACKER;
typedef void* HPTCL3MOTIONTRACKER;

class C3DEffect  
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Load(const char* pszFilename) = 0;
	virtual void Unload() = 0;

	virtual void Bind(C3DObj* lpObj, const char* pszPhyName = NULL, bool bIsBindToBone = false, D3DXMATRIX* pMatSelfTransform = NULL) = 0;
	virtual void Draw(bool bNextFrame = true, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA, bool bZuffer = false, DWORD dwStepCount = 0, int billboardType = 0, D3DXVECTOR3* pCenter = NULL) = 0;
	virtual void SetTexture(C3DTexture* pTexture) = 0;
	virtual void ResetShape() = 0;
	
	virtual void Move(float x, float y, float z) = 0;
	virtual void Rotate(float xRadian, float yRadian, float zRadian) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void SetMatrix(const D3DXMATRIX& matrix) = 0;
	
	virtual void SetColor(DWORD dwColor) = 0;
	
	virtual DWORD GetPtclAmount() const = 0;
	virtual void SetPtclCurFrame(DWORD dwIndex, DWORD dwFrame) = 0;
	virtual int	GetPtclFrames(DWORD dwIndex) const = 0;
	
	virtual DWORD GetPhyAmount() const = 0;
	virtual void SetPhyCurFrame(DWORD dwIndex, DWORD dwFrame) = 0;
	virtual int	GetPhyFrames(DWORD dwIndex) const = 0; 
	
	virtual DWORD GetMaxFrames() const = 0;
	virtual void SetFrame(DWORD dwFrame) = 0;
	
	virtual DWORD GetShapeAmount() const = 0;
	
	virtual HSHAPEANIMTRACKER CreateShapeInfo() = 0;
	virtual void BackupShapeInfo(HSHAPEANIMTRACKER hShapeAnimTracker) = 0;
	virtual void RestoreShapeInfo(HSHAPEANIMTRACKER hShapeAnimTracker) = 0;
	
	virtual HPTCLMOTIONTRACKER CreatePtclInfo() = 0;
	virtual void SetPtclInfo(HPTCLMOTIONTRACKER info) = 0;

	virtual HPTCL3MOTIONTRACKER CreatePtcl3Info() = 0;
	virtual void SetPtcl3Info(HPTCL3MOTIONTRACKER info) = 0;
	virtual void SetGravity(D3DXVECTOR3 vGravity) = 0;
	virtual	void SetExternalSpeed(D3DXVECTOR3 vSpeed) = 0;
	virtual void SetTexture(int nTextureID) = 0;
	virtual bool GetAnimationCenter(D3DXVECTOR3& vCenter) = 0;
	virtual void Reload(const char* pszFileName) = 0;
	virtual const D3DXMATRIX& GetWorldMatrix() = 0;

	virtual bool Set3DMotion(C3DMotion* p3DMotion) = 0;

	virtual bool HitTest(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vDir) = 0;
};

//////////////////////////////////////////////////////////////////////
class C3DScene
{
public:
	virtual unsigned long Release() = 0;

	// 创建一个c3
	virtual bool Create(const char* filename) = 0;

	// 设置当前帧数
	virtual void SetFrame(DWORD dwFrame) = 0;

	// 绘制
	virtual void Draw(bool bStaticScene) = 0;

	// 换贴图
	virtual void ChangeTexture(C3DTexture* pTexture, const char* pszObjName = NULL) = 0;
	virtual void ChangeMixTex(C3DTexture* pTexture, const char* pszObjName = NULL) = 0;

	// 设置贴图混合方式
	virtual void SetMixOpt(int nOpt) = 0;

	// 设置混合模式
	virtual void SetBlendMode(int asb, int adb) = 0;

	// 设置材质颜色
	virtual void SetColor(DWORD dwARGB) = 0;

	// 设置材质
	virtual void SetMaterial(const D3DMATERIAL& mat) = 0;
	
	// 相交计算，true表示有相交, 如果有多个相交点，返回z值最小的点。
	virtual bool HitTest(int nScrX, int nScrY, float& x, float& y, float& z) = 0;
};

// Graphic函数族 ///////////////////////////////////////////////////////////////
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreate(const char* pszFile);
extern "C" GRAPHIC_API CMyBitmap* BmpCreate(void);
extern "C" GRAPHIC_API CPuzzleCell* PuzzelCellCreate();
extern "C" GRAPHIC_API CPuzzleTriangle* PuzzelTriangleCreate();
extern "C" GRAPHIC_API CPuzzleBlock* PuzzelBlockCreate(DWORD dwWidth, DWORD dwHeight, DWORD dwGridCountX, DWORD dwGridCountY);
extern "C" GRAPHIC_API CMultiBmp* MultiBmpCreate(void);
extern "C" GRAPHIC_API CMultiRect* MultiRectCreate(void);
extern "C" GRAPHIC_API CTargetBmp* TargetBmpCreate(DWORD uWidth, DWORD uHight);

// 从C3Texture创建C3DTexture或CMyBitmap对象.
// bAutoDeleteC3Texture:是否在C3DTexture/CMyBitmap被删除时,自动删除创建时传入的C3Texture.
extern "C" GRAPHIC_API C3DTexture* TextureCreateEx(C3Texture* pC3Tex, bool bAutoDeleteC3Texture);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateEx(C3Texture* pC3Tex, bool bAutoDeleteC3Texture);
extern "C" GRAPHIC_API CTargetBmp* TargetBmpCreateEx(DWORD uWidth, DWORD uHight);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateFrom3DTexture(C3DTexture* p3DTexture);
extern "C" GRAPHIC_API C3DTexture* TextureCreateFromTargetBmp(CTargetBmp* pTargetBmp);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateFromTargetBmp(CTargetBmp* pTargetBmp);
extern "C" GRAPHIC_API C3DGridPlane* GridPlaneCreate(DWORD dwGridRow, DWORD dwGridCol);

extern "C" GRAPHIC_API void SetCartoonSilhouetteWidth(float fWidth);
extern "C" GRAPHIC_API void SetCartoonSilhouetteColor(DWORD dwColor);
extern "C" GRAPHIC_API void SetDefaultC3FontFaceIndex(int nDefaultFontFaceIndex);

// 创建一个空纹理,用于写入数据
// Note: 为保证兼容所有低端机器,dwWidth,dwHeight必须相等,且为2的幂次方!
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateEmpty(DWORD dwWidth, DWORD dwHeight);

#endif//_GRAPHIC_H