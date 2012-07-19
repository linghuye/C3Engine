/********************************************************************
Created:		2006_4_25 9:18
Filename:		ITqSound.h
Author:		    lhq
Purpose:		
********************************************************************/
#ifndef _ITQSOUND_H_2006_4_25_9_18
#define _ITQSOUND_H_2006_4_25_9_18
//////////////////////////////////////////////////////////////////////
/*#ifdef TQSOUND_EXPORTS
#define TQSOUND_API __declspec(dllexport)
#else
#define TQSOUND_API __declspec(dllimport)
#endif*/
//#define TQSOUND_EXPORTS	//先编译通过测试

#define TQSOUND_EXPORTS

//////////////////////////////////////////////////////////////////////
enum TQSRESULT
{
	TQR_OK					=		0, // 正常
	TQR_PARAMETER_ERROR		=		1, // 参数检查错误
	TQR_FILENAME_ERROR		=		2, // 不识别的文件名
	TQR_AL_ERROR			=		3, // AL错误
	TQR_FILE_OPEN_ERROR		=		4, // 打开文件失败
	TQR_FILETYPE_ERROR		=		5, // 文件类型错误(不识别的文件类型) 
	TQR_NOINIT				=		6,	// 未初始化
};
//////////////////////////////////////////////////////////////////////
enum ENVIRONMENT
{
    ENVIRONMENT_GENERIC,			//普通		
	ENVIRONMENT_PADDEDCELL,			//软垫房
	ENVIRONMENT_ROOM,				//空房间
	ENVIRONMENT_BATHROOM,			//浴室
	ENVIRONMENT_LIVINGROOM,			//客厅
	ENVIRONMENT_STONEROOM,			//石质房间
	ENVIRONMENT_AUDITORIUM,			//礼堂
	ENVIRONMENT_CONCERTHALL,		//音乐厅
	ENVIRONMENT_CAVE,				//洞穴
	ENVIRONMENT_ARENA,				//圆形剧场中心舞台
	ENVIRONMENT_HANGAR,				//飞机库
	ENVIRONMENT_CARPETEDHALLWAY,	//带地毯大厅
	ENVIRONMENT_HALLWAY,			//走廊
	ENVIRONMENT_STONECORRIDOR,		//石质通道
	ENVIRONMENT_ALLEY,				//小巷
	ENVIRONMENT_FOREST,				//森林
	ENVIRONMENT_CITY,				//城市
	ENVIRONMENT_MOUNTAINS,			//群山
	ENVIRONMENT_QUARRY,				//采石场
	ENVIRONMENT_PLAIN,				//空旷地
	ENVIRONMENT_PARKINGLOT,			//停车场
	ENVIRONMENT_SEWERPIPE,			//下水管道
	ENVIRONMENT_UNDERWATER,			//水中
	ENVIRONMENT_DRUGGED,			//吸毒
	ENVIRONMENT_DIZZY,				//眩晕
	ENVIRONMENT_PSYCHOTIC,			//精神恍惚
	ENVIRONMENT_COUNT
};
enum PlayStatus
{
	StatusInit = 0,
	StatusPlaying,
	StatusStop,
	StatusPause,
};

//////////////////////////////////////////////////////////////////////

void setEnvPath(const char* lpszPath);

//////////////////////////////////////////////////////////////////////
class ISound
{
public:
	// 自释放接口
	virtual		TQSRESULT	Release(void)									=	0;

	//播放声音文件，不带3D效果
	virtual		TQSRESULT	PlaySound(const char* lpszFileName, long nLoop=0, long nVolume=100)	=	0;

	//播放声音文件，带3D效果
	virtual		TQSRESULT	Play3DSound( const char* lpszFileName, float fWorldX, float fWorldY, float fWorldZ,
										float fMaxDistance=50, long nLoop=0, long nVolume=100, bool bPlay=true ) =	0;

