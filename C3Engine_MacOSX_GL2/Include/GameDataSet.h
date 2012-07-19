#ifndef _GAMEDATASET_H
#define _GAMEDATASET_H

#include "C3Primitive.h"
#include "DXPrimitive.h"
#define GRAPHICDATA_API

class CAni;
class C3DObj;
class C3DMotion;
class C3DEffect;
class C3DTexture;
class C3DScene;

enum { _MAX_EFFECT_C3 = 16 };

// Note: 若直接使用CMy3DEffectInfo结构数据,请确认C3版本是否支持
struct CMy3DEffectInfo
{
	char szIndex[32];
	int nAmount;
	OBJID idEffect[_MAX_EFFECT_C3];
	OBJID idTexture[_MAX_EFFECT_C3];
	int	nAsb[_MAX_EFFECT_C3];
	int nAdb[_MAX_EFFECT_C3];
	int nScale[_MAX_EFFECT_C3];
	int nFrameOffset[_MAX_EFFECT_C3];
	int nC3FrameInterval[_MAX_EFFECT_C3];
	unsigned char nLoopOnce[_MAX_EFFECT_C3];
	bool bZBuffer[_MAX_EFFECT_C3];
	D3DXVECTOR3 vSelfOffset[_MAX_EFFECT_C3];
	unsigned char ucBillboard[_MAX_EFFECT_C3];
	int nDelay;
	int nLoopTime;
	int nFrameInterval;
	int nLoopInterval;
	int nOffsetX;
	int nOffsetY;
	int nOffsetZ;
	int nBillboard;
	int nRunLevel;				// 运行时重要等级,帧率低时底层低(数值高)的光效不播放.
	bool bColorEnable;			// 是否允许上色,默认下光效不受色彩设置影响.
};

enum { _MAXPART_SIMPLEOBJ = 4 };
struct C3DSimpleObjInfo
{
	OBJID idType;
	int	nParts;
	OBJID idMesh[_MAXPART_SIMPLEOBJ];
	OBJID idTexture[_MAXPART_SIMPLEOBJ];
};

struct C3DSimpleObjInfoEx : C3DSimpleObjInfo
{
	OBJID idMixTex[_MAXPART_SIMPLEOBJ];
	int	nMixOpt[_MAXPART_SIMPLEOBJ];
	const D3DMATERIAL* material[_MAXPART_SIMPLEOBJ];
	int	nAsb[_MAXPART_SIMPLEOBJ];
	int	nAdb[_MAXPART_SIMPLEOBJ];
};

enum { MAX_PART = 4 };
struct RolePartInfo
{
	int	nPart;
	OBJID idMesh[MAX_PART];
	OBJID idTexture[MAX_PART];
	OBJID idMixTex[MAX_PART];
	int	nMixOpt[MAX_PART];
	const D3DMATERIAL* material[MAX_PART];
	int	nAsb[MAX_PART];
	int	nAdb[MAX_PART];
};

enum 
{ 
	C3_EXIGENCE_NORMAL = 0,					  // 异步加载	
	C3_EXIGENCE_IMMEDIATE = 1,					  // 立即加载	
	C3_EXIGENCE_ABOVENORMAL = 2,				  // 无用(历史遗留)
	C3_EXIGENCE_FORCE_BACKGROUND = 3			  // 后台加载	
};
enum 
{ 
	INTERFACE_ICONANI = 0 
};
enum 
{ 
	DETAIL_HIGHT, 
	DETAIL_NORMAL, 
	DETAIL_LOW
};
enum 
{
	RELOAD_3DTEXTURE		= 0x1,				// 仅重载纹理资源
	RELOAD_3DOBJ			= 0x2,				// 仅重载3DObj资源
   	RELOAD_3DEFFECT			= 0x4,				// 仅重载3DEffect资源
   	RELOAD_3DMOTION			= 0x8,				// 仅重载3DMotion资源
  	RELOAD_DATA_ANI			= 0x10,				// 仅重载Data Ani资源
   	RELOAD_3DTEXTURE_INI	= 0x20,				// 仅重载3DTexture.ini配置
   	RELOAD_3DEFFECT_INI		= 0x40,				// 仅重载3DEffectObj.ini,3DEffect.ini配置
   	RELOAD_3DOBJ_INI		= 0x80,				// 仅重载3DObj.ini配置
   	RELOAD_ROLEINFO_INI		= 0x100,			// 重载RolePart.ini及RolePart.ini中引用到的armor.ini,3dmotion.ini等角色配置
   	RELOAD_3DSIMPLEOBJ_INI	= 0x200,			// 仅重载3DSimpleObj.ini配置
    RELOAD_DATA_ANI_INI		= 0x400,			// 仅重载DataAni的配置
	RELOAD_3DSCENE			= 0x800,			// 重载3DScene
	RELOAD_ALL				= 0xFFF,			// 重载全部
};

