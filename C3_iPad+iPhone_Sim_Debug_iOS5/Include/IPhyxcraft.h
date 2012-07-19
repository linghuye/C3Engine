#ifndef __PHYX_CRAFT_H__
#define __PHYX_CRAFT_H__

typedef void* HPHYXMESH;
typedef void* HPHYXACTOR;
typedef void* HPHYXCONTROLLER;
typedef void* HPHYTERRAIN;
typedef void* HSOFTBODYACTOR;
typedef void* HPHYXREGION;
typedef void* HPHYXCONSTRAINT;

/**********************************************************************************
** 物理引擎推算出的运动状态
**********************************************************************************/
enum PHYX_ROLE_MOVE_STATE
{	
	DM_ROLE_IDLE		=	0,  // 静止
	DM_ROLE_HORMOVE		=	1,  // 水平移动
	DM_ROLE_JUMPUP		=	2,  // 具有向上的速度
	DM_ROLE_SLIP_DOWN	=	3,  // 下落或者下滑
	DM_ROLE_REBOUND		=	4,  // 反弹过程
};

/**********************************************************************************
** 游戏物理物件分组类型,以下是被引擎内部使用的常规分组.
** 可以自定义扩展,扩展从6开始,最多31组!默认下:
** 1.ROLE_CONTROLER和DYNAMIC_OBJECTS和STATIC_TERRAIN&STATIC_OBJECT三者之间发生碰撞,
** 2.STATIC_TERRAIN/STATIC_OBJECT之间不发生碰撞.
** 3.其他PHYX_GROUP_CUSTOM_BEGIN以上组默认都不发生碰撞,组内的对象都是静态的.
** 4.关闭打开分组间的碰撞使用EnableGroupCollision.
** 5.设置组内对象是否动态使用EnableGroupDynamic
**********************************************************************************/
enum 
{ 
	PHYX_GROUP_NONE					= 0,	
	PHYX_GROUP_STATIC_TERRAIN		= 1,
	PHYX_GROUP_STATIC_OBJECT		= 2, 
	PHYX_GROUP_ROLE_CONTROLER		= 3,
	PHYX_GROUP_DYNAMIC_OBJECTS		= 4,
	PHYX_GROUP_CAMERA				= 5,
	PHYX_GROUP_CUSTOM_BEGIN			= 6,
	PHYX_GROUP_MAX					= 32
};

/***********************************************************************
**  物理接口描述了游戏引擎所需的物理概念和需求.任何引擎应该实现这个需求接口,
**  而不是让游戏去适应外部物理物理引擎的构建概念.
***********************************************************************/
struct PHYX_DEBUG_DATA
{
	bool bCollide;
	D3DXVECTOR3 vNormal;
	void* pCollideObjectUserData;
	D3DXVECTOR3 ptCollide1;
	D3DXVECTOR3 ptCollide2;
};

struct RAY_PICKUP_INFO
{	
	D3DXVECTOR3 vHitPos;
	D3DXVECTOR3 vHitNormal;
	float fRayHit;
	void* pClientObject;
	HPHYXACTOR  hHitActor;
};

struct PHYX_COLLIDE_INFO
{	
	HPHYXACTOR  hHitActor;
	void* pClientObject;
	D3DXVECTOR3 vHitPos;
	D3DXVECTOR3 vHitNormal;
};

struct PHYX_ROLE_INFO
{	
	PHYX_ROLE_MOVE_STATE state;
	D3DXVECTOR3 vVertSpeed;
};

struct PHYX_ENGINE_PARAM_DESC
{
	enum
	{
		SET_GRAVITY					= 0x1,
		SET_CONVEX_SHAPE_MARGIN		= 0x2,
		SET_CONCAVE_SHAPE_MARGIN	= 0x4,
	};
	int nFlag;
	float fConvexShapeMargin;		// 凸体的边缘大小
	float fConcaveShapeMargin;		// 凹体的边缘大小
	D3DXVECTOR3 vWorldGravity;		// 世界重力值
};