	//从内存中播放声音，lpszFileName用来做索引
	virtual		TQSRESULT PlaySoundByBuffer( const char* lpszFileName, const char* pBuffer, long lSize, long nLoop=0, long nVolume=100)	=	0;
	virtual		TQSRESULT Play3DSoundByBuffer( const char* lpszFileName, const char* pBuffer, long lSize, float fWorldX, float fWorldY, float fWorldZ,
										float fMaxDistance, long nLoop=0, long nVolume=100, bool bPlay=true )	=	0;

	//停止播放声音文件
	virtual		void	Stop(void)												=	0;

	//暂停播放声音文件
	virtual		void	Pause(void)												=	0;
	
	//是否在播放
	virtual		PlayStatus	GetState(void)										=	0;

	//重新播放某个声音文件
	virtual		bool	RePlay(void)											=	0;
	
	//继续播放声音文件
	virtual		bool	Continue(void)											=	0;

	//设置音量(0~100)
	virtual		bool	SetVolume( long iVolume )								=	0;
	//取音量(0~100)
	virtual		bool	GetVolume( long& iVolume )								=	0;
	
	//设置最小距离
	virtual		bool	SetMinDistance( float fValue )							=	0;
	//取最小距离
	virtual		bool	GetMinDistance( float& fValue )							=	0;
	
	//设置最大距离
	virtual		bool	SetMaxDistance( float fValue )							=	0;
	//取最大距离
	virtual		bool	GetMaxDistance( float& fValue )							=	0;
	
	//设置声源位置
	virtual		bool	SetSourcesPos( float fSourcesX, float fSourcesY, float fSourcesZ )	=	0;
	//取声源位置
	virtual		bool	GetSourcesPos( float& fSourcesX, float& fSourcesY, float& fSourcesZ )	=	0;
	
	//高级接口
	virtual		class	IAdvanceSound* QueryAdvance(void)						=	0;
};

//////////////////////////////////////////////////////////////////////
class ITqSoundManager
{
public:
	// 自释放接口
	virtual		TQSRESULT	Release(void)										=	0;

	//初始化
	virtual		bool	Init()													=	0;

	//播放声音文件，不带3D效果 // 播放完60s自释放
	virtual		TQSRESULT	PlaySound(const char* lpszFileName, long nLoop=0, long nVolume=100)	=	0;

	//播放声音文件，带3D效果 // 播放完60s自释放
	virtual		TQSRESULT	Play3DSound( const char* lpszFileName, float fWorldX, float fWorldY, float fWorldZ,
																	float fMaxDistance=50,
																	long nLoop=0,long nVolume=100, bool bPlay=true ) =	0;

	//处理声音，由外部程序循环调用
	virtual		void	ProcessSound(void)										=	0;
	
	//设置所有声音文件的音量百分比
	virtual		bool	SetVolume(long lPercent)								=	0;

	//停止所有声音文件
	virtual		bool	Stop(void)												=	0;

	//设置听者位置
	virtual		void	SetListenerPos(float fX, float fY, float fZ)			=	0;
	//取听者位置
	virtual		void	GetListenerPos(float& fX, float& fY, float& fZ)			=	0;
	
	//设置听者速度
	virtual		void	SetListenerVel(float fX, float fY, float fZ)			=	0;
	//取听者速度
	virtual		void	GetListenerVel(float& fX, float& fY, float& fZ)			=	0;
	
	//设置环境 dwValue 见 enum Enum_ENVIRONMENT
	virtual		bool	SetEnvironment(ENVIRONMENT eValue)						=	0;

	//取环境 dwValue 见 enum Enum_ENVIRONMENT
	virtual		bool	GetEnvironment(ENVIRONMENT& eValue)						=	0;

	//创建自管理对象 播放完不会自释放 需要调用 ISound 的 release 释放自管理对象接口
	virtual		ISound*	CreateSound(const char* pszSoundName = NULL, int nLoop = 0, int nVolume = 100)			=	0;

	//////////////////////////////高级调用///////////////////////////////////
	virtual		class	ITqAdvanceSoundManager*	QueryAdvanceSoundManager()		=	0;
};

/*TQSOUND_API*/ ITqSoundManager* TqSoundCreate();

//////////////////////////////////////////////////////////////////////
#endif  //#ifndef _ITQSOUND_H_2006_4_25_9_18



