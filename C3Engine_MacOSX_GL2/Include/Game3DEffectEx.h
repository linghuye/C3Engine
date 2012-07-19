// Game3DEffectEx.h: interface for the CGame3DEffectEx class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _GAME_3DEFFECT_EX_H_
#define _GAME_3DEFFECT_EX_H_

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

// 索引返回-1意味着失败
// Note: 不得在I3DEffectEvent回调函数里删除光效!
class I3DEffectEvent
{
public:
	// 播放开始/延时完成
	virtual void OnPlay(int id) = 0;

	// 循环开始
	virtual void OnLoop(int id) = 0;

	// 进入循环间隔
	virtual void OnInterval(int id) = 0;

	// 完成
	virtual void OnOver(int id) = 0;

	// 将被自动删除
	virtual void OnDelete(int id) = 0;
};

class CGame3DEffectEx  
{
public:
	virtual unsigned long Release()	= 0;

	virtual	bool Process() = 0;
	virtual void Clear() = 0;
	virtual void Show(C3_POS posWorld, int id = -1) = 0;
	
	// 添加入特效集，成功返回特效id，错误返回 -1
	virtual int  Add(const char* pszIndex, bool bAutoDelete = true) = 0;

	// 删除指定的特效，如果有多个相同索引的特效，则删除第一个。
	virtual void Delete(const char* pszIndex) = 0;

	// 按照指定id从特效集合里面删除特效
	virtual void Delete(int id) = 0;

	// 按照指定id暂停/继续特效播放
	virtual void Pause(int id = -1) = 0;
	virtual void Continue(int id = -1) = 0;
	virtual void SetFrame(int nFrame, int id = -1) = 0;
	virtual void SetElapseTime(DWORD dwElapsedTime, int id = -1) = 0;

	// 空间位置变化，idx为特效的索引编号或者id，由bCtrlID决定, -1即所有特效
	// 这里的x,y,z是3D坐标系,注意到在2.5D视图下3D的Z轴向下,Y轴向里.
	virtual void Rotate(float xRadian, float yRadian, float zRadian, int idx = -1, bool bCtrlID = false) = 0;
	virtual void Scale(float x, float y, float z, int idx = -1, bool bCtrlID = false) = 0;
	virtual void Move(float x, float y, float z, int idx = -1, bool bCtrlID = false) = 0;
	
	// RotateZ用以兼容以前的代码,请不要再使用.
	// RotateZ相当相当于 Rotate(0.0f, 0.0f, D3DXToRadian(nAngleDegree), id, true);
	virtual void RotateZ(int id, int nAngleDegree) = 0;
	
	// 直接设置矩阵
	virtual void SetMatrix(const D3DXMATRIX& matrix, int id = -1) = 0;
	
	// 重置所有特效的运动
	virtual void ResetMovement() = 0;
	
	// 设置在Z轴上的偏移(即2.5D中的向下纵轴)
	virtual void SetOffset(int nOffset) = 0;
	virtual void SetHeight(float fHeight) = 0;
	
	// 设置是否作2.5D倾斜
	virtual void SetOblique(bool bOblique) = 0;

	// 设置颜色ARGB, id为特效的索引编号或者id，由bCtrlID决定, -1即所有特效.
	virtual void SetColor(DWORD dwColor, int idx = -1, bool bCtrlID = false) = 0;

	// 设置特效的混合方式, 其中id为特效id, num为特效的子件下标
	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, int id = -1, int num = -1) = 0;

	// 取特效帧数
	virtual int	 GetFrameAmount(int id = -1) = 0;

	// 检查名为pszIndex的特效是否存在
	virtual bool TestEffect(const char* pszIndex) = 0;

	// 查找名为pszIndex的特效,返回id
	virtual int	 FindEffect(const char* pszIndex) = 0;	

	// 使特效持续不断播放
	virtual void EverPlay(int id = -1) = 0;

	// 设置某个特效的偏移
	virtual void SetOffset(int nX, int nY, int nZ, int id = -1) = 0;

	// 重置控制回调接口
	virtual void SetEventInterface(I3DEffectEvent* pEvent) = 0;
	
	virtual void SetGravity(D3DXVECTOR3 vctGravity, int id = -1) = 0;
	virtual void SetWind(D3DXVECTOR3 vctWind, int id = -1) = 0;
		
	// id是Add返回的特效id, index是3DEffect.ini定义的多个特效的索引(EffectIdXX).
	// nTextureID是内部纹理ID,外部逻辑必须确保在CGame3DEffectEx生命期内nTextureID的有效性.
	// Note: 若nTextureID == -1会清除之前SetTexture的效果.
	// 光效只支持单层简单贴图
	virtual void SetTexture(int nTextureID, int nId = -1, int nIndex = -1) = 0;
	
	virtual bool Reload() = 0;
		
	// 设置特效帧间隔
	virtual void SetFrameInterval(DWORD dwFrameInterval, int id = -1) = 0;
	
	// 取特效延迟播放的帧数, = Delay / FrameInterval.
	virtual	int GetDelayFrameCount(int id) = 0;	
	
	// 设置特效当前即时帧
	virtual void SetImmediateFrame(int nFrame, int id = -1) = 0;
	
	// 允许绑定到人物的光效有自己的局部变换.
	// Note: 该特性历史上默认被关闭.
	virtual void AllowSelfTransformInRolePart(bool bEnable) = 0;
	
	// 添加光效,不同于Add方法,AddOnlyShell只加载特效定义,不尝试加载实际资源.
	virtual int AddOnlyShell(const char* pszIndex, bool bAutoDelete = true) = 0;
	
	virtual void SetLoopTime(DWORD dwLoopTime, int id) = 0;
	
	// 添加并即时加载,并且若光效的Delay为0,立即进入播放状态.
	virtual int AddImmediate(const char* pszIndex, const char* szBindPhyName = NULL, bool bAutoDelete = true) = 0;
	
	// 取Title为pszIndex的光效个数(pszIndex大小写相关)
	virtual DWORD GetEffectCountByName(const char* pszTitle) = 0;
	
	// 取Title为pszIndex的第nIndex个光效的控制ID(pszIndex大小写相关,nIndex以0为基)
	// 若没有第nIndex个,返回-1.
	virtual int FindEffectByName(const char* pszTitle, int nIndex) = 0;
	
	virtual bool GetEffectOffset(int id, int& nX, int& nY, int& nZ) = 0;
	
	// 直接使用矩阵显示
	// Note: Show3D方法不考虑Move/Rotate/Scale/SetMatrix/SetOblique/RotateZ/SetHeight/SetOffset的设置
	// Show3D仍然考虑3DEffect.ini里配置的Scale,Offset.
	virtual void Show3D(const D3DXMATRIX& matWorld, int id = -1) = 0;
	
	virtual void SetResourceFrame(DWORD dwFrame) = 0;
};

extern "C" ROLE3D_API CGame3DEffectEx* Game3DEffectCreate(I3DEffectEvent* pEvent = NULL);
extern "C" ROLE3D_API void LimitGame3DEffectLevel(int nGameEffectLevelLimit);
#endif//_GAME_3DEFFECT_EX_H_