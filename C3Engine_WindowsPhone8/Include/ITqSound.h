/********************************************************************
Created:		2006_4_25 9:18
Filename:		ITqSound.h
Author:		    lhq
Purpose:		
********************************************************************/
#ifndef _ITQSOUND_H_2006_4_25_9_18
#define _ITQSOUND_H_2006_4_25_9_18
//////////////////////////////////////////////////////////////////////
#ifdef TQSOUND_EXPORTS
#define TQSOUND_API __declspec(dllexport)
#else
#define TQSOUND_API __declspec(dllimport)
#endif

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
class ISound
{
public:
	// 自释放接口
	virtual		TQSRESULT	Release(void)									=	0;

	//播放声音文件，不带3D效果
	virtual		TQSRESULT	PlaySound(const char* lpszFileName, LONG nLoop=0, LONG nVolume=100)	=	0;

	//播放声音文件，带3D效果
	virtual		TQSRESULT	Play3DSound( const char* lpszFileName, FLOAT fWorldX, FLOAT fWorldY, FLOAT fWorldZ,
										FLOAT fMaxDistance=50, LONG nLoop=0, LONG nVolume=100, bool bPlay=true ) =	0;

	//从内存中播放声音，lpszFileName用来做索引
	virtual		TQSRESULT PlaySoundByBuffer( const char* lpszFileName, const char* pBuffer, LONG lSize, LONG nLoop=0, LONG nVolume=100)	=	0;
	virtual		TQSRESULT Play3DSoundByBuffer( const char* lpszFileName, const char* pBuffer, LONG lSize, FLOAT fWorldX, FLOAT fWorldY, FLOAT fWorldZ,
										FLOAT fMaxDistance, LONG nLoop=0, LONG nVolume=100, bool bPlay=true )	=	0;

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
	virtual		bool	SetVolume( LONG iVolume )								=	0;
	//取音量(0~100)
	virtual		bool	GetVolume( LONG& iVolume )								=	0;
	
	//设置最小距离
	virtual		bool	SetMinDistance( FLOAT fValue )							=	0;
	//取最小距离
	virtual		bool	GetMinDistance( FLOAT& fValue )							=	0;
	
	//设置最大距离
	virtual		bool	SetMaxDistance( FLOAT fValue )							=	0;
	//取最大距离
	virtual		bool	GetMaxDistance( FLOAT& fValue )							=	0;
	
	//设置声源位置
	virtual		bool	SetSourcesPos( FLOAT fSourcesX, FLOAT fSourcesY, FLOAT fSourcesZ )	=	0;
	//取声源位置
	virtual		bool	GetSourcesPos( FLOAT& fSourcesX, FLOAT& fSourcesY, FLOAT& fSourcesZ )	=	0;
	
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
	virtual		TQSRESULT	PlaySound(const char* lpszFileName, LONG nLoop=0, LONG nVolume=100)	=	0;

	//播放声音文件，带3D效果 // 播放完60s自释放
	virtual		TQSRESULT	Play3DSound( const char* lpszFileName, FLOAT fWorldX, FLOAT fWorldY, FLOAT fWorldZ,
																	FLOAT fMaxDistance=50,
																	LONG nLoop=0,LONG nVolume=100, bool bPlay=true ) =	0;

	//处理声音，由外部程序循环调用
	virtual		void	ProcessSound(void)										=	0;
	
	//设置所有声音文件的音量百分比
	virtual		bool	SetVolume(LONG lPercent)								=	0;

	//停止所有声音文件
	virtual		bool	Stop(void)												=	0;

	//设置听者位置
	virtual		void	SetListenerPos(FLOAT fX, FLOAT fY, FLOAT fZ)			=	0;
	//取听者位置
	virtual		void	GetListenerPos(FLOAT& fX, FLOAT& fY, FLOAT& fZ)			=	0;
	
