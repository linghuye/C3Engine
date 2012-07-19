#ifndef I3DROLE_H
#define I3DROLE_H

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
#include "D3DXMathe.h"

class C3DMotion;
class C3DTexture;
class CGame3DEffectEx;
class I3DRole
{
public:
	virtual unsigned long Release() = 0;
 
	// 鼠标选择探测
	// Note: 此函数里的nOffsetX,nOffsetY,毫无作用,仅是历史接口兼容,一律传0.
	virtual bool HitTest(int nHitScreenX, int nHitScreenY, int nOffsetX, int nOffsetY, const char* pszPartName = NULL) = 0;

	// 判断资源是否就绪
	virtual bool IsPartResourceReady(const char* pszPartName) = 0;
	
	// 显示指定部件,部件模型的整体的世界位置是在此接口中传入的.
	// Note:部件的位置还受其局部矩阵的影响,局部矩阵在AddPart和SetPartSelfMatrix接口中传入.
	virtual void Draw(const char* pszPartName, 
				  const D3DXMATRIX& matrixCal, 
				  const D3DXMATRIX& matrixTrans, 
				  bool bClearMatrix) = 0;

	// 增加部件, 系统将在part_name.ini中搜寻idType的信息，名字none或者null都表示无。
	// 如果idType为0，则会产生一个空的part，用于支持直接在虚拟体上绑特效。
	virtual bool AddPart(const char* pszPartName, 
					  const char* pszFollowDumy, 
					  const char* pszDumyPartName,
					  OBJID idType,
					  const D3DXMATRIX* matPartSelf = NULL) = 0;

	// 合并部件，把source_part.ini中idSrcPartType的部件信息合并到目标部件target_part中去。
	virtual bool MergePart(const char* pszTargetPart, 
					  const char* pszSourcePart, 
					  OBJID idSrcPartType) = 0;

	// 拆除部件，把目标部件中包含的指定部件拆除出去。
	virtual bool DepartPart(const char* pszTargetPart, 
					  const char* pszSourcePart, 
					  OBJID idSrcPartType) = 0;

	// 设置Part自身矩阵
	virtual bool SetPartSelfMatrix(const char* pszPartName, D3DXMATRIX* matPartSelf) = 0;

	// 删除部件
	virtual void DelPart(const char* pszPartName) = 0;

	// 测试部件存在
	virtual bool TestPart(const char* pszPartName) = 0;
	
	// 设置,修改部件信息
	virtual void SetPartInfo(const char* pszPartName, const char* pszFollowDumy, const char* pszDumyPartName) = 0;

	// 设置当前动作
	virtual void SetPartMotion(const char* pszPartName, C3DMotion* pMotion) = 0;
	virtual void SetPartDefaultMotion(const char* pszPartName) = 0;
	virtual void SetPartBlendMotion(const char* pszPartName, C3DMotion* pMotion, float fWeight) = 0;

	// 清除指定部件的动作
	virtual void ClrPartMotion(const char* pszPartName) = 0;

	// 设置阴影属性
	// Note: sr,sg,sb没有作用,多余(历史遗留).
	virtual void SetShadowInfo(int nShadowType, float fLightX, float fLightY, float fLightZ, 
								  float sa = 0.35f, float sr = 0.0f, float sg = 0.0f, float sb = 0.0f) = 0;
	
	// 设置混合属性,混合属性的图形学含义请参考DirectX书籍和MSDN,或直接使用工具配置.
	virtual void SetBlendMode(int nAsb, int nAdb) = 0;
	
	// 设置贴图，如果部件名字为NULL则设置所有部件
	// 1.SetTexture,SetTexMixOpt修改的纹理对象和混合方式,仅对Set之后的1次IRole3D::Draw有效,再后的Draw仍然使用的ini配置的纹理和混合方式.
	// 2.若指针为空,将使用ini配置的纹理.
	virtual void SetTexture(C3DTexture* pTex, C3DTexture* pMixTex, const char* pszPartName = NULL) = 0;
	
	// 设置贴图混合方式,如果部件名字为NULL则设置所有部件
	// 混合方式的图形学含义,请参考工具组3D编辑器.
	virtual void SetTexMixOpt(int nOpt, const char* pszPartName = NULL) = 0;
	
	// 设置颜色
	// Note: ClrPartMotion会将设置的颜色清白.
	virtual void SetColor(float a, float r, float g, float b) = 0;

	// 设置当前帧
	virtual void SetFrame(DWORD dwFrame) = 0;
	virtual void SetBlendFrame(DWORD dwFrame) = 0;

	// 部件上添加特效, 名字为null时所有部件都起作用
	// bAllowRepeat: 是否允许重复添加相同光效.
	virtual void AddEffect(const char* pszEffect, const char* pszPartName = NULL, bool bAllowRepeat = false) = 0;