struct HAVOK_PRECOMPILE_DATA
{
	D3DXVECTOR4 info;
	DWORD size;
	BYTE* pData;
};

typedef enum
{
	PHYX_INTERNAL_ACOTR			=	0,
	PHYX_STATIC_OBJECT_ACTOR	=	1,
	PHYX_DYNAMIC_OBJECT_ACTOR	=	1,
	PHYX_KINEMATIC_OBJECT_ACTOR	=	3,
	PHYX_TERRAIN_ACTOR			=	4,
	PHYX_ROLE_ACTOR				=	5,
	PHYX_REGION_ACOTR			=	6,
}	PHYX_ACTOR_TYPE;

struct IDrawForDebug
{
	virtual void DrawLine(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo, const D3DXVECTOR3& color) = 0;
};

struct IPhyxWorld
{	
	/************************************************************
	**  销毁整个物理世界,先前所有创建的资源都将被销毁.
	************************************************************/
	virtual void Release() = 0;

	/************************************************************
	**  调整物理世界的内部参数
	************************************************************/
	virtual void AdjustPhysicsEngineParamters(const PHYX_ENGINE_PARAM_DESC& paramDesc) = 0;
	
	/*****************************************************************
	**  创建Mesh体的物理碰撞模型数据,注意HPHYXMESH是数据,不是世界实体.
	**  所传所有内存指针均为临时内存,不可直接保留使用.
	*****************************************************************/
	virtual HPHYXMESH BuildPhyxMesh(DWORD nPolygonSoupVertices, const D3DXVECTOR3* pVertices, DWORD nTrianglePrims, const WORD* pTrianglePrims, void* pPreCompileData = NULL) = 0;

	/*****************************************************************
	**  根据传入的顶点集合创建凸包碰撞模.
	**  所传所有内存指针均为临时内存,不可直接保留使用.
	*****************************************************************/
	virtual HPHYXMESH BuildConvexHullMesh(DWORD nPolygonSoupVertices, const D3DXVECTOR3* pVertices, void* pPreCompileData = NULL) = 0;

	/*****************************************************************
	**  创建Box Mesh体的物理碰撞模型数据.
	**  vExtent为Box的长宽高,Box中心默认建立在原点上
	*****************************************************************/
	virtual HPHYXMESH BuildBoxMesh(D3DXVECTOR3 vExtent) = 0;
	
	/*****************************************************************
	**  创建Sphere Mesh体的物理碰撞模型数据.
	**  fRadius为球体的半径,球体中心默认建立在原点上
	*****************************************************************/
	virtual HPHYXMESH BuildSphereMesh(float fRadius) = 0;
	
	/*****************************************************************
	**  创建Plane Mesh体的物理碰撞模型数据.
	**  vNormal是平面法线,fDist是平面到原点的距离.
	*****************************************************************/
	virtual HPHYXMESH BuildPlaneMesh(D3DXVECTOR3 vNormal, float fDist) = 0;
	
	/*****************************************************************
	**  创建Capsule Mesh体的物理碰撞模型数据.
	**  fHeight是整个胶囊的高,fRadius是胶囊圆柱体部分的横截半径.
	**  nUpAxis: 胶囊的高度轴在0=X,1=Y,2=Z
	*****************************************************************/
	virtual HPHYXMESH BuildCapsuleMesh(float fHeight, float fRadius, int nUpAxis) = 0;
	
	/*******************************************************************************
	**  销毁物理碰撞模型数据
	**  Note: 只能在所有使用这个HPHYXMESH的Actor都销毁后才能对其调用DestroyPhyxMesh.
	*******************************************************************************/
	virtual void DestroyPhyxMesh(HPHYXMESH hPhyxMesh) = 0;
	
	/*****************************************************************
	**  以HPHYXMESH为物理模型数据,创建一个物理实体,并添加到物理世界中去.
	**  vPos/qRot/fScale: 物理实体的世界位置
	**  nGameGroup: 添加到哪个分组
	**  userData: 客户端绑定数值.
	*****************************************************************/
	virtual HPHYXACTOR AddNewPhyxActor(HPHYXMESH hPhyxMesh, D3DXVECTOR3 vPos, D3DXQUATERNION qRot, float fScale, DWORD nGameGroup, void* userData) = 0;