	//设置听者速度
	virtual		void	SetListenerVel(FLOAT fX, FLOAT fY, FLOAT fZ)			=	0;
	//取听者速度
	virtual		void	GetListenerVel(FLOAT& fX, FLOAT& fY, FLOAT& fZ)			=	0;
	
	//设置环境 dwValue 见 enum Enum_ENVIRONMENT
	virtual		bool	SetEnvironment(ENVIRONMENT eValue)						=	0;

	//取环境 dwValue 见 enum Enum_ENVIRONMENT
	virtual		bool	GetEnvironment(ENVIRONMENT& eValue)						=	0;

	//创建自管理对象 播放完不会自释放 需要调用 ISound 的 release 释放自管理对象接口
	virtual		ISound*	CreateSound(const char* pszSoundName = NULL, int nLoop = 0, int nVolume = 100)			=	0;

	//////////////////////////////高级调用///////////////////////////////////
	virtual		class	ITqAdvanceSoundManager*	QueryAdvanceSoundManager()		=	0;
};

TQSOUND_API ITqSoundManager* TqSoundCreate();


//////////////////////////////////////////////////////////////////////
////////////////////////////高级调用//////////////////////////////////
#ifndef TQSOUND_EXPORTS

typedef struct _EAXBUFFERPROPERTIES
{
	LONG lDirect;					// direct path level
	LONG lDirectHF;					// direct path level at high frequencies
	LONG lRoom;						// room effect level
	LONG lRoomHF;					// room effect level at high frequencies
	FLOAT flRoomRolloffFactor;		// like DS3D flRolloffFactor but for room effect
	LONG lObstruction;				// main obstruction control (attenuation at high frequencies) 
	FLOAT flObstructionLFRatio;		// obstruction low-frequency level re. main control
	LONG lOcclusion;				// main occlusion control (attenuation at high frequencies)
	FLOAT flOcclusionLFRatio;		// occlusion low-frequency level re. main control
	FLOAT flOcclusionRoomRatio;		// occlusion room effect level re. main control
	LONG lOutsideVolumeHF;			// outside sound cone level at high frequencies
	FLOAT flAirAbsorptionFactor;	// multiplies DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF
	ULONG dwFlags;					// modifies the behavior of properties
} EAXBUFFERPROPERTIES, *LPEAXBUFFERPROPERTIES;

typedef struct _EAXLISTENERPROPERTIES
{
	LONG lRoom;						// room effect level at low frequencies
	LONG lRoomHF;					// room effect high-frequency level re. low frequency level
	FLOAT flRoomRolloffFactor;		// like DS3D flRolloffFactor but for room effect
	FLOAT flDecayTime;				// reverberation decay time at low frequencies
	FLOAT flDecayHFRatio;			// high-frequency to low-frequency decay time ratio
	LONG lReflections;				// early reflections level relative to room effect
	FLOAT flReflectionsDelay;		// initial reflection delay time
	LONG lReverb;					// late reverberation level relative to room effect
	FLOAT flReverbDelay;			// late reverberation delay time relative to initial reflection
	ULONG dwEnvironment;			// sets all listener properties
	FLOAT flEnvironmentSize;		// environment size in meters
	FLOAT flEnvironmentDiffusion;	// environment diffusion
	FLOAT flAirAbsorptionHF;		// change in level per meter at 5 kHz
	ULONG dwFlags;					// modifies the behavior of properties
} EAXLISTENERPROPERTIES, *LPEAXLISTENERPROPERTIES;

#endif//#ifndef TQSOUND_EXPORTS

class IAdvanceSound
{
public:
	//设置声源方向
	virtual		bool	SetSourceDir( FLOAT fX, FLOAT fY, FLOAT fZ )	=	0;
	//取声源方向
	virtual		bool	GetSourceDir( FLOAT& fX, FLOAT& fY, FLOAT& fZ )	=	0;
	