struct C3DObjInfo;
class CGameDataSet 
{
public:
	virtual bool Init(const char* pszAppFolder) = 0;
	virtual bool InitStrRes() = 0;
	virtual void Destroy() = 0;
	virtual void Process() = 0;
	
	virtual CAni* GetDataAni(const char* pszFileName, const char* pszTitle, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 30*1000) = 0;
	virtual C3DObj* Get3DObj(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	virtual C3DTexture* Get3DTexture(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	virtual C3DEffect* Get3DEffect(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;

	// pszPartName: 取哪个部件的动作
	virtual C3DMotion* Get3DMotion(const char* pszPartName, int64_t id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	
	virtual const C3DObjInfo* Get3DObjInfo(OBJID id) = 0;
	virtual OBJID Get3DTextureID(C3DTexture* pTexture) = 0;
	
	// 不要使用该接口!!
	virtual C3DEffect*  CreateNew3DEffect(OBJID id, bool bForceReload, bool bConfirmReload) = 0;
	
	virtual const char*	GetSound(OBJID idSound) = 0;
	virtual const char*	GetVersionInfo() = 0;
	virtual const char* Get3DMotionRes(const char* pszPartName, int64_t idMotion) const = 0;

	virtual const char* GetStr(OBJID idRes) const = 0;

	virtual const C3DSimpleObjInfo*	Get3DSimpleObjInfo(OBJID idType) const = 0;
	virtual const C3DSimpleObjInfoEx* Get3DSimpleObjInfoEx(OBJID idType) const = 0;

	virtual const CMy3DEffectInfo* GetMy3DEffectInfo(const char* pszIndex) const = 0;
	virtual CMy3DEffectInfo* GetMy3DEffectInfo(const char* pszIndex) = 0;
	virtual void AddMy3DEffectInfo(const CMy3DEffectInfo* info) = 0;

	virtual const RolePartInfo*	GetRoleRartInfo(const char* pszPartName, OBJID id) const = 0;
	
	// 通过id取得接口指针
	virtual void* QueryInterface(int iid) = 0;
	virtual int	GetEmotionIconInfoAmount() const = 0;
	virtual const char*	GetEmotionIconInfo(int nIndex) const = 0;
	virtual void BuildBin() = 0;
	virtual	DWORD GetActionTransitionTime(DWORD dwActionFrom, DWORD dwActionTo) const = 0;
	virtual	void ImmediateExigence(DWORD dwTime) = 0;

	// 清除当前加载的3D资源,并重新加载配置文件.
	virtual bool Reload(bool bBinary, DWORD dwReloadFlag = RELOAD_ALL) = 0;

	virtual C3DScene* Get3DScene(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	
	// 动态设置StrRes字符串项
	virtual void SetStr(OBJID idRes, const char* pszStr) = 0;
	
	// 取Material.ini里配置的命名材质数据.
	virtual const D3DMATERIAL*	GetMaterial(const char* pszMaterial) = 0;
	
	virtual void OnResetDevice() = 0;

	virtual bool InitDBStrRes() = 0;
	
	virtual bool IsInitSuccessfully() = 0;
	
	// 取所有部件名称,返回名字指针到arrRolePartNames数组
	// nMaxCount为传入的arrRolePartNames最大容纳的元素个数
	// 返回实际取得的个数.
	virtual int GetRolePartConfig(const char** ppRolePartNames, int nMaxCount) = 0;
	
	struct C3_ANI_RESOURCE_DESC
	{
		const char* pszAniFileName;
		const char* pszAniIndex;
	};
	
	struct C3_ROLEPART_RESOURCE_DESC
	{
		const char* pszRolePartName;
		OBJID idPart;
	};
	
	struct C3_ROLE_MOTION_RESOURCE_DESC
	{
		const char* pszRolePartName;
		int64_t idMotion;
	};
	
	// 上层对资源的需求接口
	struct IResourceDemand
	{	
		// 底层主动调用获取需求资源
		virtual int GetEffectTitleCount() = 0;
		virtual const char* GetEffectTitle(int nIndex) = 0;
		
		// Ani资源
		virtual int GetAniIndexCount() = 0;
		virtual C3_ANI_RESOURCE_DESC GetAniIndex(int nIndex) = 0;
		
		// 3DSimpleObj资源
		virtual int GetSimpleObjCount() = 0;
		virtual OBJID GetSimpleObjID(int nIndex) = 0;
		
		// 角色模型显示资源
		virtual int GetRolePartCount() = 0;
		virtual C3_ROLEPART_RESOURCE_DESC GetRolePartDesc(int nIndex) = 0;
		
		// 角色动作资源的RolePartName强制为RP_BODY
		virtual int GetRoleMotionCount() = 0;
		virtual C3_ROLE_MOTION_RESOURCE_DESC GetRoleMotionDesc(int nIndex) = 0;
		
		// 直接文件资源
		virtual int GetFileResourceCount() = 0;
		virtual const char* GetFileResourceName(int nIndex) = 0;
		
		// 在底层分析完资源后,总结汇报这些资源需要使用的文件资源.
		// Note: 这里的字符串指针数组是临时的,不能保存指针使用!
		virtual void ReportReferenceFileResource(int nFileResouceCount, const char** pArrFileNames, DWORD dwUserData) = 0;
		
		// 通知开始下载同步资源
		virtual void ResourceSynchronizeBegin(DWORD dwUserData) = 0;
		
		// 通知该资源需求的资源已经准备好
		// Note: bSuccess可能为false,比如P2P失败.
		virtual void ResourceSynchronizeFinish(bool bSuccess, DWORD dwUserData) = 0;
		
		// 通知该资源需求的资源已经准备好
		// Note: bSuccess可能为false,比如P2P失败.
		virtual void SingleFileSynchronizeFinish(const char* pszFileName, DWORD dwUserData) = 0;
	};
	
	// Note: 底层通过IResourceDemand接口获得需要的资源列表
	typedef enum 
	{	
		RESOURCE_NO_REFERENCE				= 0,
		RESOURCE_FILES_COMPLETE				= 1,
		RESOURCE_MISSING_TO_DOWNLOAD		= 2,
		RESOURCE_NOT_SUPPORT_MULTI_MISSION	= 3,
		RESOURCE_P2P_BROKEN					= 4,
	}	RESOURCE_SYN_RESULT;

	// Note: 再没有回调ResourceSynchronizeFinish前,调用者必须保证resourceDemand的有效性.
	virtual RESOURCE_SYN_RESULT SynchronizeResource(IResourceDemand& resourceDemand, DWORD dwUserData) = 0;

	virtual	void SetResourceLifeCheckPeriod(DWORD dwTime) = 0;
	
	virtual bool ReloadGameEffectResource(const char* pszEffectTitle) = 0;
	
	virtual bool ReloadAniResource(C3_ANI_RESOURCE_DESC& desc) = 0;
	
	virtual bool Reload3DSimpleObjResource(OBJID nSimpleObjID) = 0;
	
	virtual bool ReloadRolePartResource(C3_ROLEPART_RESOURCE_DESC& desc) = 0;
	
	virtual bool ReloadRoleMotionResource(C3_ROLE_MOTION_RESOURCE_DESC& desc) = 0;
	
	virtual bool Reload3DSceneResource(OBJID n3DSceneID) = 0;
};

//////////////////////////////////////////////////////////////////////
extern "C" GRAPHICDATA_API CGameDataSet& GameDataSetQuery();
extern "C" GRAPHICDATA_API void GameDataSetDestroy();
#define g_objGameDataSet GameDataSetQuery()

#endif//_GAMEDATASET_H