	/************************************************************
	**  取得hPhyxActor的当前世界位置
	************************************************************/
	virtual bool GetPhyxActorPose(HPHYXACTOR hPhyxActor, D3DXMATRIX& matResult) = 0;
	
	/**********************************
	**  动态修改Actor的缩放比例
	**  Note: fScale是相对于当前的比例.
	**********************************/
	virtual bool ChangeActorScale(HPHYXACTOR hPhyxActor, float fScale) = 0;
	
	/**********************************
	**  强制移动Actor到某个位置
	**********************************/
	virtual bool MoveActorTo(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot) = 0;

	/**********************************
	**  获取Actor当前速度
	**********************************/
	virtual D3DXVECTOR3 GetActorCurrentSpeed(HPHYXACTOR hPhyxActor) = 0;

	/********************************************
	**  获取Actor的类型(Refer to PHYX_ACTOR_TYPE)
	********************************************/
	virtual PHYX_ACTOR_TYPE GetActorType(HPHYXACTOR hPhyxActor) = 0;
	
	/*****************************************************************************
	**  获取Actor身上绑定的客户端数据(即在AddNewPhyxActor时通过userData传入的数值.
	*****************************************************************************/
	virtual void* GetActorUserData(HPHYXACTOR hPhyxActor) = 0;
	
	/*****************************************************************
	**  删除世界物理实体
	*****************************************************************/
	virtual void RemovePhyxActor(HPHYXACTOR hPhyxActor) = 0;
	
	/*******************************************************************
	**  添加人物控制器,添加到ROLE_CONTROLLERN组.
	**  vAABBMin/vAABBMax: 人物模型的包围盒
	**  vInitPos/vOrientation: 初始位置
	**  userData: 客户端绑定数值.
	*******************************************************************/
	virtual HPHYXCONTROLLER AddRoleController(const D3DXVECTOR3& vAABBMin, const D3DXVECTOR3& vAABBMax, const D3DXVECTOR3& vInitPos, const D3DXVECTOR3& vOrientation, void* userData) = 0;

	/*******************************************************************
	**  删除人物控制器
	*******************************************************************/
	virtual void RemoveRoleController(HPHYXCONTROLLER hPhyxController) = 0;
	
	/*********************************************************************************
	** 设置地形标准,支持多个地形区域,每个区域可分成多个Block,每个Block可分成多个Grid.
	** nAreaSideBlockCount: 每个区域由nAreaSideBlockCount * nAreaSideBlockCount个Block组成.
	** nBlockSideGridCount: 每个Block由nBlockSideGridCount * nBlockSideGridCount个Grid组成
	** fGridSize: 最小单位块的边长(只支持正方格为最小单位块)
	** Note: 如果客户端只使用到一个层次,请设置nAreaSideBlockCount=1.
	**********************************************************************************/
	virtual void SetHeighFieldTerrainSpec(int nAreaSideBlockCount, int nBlockSideGridCount, float fGridSize) = 0;
	
	/*********************************************************************************
	**  添加一个地形Block,添加到STATIC_TERRAIN组.
	**  Note:一个Block被分为nBlockSideGridCount * nBlockSideGridCount个Grid.
	**  vBase: 该地形块的起始点的坐标.
	**  fHeights: 该地形块的浮点高度图数据,必须有(nBlockSideGridCount + 1)*((nBlockSideGridCount + 1)个浮点数.
	**  Note: 最终的地形以vBase为原点,在XY方向上按fGridSize每格递增,在Z高度上按: 浮点高度图数值 + vBase.z
	*********************************************************************************/
	virtual HPHYTERRAIN AddHeighFieldTerrainArea(D3DXVECTOR3 vBase, float* fHeights, void* pClientPointer) = 0;
	