	//设置声源速度
	virtual		bool	SetSourcesVel( FLOAT fX, FLOAT fY, FLOAT fZ )	=	0;
	//取声源速度
	virtual		bool	GetSourcesVel( FLOAT& fX, FLOAT& fY, FLOAT& fZ )	=	0;
	
	//设置声源方向
	virtual		bool	SetConeOrientation( FLOAT fDirX, FLOAT fDirY, FLOAT fDirZ )	=	0;
	//取声源方向
	virtual		bool	GetConeOrientation( FLOAT& fDirX, FLOAT& fDirY, FLOAT& fDirZ )	=	0;

	virtual		bool	SetConeInsideAngle( DWORD dwValue )				=	0;
	virtual		bool	GetConeInsideAngle( DWORD& dwValue )			=	0;

	virtual		bool	SetConeOutsideAngle( DWORD dwValue )			=	0;
	virtual		bool	GetConeOutsideAngle( DWORD& dwValue )			=	0;

	virtual		bool	SetConeOutsideVolume( FLOAT fValue )			=	0;
	virtual		bool	GetConeOutsideVolume( FLOAT& fValue )			=	0;
	
	
	////////////////////////////环境音效相关//////////////////////////////////
	/*************************************************************************
	功能：一次搞定所有的参数 见 LPEAXBUFFERPROPERTIES 
	*************************************************************************/
	virtual		bool	SetSourceAll( LPEAXBUFFERPROPERTIES lpData )	=	0;
	virtual		bool	GetSourceAll( LPEAXBUFFERPROPERTIES lpData )	=	0;
	
	virtual		bool	SetSourceDirect( LONG lValue )					=	0;
	virtual		bool	GetSourceDirect( LONG& lValue )					=	0;

	virtual		bool	SetSourceDirectHF( LONG lValue )				=	0;
	virtual		bool	GetSourceDirectHF( LONG& lValue )				=	0;

	virtual		bool	SetSourceRoom( LONG lValue )					=	0;
	virtual		bool	GetSourceRoom( LONG& lValue )					=	0;

	virtual		bool	SetSourceRoomHF( LONG lValue )					=	0;
	virtual		bool	GetSourceRoomHF( LONG& lValue )					=	0;

	virtual		bool	SetSourceRolloff( FLOAT fValue )				=	0;
	virtual		bool	GetSourceRolloff( FLOAT& fValue )				=	0;

	virtual		bool	SetSourceOutside( LONG lValue )					=	0;
	virtual		bool	GetSourceOutside( LONG& lValue )				=	0;
	
	virtual		bool	SetSourceAbsorption( FLOAT fValue )				=	0;
	virtual		bool	GetSourceAbsorption( FLOAT& fValue )			=	0;

	virtual		bool	SetSourceFlags( DWORD dwValue )					=	0;
	virtual		bool	GetSourceFlags( DWORD& dwValue )				=	0;

	virtual		bool	SetSourceObstruction( LONG lValue )				=	0;
	virtual		bool	GetSourceObstruction( LONG& lValue )			=	0;

	virtual		bool	SetSourceObstructionLF( FLOAT fValue )			=	0;
	virtual		bool	GetSourceObstructionLF( FLOAT& fValue )			=	0;

	virtual		bool	SetSourceOcclusion( LONG lValue )				=	0;
	virtual		bool	GetSourceOcclusion( LONG& lValue )				=	0;

	virtual		bool	SetSourceOcclusionLF( FLOAT fValue )			=	0;
	virtual		bool	GetSourceOcclusionLF( FLOAT& fValue )			=	0;

	virtual		bool	SetSourceOcclusionRoom( FLOAT fValue )			=	0;
	virtual		bool	GetSourceOcclusionRoom( FLOAT& fValue )			=	0;
	//////////////////////////////////////////////////////////////////////
};

	//////////////////////////////高级调用////////////////////////////////////
