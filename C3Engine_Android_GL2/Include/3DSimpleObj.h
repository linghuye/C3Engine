#ifndef _3DSIMPLEOBJ_H
#define _3DSIMPLEOBJ_H

#if defined(_WIN32)
#ifndef ROLE3D_API
#ifdef ROLE3D_EXPORTS
#define ROLE3D_API __declspec(dllexport)
#else
#define ROLE3D_API __declspec(dllimport)
#endif
#endif
#else
#define ROLE3D_API
#endif

#include "C3Primitive.h"

class C3DMotion;
class C3DTexture;
class C3DSimpleObj 
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(OBJID idType) = 0;
	
	// Note: 此函数里的nMapX,nMapY毫无作用,仅是历史接口兼容,一律传0.
	// 返回false表明需要的资源尚未加载完毕.
	virtual bool Draw2BG(int nMapX = 0, int nMapY = 0, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA) = 0;
	
	// Note: 此函数里的nMapX,nMapY毫无作用,仅是历史接口兼容,一律传0.
	virtual bool HitTest(int nHitScreenX, int nHitScreenY, int nMapX, int nMapY) = 0;
	
	// 设置位置和Yaw角度
	// Note: nY实际上是设置世界坐标的z值.
	// Note: nHeight的作用是将世界坐标的Z值加上nHeight * 2. (I dont know why? It must be a legacy param.)
	virtual void SetPos(int nX, int nY, int nHeight, int nRotateDegree, float fScale) = 0;
	
	// SetDeep设置的是3d下y值,仅有此方法设置了此y值,默认y值为0.0f.
	virtual void SetDeep(float fDeep) = 0;
	
	// 设置绕X轴的倾斜角度
	virtual void SetRotateX(int nRotateXDegree) = 0;
	
	// 设置物体的欧拉旋转角度,即依次按x,y,z轴旋转.
	// 这里的x,y,z是3D坐标系,注意到在2.5D视图下3D的Z轴向下,Y轴向里.
	virtual void SetRotate(float xRadian, float yRadian, float zRadian) = 0;
	
	// 设置阴影光照参数
	// 这里的x,y,z是3D坐标系,注意到在2.5D视图下3D的Z轴向下,Y轴向里.
	virtual void SetLightOffset(int type, float x, float y, float z, float a, float r = 0.0f, float g = 0.0f, float b = 0.0f) = 0;
	
	// 是否要绕X轴倾斜,为true.倾斜角度由SetRotateX指定,默认为-30度.
	virtual void SetOblique(bool bOblique) = 0;
	
	// 设置是否渲染阴影,nShadowType=2时渲染阴影.
	virtual void SetShadowType(int nShadowType) = 0;
	
	// 设置动作
	virtual void SetObjMotion(int64_t idMotion) = 0;

	// 设置颜色
	virtual void SetARGB(float a, float r, float g, float b) = 0;
	
	// 设置贴图,Note:
	// 1.SetTexture,SetARGB修改的纹理对象和混合方式,仅对Set之后的1次C3DSimpleObj::Draw2BG有效,再后的Draw2BG仍然使用的ini配置的纹理和混合方式.
	// 2.若指针为空,将使用ini配置的纹理.
	virtual void SetTexture(C3DTexture* pTex, C3DTexture* pMixTex, int nMixOpt = TEXMIX_MODULATE) = 0;
	
	// 设置动作帧
	virtual void SetFrame(DWORD dwFrame) = 0;
	
	// 设置动作
	virtual void SetMotion(C3DMotion* pMotion) = 0;
	
	// 为C3DSimpleObj绑定特效,特效初始绑定在模型的原点.
	virtual void AddEffect(const char* pszEffect) = 0;
	
	// 设置物件缩放比例,这里的x,y,z指3D世界的x,y,z.
	virtual void SetScale(float x, float y, float z) = 0;

	// 重载资源
	virtual bool Reload() = 0;

	// 取当前绑定的动作的最大帧数
	virtual DWORD GetMaxFrameCount() = 0;
	
	// 设置该3DSimpleObj为静止物体,以节省骨骼计算.
	virtual void SetStatic(bool bStatic) = 0;
	
	// 屏幕窗口点击测试,精确到空间三角面,屏幕像素级.
	// 返回的t为射线公式: R = v0 + t * vDir, 其中v0为点中的近平面的投射点,vDir为镜头视线方向.
	// 为使骨骼运动下的点击检测正确,请确认调用该方法时已设置了正确的Motion和Frame.
	virtual bool ProbeIntersection(int nHitScreenX, int nHitScreenY, float& t) = 0;

	// 左右颠倒地显示模型,即作镜像.
	virtual void SetMirror(bool bMirror) = 0;
	
	// 为过程式C3DSimpleObj添加顶点数据
	// 1.若当前对象不是使用Simple3DProcedureObjCreate创建的,返回false.
	// 2.受内部实现限制最多只能添加16个几何体,超出则返回false.
	// 3.dwTriangleCount为三角形个数, pTriVertices和pTexCoords分别指向顶点和纹理坐标数组,每3个顶点构成1个三角形.
	virtual bool AddProcPhy(DWORD dwTriangleCount, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals = NULL) = 0;
	
	virtual bool AddProcPhyEx(DWORD dwVertexCount, DWORD dwTriangleCount, int nStride, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals, DWORD* pColors, unsigned short* pIndices) = 0;
	
	virtual void ClearProcPhy() = 0;
	
	virtual void ChangeTextureMap(OBJID idTex, OBJID idMixTex, int nMixOpt) = 0;
	
	// 屏幕窗口点击测试,精确到空间三角面,屏幕像素级.
	// 返回的t为射线公式: R = v0 + t * vDir, 其中v0为点中的近平面的投射点,vDir为镜头视线方向.
	// 为使骨骼运动下的点击检测正确,请确认调用该方法时已设置了正确的Motion和Frame.
	// 返回的ucProperty为击中的三角面的应用层属性值. 该属性值定义于模型文件对应的属性文件中.
	// bNeedMotion: 精确点击是否需要考虑动作的影响.
	virtual bool ProbeObjInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty) = 0;
	
	// 设置双层纹理的混合方式
	virtual void SetMixOperation(int nMixOpt) = 0;
	
	// 绑定光效到某个虚拟体上
	virtual void BindEffect(const char* pszEffect, const char* pszBindPhyName) = 0;
		
	// 设置物体在全3D下的位置
	virtual void SetPosition(float x, float y, float z) = 0;
	
	// 设置双层贴图混合方式及参数
	virtual void SetTextureShader(int nOperation, DWORD dwOperationData) = 0;

	virtual void SetTexture2(C3DTexture* pTex, C3DTexture* pMixTex, int nOperation, DWORD dwOperationData) = 0;
	
	// 设置阴影属性
	virtual void SetShadowInfo(int type, float x, float y, float z, float fShadowAlpha) = 0;
	
	// 仅根据传入矩阵进行渲染,忽略所有SetXXX的位置设置.
	virtual bool Draw3D(const D3DXMATRIX& matInplace, const D3DXMATRIX& matTranslate, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA, D3DMATERIAL* pMaterial = NULL) = 0;
	
	virtual C3_PUBLIC_MODEL_VERTEX* GetModelVertexData(int& nStride, int& nVertexCount, int nPartIndex = 0) = 0;

	virtual WORD* GetTriangleData(int& nTriangleCount, int nPartIndex) = 0;
	
	// 考虑骨骼运动信息的HitTest,仅适用于只有一根骨骼的简单物体.
	virtual bool HitTest3D(int nHitScreenViewX, int nHitScreenViewY, const D3DXMATRIX& matInplace, const D3DXMATRIX& matTranslate) = 0;
};

// 根据3DSimpleObj.ini/3DSimpleObjEx.ini中配置的物件ID直接创建一个3DSimpleObj
extern "C" ROLE3D_API C3DSimpleObj* Simple3DObjCreate(OBJID idType = ID_NONE);

// 根据显式指定物件的资源ID创建一个3DSimpleObj
extern "C" ROLE3D_API C3DSimpleObj* Simple3DObjCreate2(OBJID idMesh, OBJID idTex, OBJID idMixTex, int nMixOpt, int nAsb, int nAdb);

// 创建一个过程式构造的3DSimpleObj,具体的顶点数据通过C3DSimpleObj::AddProcPhy传入.
// 这样创建出的C3DSimpleObj已被强制SetStatic(true).
extern "C" ROLE3D_API C3DSimpleObj* Simple3DProcedureObjCreate(OBJID idTex, OBJID idMixTex = 0, int nMixOpt = 1, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA);

#endif//_3DSIMPLEOBJ_H