	/************************************************************
	**  删除碰撞控制体
	************************************************************/
	virtual void RemoveHeighFieldTerrainArea(HPHYTERRAIN hPhyxTerrain) = 0;

	/***********************************************************************************************
	**  计算射线(vBegin -> vEnd)击中的第一个世界物理实体,
	**  返回值为被击中的HPHYXACTOR的AddNewPhyxActor传入的客户端指针参数.
	**  nGroupFlag为需要检测的组的位(等于1 << nGroupID),为0表示检查所有!
	************************************************************************************************/
	virtual bool RayPickup(D3DXVECTOR3& vBegin, D3DXVECTOR3& vEnd, DWORD nGroupFlags, RAY_PICKUP_INFO* pResultInfo) = 0;
	virtual bool SegmentPickup(D3DXVECTOR3& vBegin, D3DXVECTOR3& vEnd, DWORD nGroupFlags, RAY_PICKUP_INFO* pResultInfo) = 0;
	
	/***********************************************************************************************
	**  尝试将HPHYXACTOR放置到vDestPos处,求在物理碰撞下的实际可放置的位置.
	**  vOrientation: 角色当前朝向,是标准向量.
	************************************************************************************************/
	virtual D3DXVECTOR3 MoveRoleController(HPHYXCONTROLLER hPhyxController, const D3DXVECTOR3& vUserSpeed, float fMoveTime, PHYX_ROLE_INFO& resultInfo) = 0;

	/************************************************************
	**  动态改变人物物理模型的大小.
	************************************************************/
	virtual bool SetRoleControllerSize(HPHYXCONTROLLER hPhyxController, const D3DXVECTOR3& vAABBMin, const D3DXVECTOR3& vAABBMax) = 0;
	
	// 撤销人物的重力影响
	virtual bool EnableRoleControllerGravity(HPHYXCONTROLLER hPhyxController, bool bEnable) = 0;
	
	// 设置人物单步最高可爬上的台阶的高度.
	virtual bool SetRoleControllerMaxStepHeight(HPHYXCONTROLLER hPhyxController, float fMaxStepHeight) = 0;
	
	// 设置人物可以站立的最大斜角
	virtual bool SetRoleControllerMaxSlopeAngle(HPHYXCONTROLLER hPhyxController, float fSlopeDegree) = 0;
	
	/************************************************************
	**  模拟物理世界运行fTimeElippse秒.
	**  Note: 客户端必须每帧调用驱动该方法
	************************************************************/
	virtual void Simulate(float fTimeEllipse) = 0;

	/************************************************************
	**  设置该分组内的物体的行为特性
	************************************************************/
	struct PHYX_GROUP_BEHAVIOR_DESC
	{
		bool bDynamic;			// 动态物理
		bool bKinematic;		// 静态可逻辑移动
	};
	virtual void SetGroupBehavior(DWORD nFromCollisionGroupID, const PHYX_GROUP_BEHAVIOR_DESC& desc) = 0;

	/************************************************************
	**  设置组和组之间是否产生碰撞
	**  Note: 碰撞是双向的,A不和B发生碰撞,不等于B不和A发生碰撞.
	************************************************************/
	virtual void EnableGroupCollision(DWORD nFromCollisionGroupID, DWORD nToCollisionGroupID, bool bEnableCollision) = 0;
	
	/************************************************************
	**  取得调试信息,方便调试中间层.
	************************************************************/
	virtual PHYX_DEBUG_DATA& GetDebugData() = 0;

	/************************************************************
	**  设置游戏的镜头数据.
	************************************************************/
	virtual float SetupCollisionCamera(float fNearPlane, float fFovDegree, float fAspect) = 0;
	
	/************************************************************
	**  镜头场景碰撞,之前必须先调用CastCamera
	************************************************************/
	virtual bool CastCamera(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vFocus, D3DXVECTOR3& vCollidePosition) = 0;