class ITqAdvanceSoundManager
{
public:
	/*************************************************************************
	功能：一次搞定所有的参数 见 LPEAXLISTENERPROPERTIES
	*************************************************************************/
	virtual		bool	SetAll( LPEAXLISTENERPROPERTIES lpData )		=	0;
	virtual		bool	GetAll( LPEAXLISTENERPROPERTIES lpData )		=	0;
	
	/*************************************************************************
	功能：设置整个效果空间的大小
	*************************************************************************/
	virtual		bool	SetRoom( LONG lValue )							=	0;
	virtual		bool	GetRoom( LONG& lValue )							=	0;
	
	/*************************************************************************
	功能：设置房间高频增益控制
	说明：环境使反射、散射获得高频增益，我们通常说的声音干湿比例，干声指直达声，
			湿声指环境因素带来的高低频增益，而这里只是提供了高频增益的控制。
	*************************************************************************/
	virtual		bool	SetRoomHF( LONG lValue )						=	0;
	virtual		bool	GetRoomHF( LONG& lValue )						=	0;
	
	/*************************************************************************
	功能：房间高频滚降系数调节
	说明：用于模拟墙面对声音的吸收。实际中，高频滚降有两个维度可以调整，
		一是滤波器衰减深度，决定高频滚降速率；一个是滤波器的截止频率，
		决定混响声的频响特性。而我们一般调整的是这两个维度的联动，转折频率高时，
		滚降速度定得大一些，转折频率低时，滚降速度就可以小一些，
		这样，滚降的效果才会自然。
	*************************************************************************/
	virtual		bool	SetRoomRolloff( FLOAT fValue )					=	0;
	virtual		bool	GetRoomRolloff( FLOAT& fValue )					=	0;
	
	/*************************************************************************
	功能：空气高频吸收
	说明：用于模拟空气对高频声波的吸收特性。空气湿度越大，对声波的高频吸收率越低。
	*************************************************************************/
	virtual		bool	SetAirAbsorption( FLOAT fValue )				=	0;
	virtual		bool	GetAirAbsorption( FLOAT& fValue )				=	0;
	
	/*************************************************************************
	功能：听者高频滚降系数调整
	说明：用于模拟听者对声音产生的不同影响，例如声音前后，
		由于人耳的构造我们听到后面的声音总是相对压抑的。同房间高频滚降系数一样，
		这里调整两个纬度的联动。支持EAX的3D游戏中，
		音效的3D定位实际上是由Direct Sound 3D来完成的。
	*************************************************************************/
	virtual		bool	SetListenerRolloff( FLOAT fValue )				=	0;
	virtual		bool	GetListenerRolloff( FLOAT& fValue )				=	0;
	
	/*************************************************************************
	功能：衰减时间设置
	说明：针对混响效果渐渐消失而言，以秒为单位，小数点后面为毫秒
	*************************************************************************/
	virtual		bool	SetDecayTime( FLOAT fValue )					=	0;
	virtual		bool	GetDecayTime( FLOAT& fValue )					=	0;
	
	/*************************************************************************
	功能：衰减高频比率设置
	说明：这里指直达声与混响信号的比率，用于增强环境声音的自然特性。
	*************************************************************************/
	virtual		bool	SetDecayHFRatio( FLOAT fValue )					=	0;
	virtual		bool	GetDecayHFRatio( FLOAT& fValue )				=	0;
	
	/*************************************************************************
	功能：环境反射设置
	说明：提供早期反射和回声的定位，系数越大，反射越大。
	*************************************************************************/
	virtual		bool	SetReflections( LONG lValue )					=	0;
	virtual		bool	GetReflections( LONG& lValue )					=	0;
	
	/*************************************************************************
	功能：反射延时设置
	说明：反射延迟时间越大，反射回声效果越明显。小数点后面以毫秒为单位。
	*************************************************************************/
	virtual		bool	SetReflectionsDelay( FLOAT fValue )				=	0;
	virtual		bool	GetReflectionsDelay( FLOAT& fValue )			=	0;
	