	// 部件上删除特效, 名字为null时所有部件都起作用
	virtual void ClrEffect(const char* pszEffect, const char* pszPartName = NULL) = 0;

	// 测试部件上特效
	virtual bool TestEffect(const char* pszEffect, const char* pszPartName = NULL) = 0;

	// 重新加载部件配置
	virtual bool Reload() = 0;
	
	// 添加并绑定特效pszEffect, 到部件pszPartName的pszBindPhyName虚拟体上,特效将跟随虚拟体运动.
	// Note: AddEffect添加的特效只是跟随部件运动.
	virtual void AddPartEffect(const char* pszEffect, const char* pszPartName = NULL, const char* pszBindPhyName = NULL, bool bAllowRepeat = false) = 0;
	
	// 屏幕窗口点击测试,精确到空间三角面,屏幕像素级.
	// 返回的t为射线公式: R = v0 + t * vDir, 其中v0为点中的近平面的投射点,vDir为镜头视线方向.
	// 为使骨骼运动下的点击检测正确,请确认调用该方法时已设置了正确的Motion和Frame.
	virtual bool ProbeIntersection(int nHitScreenX, int nHitScreenY, float& t, const char* pszPartName = NULL) = 0;
	
	// 左右颠倒地显示模型,即作镜像.
	virtual void SetMirror(bool bMirror) = 0;
	
	// 取出部件绑定的特效体集合接口
	virtual CGame3DEffectEx* GetGame3DEffect(const char* pszPartName) = 0; 
	
	// 计算部件pszPartName上的虚拟体pszDummyName的当前帧(SetFrame)的变换矩阵.
	// Note: 此变换矩阵表达了虚拟体对绑定到其上的部件/特效的空间作用.
	// 如果找不到部件和虚拟体,返回false.
	virtual bool CalculateVirtualMotion(const char* pszPartName, const char* pszDummyName, D3DXMATRIX& matResult) = 0;
	
	// 设置单个Part的颜色,若idMergedPartType不为0,仅设置Part中对应被merge的Obj
	// Note: ClrPartMotion会将设置的颜色清白.
	virtual bool SetColorEx(float alpha, float red, float green, float blue, const char* pszPartName, const char* pszMergedPartName, OBJID idMergedPartType = 0) = 0;
	
	// 修改某个Part的纹理配置
	virtual void ChangeTextureMap(OBJID idTex, OBJID idMixTex, int nMixOpt, const char* pszPartName = NULL) = 0;
	
	// 屏幕窗口点击测试,精确到空间三角面,屏幕像素级.
	// 返回的t为射线公式: R = v0 + t * vDir, 其中v0为点中的近平面的投射点,vDir为镜头视线方向.
	// 为使骨骼运动下的点击检测正确,请确认调用该方法时已设置了正确的Motion和Frame.
	// 返回的ucProperty为击中的三角面的应用层属性值. 该属性值定义于模型文件对应的属性文件中.
	// bNeedMotion: 精确点击是否需要考虑动作的影响.
	virtual bool ProbeObjInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty, const char* pszPartName = NULL) = 0;
	
	// 将某部件绑到骨架的任意一根骨骼上
	virtual void BindPartToBodyBone(const char* pszPartName, const char* pszFollowPartName, const char* pszFollowBoneName) = 0;
	
	// 直接设置空间矩阵位置
	virtual void SetTransform(const D3DXMATRIX& matInplace, const D3DXMATRIX& matTrans) = 0;
	
	// 添加并绑定部件到某根骨骼上
	virtual void AddEffectBindToBone(const char* pszEffect, const char* pszPartName = NULL, const char* pszBoneName = NULL, bool bAllowRepeat = false) = 0;
	
	virtual bool ProbeRoleInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty) = 0;
	
	// 设置双层贴图混合方式及参数
	// 1.SetTexture,SetTexMixOpt修改的纹理对象和混合方式,仅对Set之后的1次IRole3D::Draw有效,再后的Draw仍然使用的ini配置的纹理和混合方式.
	virtual void SetTextureShader(int nOperation, DWORD dwOperationData, const char* pszPartName = NULL) = 0;

	// 分部件设置当前帧
	virtual bool SetFrameEx(DWORD dwFrame, const char* pszPartName) = 0;
	virtual bool SetBlendFrameEx(DWORD dwFrame, const char* pszPartName) = 0;

	virtual bool SetMaterial(const D3DMATERIAL& material) = 0;
};

extern "C" ROLE3D_API I3DRole* I3DRoleCreate();

// 一般情况无需调用. Note:魔域项目使用了LoadAdjustConfig,调整了其地图中光效位置的错误,其地图数据不同于其他项目!!!
extern "C" ROLE3D_API void LoadAdjustConfig();

extern "C" ROLE3D_API void SyncObliqueAngle();

#endif//I3DROLE_H