	/********************************************************************
	**  给动态物件hPhyxActor设置物理属性
	********************************************************************/
	struct DYNAMIC_ACTOR_PHYSICS_DESC
	{
		enum
		{
			SET_MASS	= 0x1,
			SET_GRAVITY = 0x2,
			SET_MASS_CENTER = 0x4,
		};
		int nFlag;
		float fMass;
		float fGravityFactor;
		float vMassCenter[3];
	};
	virtual bool SetDynamicActorPhysics(HPHYXACTOR hPhyxActor, const DYNAMIC_ACTOR_PHYSICS_DESC& desc) = 0;

	/********************************************************************
	**  给动态物件hPhyxActor施加世界方向上的瞬间推力vImpulse的作用.
	********************************************************************/
	virtual bool ApplyImpulseToActor(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vImpulse) = 0;

	/*****************************************************************************
	**  给动态物件hPhyxActor施加世界方向上的力vForce的作用,持续施加fContinueTime.
	*****************************************************************************/
	virtual bool ApplyForceToActor(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vForce, float fContinueTime) = 0;
	
	/********************************************************************
	**  碰撞事件回调
	********************************************************************/
	struct IActorCollisionEvent
	{
		// 监测到有物体碰撞到hRegisteredActor
		// Note: 使用GetActorType获得infoCollide.hHitActor的具体类型
		virtual void ActorCollided(HPHYXACTOR hRegisteredActor, PHYX_COLLIDE_INFO infoCollide) = 0;
	};

	struct IRegionEvent
	{	
		// 监测到有物体进入区域hRegion
		// Note: 使用GetActorType获得Actor的具体类型
		virtual void ActorEnterRegion(HPHYXREGION hRegion, HPHYXACTOR hPhyxActor) = 0;

		// 监测到有物体离开区域hRegion
		virtual void ActorLeaveRegion(HPHYXREGION hRegion, HPHYXACTOR hPhyxActor) = 0;
	};

	/********************************************************************
	**  注册某个物件的碰撞事件回调
	********************************************************************/
	virtual bool RegisterActorCollisionEvent(HPHYXACTOR hPhyxActor, IActorCollisionEvent& event) = 0;
	virtual bool UnregisterActorCollisionEvent(HPHYXACTOR hPhyxActor) = 0;

	/********************************************************************
	**  注册某个区域的碰撞回调
	**  nGameGroup: 用于碰撞分组
	********************************************************************/
	virtual HPHYXREGION AddAABBGhostRegion(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, DWORD nGameGroup, IRegionEvent& callback) = 0;
	virtual HPHYXREGION AddShapeGhostRegion(HPHYXMESH hPhyxMesh, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, IRegionEvent& callback) = 0;
	virtual bool MoveGhostRegionTo(HPHYXREGION hGhostRegion, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot) = 0;
	virtual bool RemoveGhostRegion(HPHYXREGION hRegion) = 0;

	/************************************************************
	**  内部调试使用
	************************************************************/
	virtual void DrawDebugPhysicsWorld(IDrawForDebug* pDrawForDebug) = 0; 

