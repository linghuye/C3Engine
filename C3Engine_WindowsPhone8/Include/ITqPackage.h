/********************************************************************
Created:		2006_4_10 21:18
Filename:		ITqPackage.h
Author:		    Huang Yuhang
Purpose:		
********************************************************************/
#ifndef _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
#define _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/
#include <stdio.h>
/********************************************************************/
#ifdef TQPACKAGE_EXPORTS
#define TQPACKAGE_API __declspec(dllexport)
#else
#define TQPACKAGE_API __declspec(dllimport)
#endif
/********************************************************************/
#define TQEDITOR
/********************************************************************/
enum TQPRESULT{
	TQR_OK					=		0, // 正常
	TQR_PARAMETER_ERROR		=		1, // 参数检查错误
	TQR_BUFFER_OVERFLOW		=		2, // 缓冲溢出
	TQR_FILE_NOTFOUND		=		3, // 未找到文件
	TQR_FILE_OPEN_ERROR		=		4, // 打开文件失败
	TQR_FILE_READ_ERROR		=		5, // 文件读失败
	TQR_FILE_WRITE_ERROR	=		6, // 文件写失败
	TQR_WRITEMUTEX_ERROR	=		7, // 包写操作互斥
	TQR_READMUTEX_ERROR		=		8, // 包读操作互斥
	TQR_COMPRESS_ERROR		=		9, // 压缩或解压错误
	TQR_NOT_MATCHING		=		10, // 数据与索引文件不匹配
	TQR_NOT_OPENFILE		=		11,	// 文件未打开
	TQR_REPACK_OK			=		12,	// 重整包成功
	TQR_REPACK_NOTINIT		=		13, // 未初始化
	TQR_FILENAME_ERROR		=		14,	// 文件名错误
	TQR_NOTINIT				=		15,	// 没初始化
	TQR_INVALID_BLOCK		=		16,	// 无效的块
	TQR_ALLOCMEMORY_ERROR	=		17,	// 分配内存失败
};

//数据文件读写模式
enum OpenFileMode{
	OpenFileMode_Read		=		0,//只读
	OpenFileMode_ReadWrite	=		1,//读写
};
/********************************************************************/
class ITqPackage
{
public:
	// 自释放接口
	virtual		ULONG		Release(void)													=	0;

	// 检查文件是否存在
	virtual		bool		CheckFile( const char* pszFileName, ULONG* uFileSize=NULL )		=	0; 

	// 创建package...
	virtual		TQPRESULT	Create(const char* pszPackageFileName)							=	0;

	// 打开package...
	virtual		TQPRESULT	Open(const char* pszPackageFileName, enum OpenFileMode eMode )	=	0;
	
	// 载入文件到Buffer，uSize传入Buffer大小，传出解压后文件大小
	virtual		TQPRESULT	Load(const char* pszFileName, BYTE* pBuffer, ULONG& uSize)		=	0;
	// 关闭package...
	virtual		TQPRESULT	Close(void)														=	0;
		
	// 添加或更新一个文件
	virtual		TQPRESULT   AddFile(const char* pszFileName)								=	0;
	
	// 更新索引文件 
	virtual		TQPRESULT   UpdateIndex(void)												=	0;

	// 删除一个文件
	virtual		TQPRESULT	DelFile(const char* pszFileName)								=	0;

	// 整理碎片
	virtual		TQPRESULT	Repack( ULONG& uItem, char* pFileName )							=	0;

	// 取文件数量
	virtual		ULONG		GetFileAmount(void)												=	0;
	
#ifdef TQEDITOR // 不随产品发布
	virtual		const char*	GetFileName(ULONG uIndex)										=	0;
#endif	//TQEDITOR
};

TQPACKAGE_API ITqPackage* TqPackageCreate();




/********************************************************************/
enum FILETYPE//文件类型
{
	FILETYPE_ALL	=	0,	// 所有
	FILETYPE_UNPACK,		// 末打包的文件
	FILETYPE_PACK,			// 打包的文件
};

enum TQSEEKMODE
{
	TQSEEK_CUR		=	SEEK_CUR,
	TQSEEK_END		=	SEEK_END,
	TQSEEK_BEGIN	=	SEEK_SET,
};
typedef void*	HTQF;		// TqFileHandle
/********************************************************************/

///////////////////////数据包相关//////////////////////////////////////
// 打开一个数据包文件
TQPRESULT	TQPACKAGE_API		TqPackagesOpen( const char* pszPackageFile );

// TQPackageClose 关闭数据包文件 输入NULL全部关闭
void		TQPACKAGE_API		TqPackagesClose( const char* pszPackageFile );
////////////////////////文件相关////////////////////////////////////////

// 探测指定文件是否在指定的文件列表中
bool		TQPACKAGE_API		TqFCheck( const char* pszFile, FILETYPE eFileType );

// 加载某个文件, 返回缓冲区以及缓冲区大小; 如果数据包文件中没有找到这个文件, 则返回false
TQPRESULT	TQPACKAGE_API		TqFDump( const char* pszFile, void*& pBuf, ULONG& uSize );

// 清空DUMP缓冲区
TQPRESULT	TQPACKAGE_API	 	TqFUndump( void* pBuffer );

// 定位数据包中的某个文件	 
TQPRESULT	TQPACKAGE_API		TqFOpen( const char* pszFile, HTQF& hTqf );

//关闭
TQPRESULT	TQPACKAGE_API		TqFClose( HTQF hTqf );

// 从文件读入一个数据块, 返回读入的数据大小(buf由外部分配)(兼容fread)
LONG		TQPACKAGE_API		TQFRead( void* pBuf, ULONG uSize, ULONG uCount, HTQF hTqf );

// 定位文件偏移
bool		TQPACKAGE_API		TqFSeek( HTQF hTqf, LONG uOffset, TQSEEKMODE eSeek );

// 取得读写指针位置
ULONG		TQPACKAGE_API		TqFTell( HTQF hTqf );

// 探测文件是否到文件尾
bool		TQPACKAGE_API		TqFIsEof( HTQF hTqf );

//取文件长度
LONG		TQPACKAGE_API		TqFGetLen( HTQF hTqf );

//取版本号
LONG		TQPACKAGE_API		TqFGetVersion(void);

TQPRESULT	TQPACKAGE_API		TqPackagesInit(const char* pszAppFolderPath);
/********************************************************************/
#endif  //_INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/