	/*************************************************************************
	功能：环境混响大小设置
	说明：当声波停止发声后，由于房间内存在声反射面，
		使声波在其间多次反射或散射而形成的声音延续现象称为混响。
	*************************************************************************/
	virtual		bool	SetReverb( LONG lValue )						=	0;
	virtual		bool	GetReverb( LONG& lValue )						=	0;
	
	/*************************************************************************
	功能：混响主延迟时间设置
	说明：用于模拟混响声阵的延迟效应，拖动滑块调节主延迟时间。
		有些混响器将次对应调整称为混响空间尺寸调整（SIZE），
		实际上用于调整混响声阵密度，主延迟时间越长，混响声阵的密度就越稀疏，
		模拟的混响空间尺寸越大。
	*************************************************************************/
	virtual		bool	SetReverbDelay( FLOAT fValue )					=	0;
	virtual		bool	GetReverbDelay( FLOAT& fValue )					=	0;
	
	/*************************************************************************
	功能：设置环境(空间)大小、尺寸。
	说明：录音工程中为模拟自然混响而多采用混响室构建大型的高档录音棚系统，
		混响室的体积一般取100立方米或更小。主延迟时间越大，混响密度越稀疏，
		混响空间越大。
	*************************************************************************/
	virtual		bool	SetEnvironmentSize( FLOAT fValue )				=	0;
	virtual		bool	GetEnvironmentSize( FLOAT& fValue )				=	0;
	
	/*************************************************************************
	功能：设置扩散
	说明：环境漫射系数，用于确定混响声阵的密度增长率。
	*************************************************************************/
	virtual		bool	SetEnvironmentDiffusion( FLOAT fValue )			=	0;
	virtual		bool	GetEnvironmentDiffusion( FLOAT& fValue )		=	0;
	
	virtual		bool	SetFlags( DWORD dwValue )						=	0;
	virtual		bool	GetFlags( DWORD& dwValue )						=	0;
	
	/*************************************************************************
	功能：衰减时间量化
	说明：暂时定义，现在无法获知效果
	*************************************************************************/
	virtual		bool	SetScaleDecayTime( bool bValue )				=	0;
	virtual		bool	GetScaleDecayTime( bool& bValue )				=	0;
	
	/*************************************************************************
	功能：反射量化
	说明：暂时定义，现在无法获知效果
	*************************************************************************/
	virtual		bool	SetScaleReflections( bool bValue )				=	0;
	virtual		bool	GetScaleReflections( bool& bValue )				=	0;
	
	/*************************************************************************
	功能：反射延迟时间量化
	说明：暂时定义，现在无法获知效果
	*************************************************************************/
	virtual		bool	SetScaleReflectionsDelay( bool bValue )			=	0;
	virtual		bool	GetScaleReflectionsDelay( bool& bValue )		=	0;
	
	/*************************************************************************
	功能：混响大小量化
	说明：暂时定义，现在无法获知效果
	*************************************************************************/
	virtual		bool	SetScaleReverb( bool bValue )					=	0;
	virtual		bool	GetScaleReverb( bool& bValue )					=	0;
	
	/*************************************************************************
	功能：混响延迟时间量化
	说明：暂时定义，现在无法获知效果
	*************************************************************************/
	virtual		bool	SetScaleReverbDelay( bool bValue )				=	0;
	virtual		bool	GetScaleReverbDelay( bool& bValue )				=	0;
	
	/*************************************************************************
	功能：高频衰减切除
	说明：在衰减过程中，声源头前的混响信号会受到后发声的掩蔽影响，
		通过此选项在需要的时候切断后发声。
	*************************************************************************/
	virtual		bool	SetClipDecayHF( bool bValue )					=	0;
	virtual		bool	GetClipDecayHF( bool& bValue )					=	0;
	
	//////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////
#endif  //#ifndef _ITQSOUND_H_2006_4_25_9_18