	/********************************************************************
	**  铰链和约束
	********************************************************************
	**  自由旋转的绑定点约束
	********************************************************************/
	struct PHYX_SOCKETBALL_CONSTRAINT_DESC
	{	
		PHYX_SOCKETBALL_CONSTRAINT_DESC() : bDisableCollisionBetweenLinkedBodys(false), fSpringDistance(0.0f), bSetInWorldSpace(false) {}
		bool bDisableCollisionBetweenLinkedBodys;
		bool bSetInWorldSpace;
		float fSpringDistance;	// !=0表示带弹簧性质
	};
	virtual HPHYXCONSTRAINT AddSocketBallConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const PHYX_SOCKETBALL_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************
	**  门旋转的约束
	********************************************************************/
	struct PHYX_HINGE_CONSTRAINT_DESC
	{	
		PHYX_HINGE_CONSTRAINT_DESC() : fLowerAngleDegree(180.0f), fUpperAngleDegree(-180.0f), bDisableCollisionBetweenLinkedBodys(false),
			bMotorEnable(false), bSetInWorldSpace(false), fAutoRotateSpeedDegree(0.0f), fAutoRotateTargetDegree(0.0f) {}
		float fLowerAngleDegree;	// Note: 取值[-180,180],以物体初始位置为基准,顺时针方向为负,逆时针方向为正.
		float fUpperAngleDegree;	// [-180,180]
		float fAutoRotateTargetDegree;
		float fAutoRotateSpeedDegree;
		bool  bMotorEnable;
		bool  bSetInWorldSpace;		// 为true表示传入的坐标表达在世界坐标系下.
		bool  bDisableCollisionBetweenLinkedBodys;
	};
	virtual HPHYXCONSTRAINT AddHingeConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXVECTOR3& vLocalAxisInA, const D3DXVECTOR3& vLocalAxisInB, const PHYX_HINGE_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************************
	**  按骨骼的本地坐标系的X轴为骨骼轴
	********************************************************************************/
	struct PHYX_BONE_CONSTRAINT_DESC
	{	
		PHYX_BONE_CONSTRAINT_DESC() : fMaxConeDegree(0.0f), fMinTwistDegree(-90.0f), fMaxTwistDegree(90.0f), bSetInWorldSpace(false), bDisableCollisionBetweenLinkedBodys(false){}
		float fMinConeDegree;
		float fMaxConeDegree;		// [ 0,180]
		float fMinTwistDegree;		// [-90, 0]
		float fMaxTwistDegree;		// [ 90, 0]
		bool bSetInWorldSpace;
		bool bDisableCollisionBetweenLinkedBodys;
	};
	virtual HPHYXCONSTRAINT AddBoneConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXQUATERNION& qLocalRotateInA, const D3DXQUATERNION& qLocalRotateInB, const PHYX_BONE_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************
	**  平移约束
	********************************************************************/
	struct PHYX_TRANSLATE_CONSTRAINT_DESC
	{	
		PHYX_TRANSLATE_CONSTRAINT_DESC() : fMinTranslate(-FLT_MAX), fMaxTranslate(FLT_MAX), bDisableCollisionBetweenLinkedBodys(false), bSetInWorldSpace(false) {}
		float fMinTranslate;
		float fMaxTranslate;
		bool bDisableCollisionBetweenLinkedBodys;
		bool bSetInWorldSpace;
	};
	virtual HPHYXCONSTRAINT AddPrismaticConstraint(HPHYXACTOR hPhyxActorA, HPHYXACTOR hPhyxActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXVECTOR3& vLocalAxisInA, const D3DXVECTOR3& vLocalAxisInB, const PHYX_TRANSLATE_CONSTRAINT_DESC& desc) = 0;
	
	/********************************************************************
	**  约束链
	********************************************************************/
	struct PHYX_CHAIN_CONSTRAINT_DESC
	{	
		PHYX_CHAIN_CONSTRAINT_DESC() : nType(1), fSpringLength(0.0f) {}
		int nType;
		float fSpringLength;
	};
	virtual HPHYXCONSTRAINT AddConstraintChain(int nActorCount, HPHYXACTOR* arrActors, const D3DXVECTOR3& vFirstLocalPivotInA, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const PHYX_CHAIN_CONSTRAINT_DESC& desc) = 0;

	virtual void RemoveConstraint(HPHYXCONSTRAINT hConstraint) = 0;
	
	// 软体支持
	enum 
	{
		CLOTH_CORNER_LEFTTOP		= 0x1,
		CLOTH_CORNER_RIGHTTOP		= 0x2,
		CLOTH_CORNER_LEFTBOTTOM		= 0x4,
		CLOTH_CORNER_RIGHTBOTTOM	= 0x8,
	};

	enum 
	{
		SOFTBODY_COLLIDE_QUALITY_NONE	= 0,
		SOFTBODY_COLLIDE_QUALITY_NORMAL	= 1,
		SOFTBODY_COLLIDE_QUALITY_GOOD	= -1,
	};

	struct CLOTH_PARAM_DESC
	{
		CLOTH_PARAM_DESC() : nGridCX(8), nGridCY(8), nCollisionQuality(0),
			fClothGridLength(0.25f), nInitFixCornerFlag(CLOTH_CORNER_LEFTTOP | CLOTH_CORNER_RIGHTTOP),
			fTotalMass(1.0f), fLinearStiffness(0.5f), fAngularStiffness(0.5f), fMargin(0.25f),
			nBlendingConstraints(2)
		{
		}
			
		int nGridCX;				// 横向格子数
		int nGridCY;				// 纵向格子数
		int nCollisionQuality;		// 0:no collision	1: simple collision 2: accurate collision
		float fClothGridLength;		// 每格长度
		int nInitFixCornerFlag;		// 初始
		float fTotalMass;			// 质量
		float fLinearStiffness;		// 布料线性速度僵直度
		float fAngularStiffness;	// 布料角速度僵直度
		float fMargin;				// 布料和其他物件间碰撞要保持的最小距离.
		int   nBlendingConstraints;	// 弯曲自由程度[0-10]
	};

	struct SOFTBODY_PARAM_DESC
	{	
		SOFTBODY_PARAM_DESC() : nRes(32), nCollisionQuality(0), fTotalMass(1.0f), fLinearStiffness(0.5f), fAngularStiffness(0.5f), fMargin(0.2f)
		{
		}
		
		int nRes;
		int nCollisionQuality;// 0:no collision	1: simple collision 2: accurate collision
		float fTotalMass;
		float fLinearStiffness;
		float fAngularStiffness;
		float fMargin;
	};
	
	enum 
	{
		ROPE_CORNER_TOP		= 0x1,
		ROPE_CORNER_BOTTOM	= 0x2,
	};

	struct ROPE_PARAM_DESC
	{	
		ROPE_PARAM_DESC() : nRes(32), nCollisionQuality(1), fTotalMass(1.0f), fLinearStiffness(0.5f), nInitFixCornerFlag(1)
		{
		}
		
		int nRes;
		float fTotalMass;
		int nInitFixCornerFlag;
		int nCollisionQuality;// 0:no collision	1: simple collision 2: accurate collision
		float fLinearStiffness;
	};
	
	// 柔体支持
	// Note: Havok不支持柔体,这些函数在Havok实现下返回NULL或false.
	virtual HSOFTBODYACTOR AddCloth(const CLOTH_PARAM_DESC& clothDesc, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddEllipsoidSoftBody(const SOFTBODY_PARAM_DESC& sbDesc, const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vRadius, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddRope(const ROPE_PARAM_DESC& rpDesc, const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddMeshSoftBody(HPHYXMESH hPhyxMesh, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, void* userdata) = 0;
	virtual bool RemoveSoftBody(HSOFTBODYACTOR hSoftBodyActor) = 0;
	
	virtual int GetSoftBodyPointCount(HSOFTBODYACTOR hSoftBodyActor) = 0;
	virtual bool GetSoftBodyPoints(HSOFTBODYACTOR hSoftBodyActor, D3DXVECTOR3* pVertices, D3DXVECTOR3* pNormals, int nVertexCount, int nStride) = 0;
	virtual int GetSoftBodyTriangleCount(HSOFTBODYACTOR hSoftBodyActor) = 0;
	virtual bool GetSoftBodyTriangleIndices(HSOFTBODYACTOR hSoftBodyActor, WORD* pIndices, int nTriangleCount) = 0;
	virtual bool BindSoftBodyToActor(HSOFTBODYACTOR hSoftBodyActor, int nVertexIndex, HPHYXACTOR hPhyxActor, bool bDisableCollisionBetweenLinkedBodies) = 0;
	virtual bool AddForceToSoftBody(HSOFTBODYACTOR hSoftBodyActor, int nVertexIndex, const D3DXVECTOR3& vForce) = 0; 
	virtual bool SetFixedSoftBodyPoint(HSOFTBODYACTOR hSoftBodyActor, int nPointIndex, bool bFixed) = 0;
};

extern "C" IPhyxWorld* CreatePhyxWorld();

#endif//__PHYX_CRAFT_H__