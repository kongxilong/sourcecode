
#pragma once
#include "CDrawingImage.h"
#include "IPropertyBase.h"
#include "IPropertyFontBase.h"

#include <string>
#include <map>
#include <vector>
#include <list>
using namespace std;

#include <atlstr.h>
using namespace ATL;

// 密码最大字符长度
#define SKIN_FILE_PASSWORD_LEN			(255)

// zip文件中的文件类型
enum ZIP_FILE_TYPE
{
	ZFT_NORMAL		= 0,
	ZFT_IMAGE
};

// 压缩文件列表
struct ZIP_FILE 
{
	WCHAR strFileName[MAX_PATH + 1];
	// 这个变量使用的公用的内存，不占用字符控件，也不需要释放，系统会自行释放
	LPCWSTR pstrSkinPath;
	BYTE byFileType;
	DWORD dwSrcFileLen;
	DWORD dwFileDataOffset;
	BYTE *pFileData;
	CDrawingImage DrawImg;

	ZIP_FILE()
	{
		memset(strFileName, 0, sizeof(strFileName));
		pstrSkinPath = NULL;
		byFileType = ZFT_NORMAL;
		dwSrcFileLen = dwFileDataOffset = 0;
		pFileData = NULL;
	};
};
// KEY:strFileName
typedef map<CStringW, ZIP_FILE*>		ZIP_FILE_MAP;
// 本地加载的皮肤包的队列
typedef list<CStringW>				SKIN_FILE_LIST;

// 皮肤包版本信息
enum SKIN_VERSION
{
	// 错误版本号
	SV_ERROR	= -1,
	// 最老的版本号
	SV_0		= 0,
	// 文件名称是非unicode的版本
	SV_1		= 1,
	// 文件名称是unicode的版本，SV_1 和 SV_2 版本的密码机制是完全一样的，区别有两点：
	// 1. 压缩包的中的文件名称的格式不一样，SV_2使用Unicode
	// 2. 压缩包中的文件压缩方式不一样，SV_2的读取速度更快
	SV_2		= 2,
};

// 一个压缩的皮肤包文件
class _declspec(novtable) IZipFile
{
public:

	// 取得皮肤包版本号
	virtual SKIN_VERSION GetSkinFileVersion(LPCWSTR pZipFilePath, int &nPasswordLen) = 0;
	// 验证用户皮肤包密码
	virtual bool ChekUserPassword_SV_1(const char *pUserPassword, LPCWSTR pZipFilePath) = 0;

	virtual void RemoveFile(ZIP_FILE *pRemove) = 0;
	// 加入一个本地文件到当前的zip文件列表中
	virtual ZIP_FILE* AppendLocalFileToZipFile(LPCWSTR pLocalFilePath, LPCWSTR pKeyFileName) = 0;

//// 读取zip文件 //////////////////////////////////////////////////////////////////////
	// 读取zip文件
	virtual bool ReadZipFile(LPCWSTR pZipFilePath) = 0;
	// 取得解压缩文件后的文件列表
	virtual ZIP_FILE_MAP *GetUnZipFileMap() = 0;
	virtual ZIP_FILE *FindUnZipFile(LPCWSTR pFileName) = 0;

	// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
	virtual bool ReadZipFileItem(ZIP_FILE* pZipItem) = 0;

//// 创建zip文件 //////////////////////////////////////////////////////////////////////
	// 初始化zip文件，pSrcFileDir：需要压缩的源文件目录
	virtual bool WriteZipInit(LPCWSTR pSrcFileDir, LPCWSTR pSaveZipFile, SKIN_VERSION skinVer, char* pszPassword) = 0;
	// 写入一个文件，pFilePath：必须是相对于WriteZipInit函数的pSrcFileDir路径的相对路径
	virtual bool WriteZipAppendFile(LPCWSTR pFileName, ZIP_FILE_TYPE ZipType) = 0;
	// 写入一段buffer，pFileName：必须是相对于WriteZipInit函数的pSrcFileDir路径的相对路径
	virtual bool WriteZipAppendBuffer(LPCWSTR pFileName, BYTE *pBuffer, int nBufferLen, ZIP_FILE_TYPE ZipType) = 0;
	// 向zip文件写入一段数据
	virtual bool WriteZipAppendStruct(ZIP_FILE *pNormalFile) = 0;
	virtual bool WriteZipEnd(bool bEntToClear) = 0;
};

// 一个皮肤包的信息
struct SKIN_FILE_ITEM
{
	// 皮肤路径
	CStringW strSkinFilePath;
	// 皮肤包压缩控制类
	IZipFile *pZipFileMgr;
	// 取得皮肤包中默认的字体信息
	IPropertyFontBase* pDefaultFontBase;
	// Resource.xml 所有种类的属性的队列
	IPropertyBaseMapMap AllPropMap;
	// Controls.xml 所有控件种类的属性的队列，KEY：控件类型名称，如：Button
	// 存储每一个控件的具体的属性，所有控件属性都存放在一个 IPropertyGroup 下
	IPropertyBaseMapMap AllCtrlPropMap;
	// Windows.xml 所有窗口的属性管理器map
	// 存储每一个窗口的具体的属性，所有窗口属性都存放在一个 IPropertyGroup 下
	IPropertyBaseMap AllWindowPropMap;
	// 布局队列，KEY：ObjectID，存储 IPropertyWindow IPropertyControl
	IPropertyBaseMap LayoutWindowMap;

	SKIN_FILE_ITEM()
	{
		strSkinFilePath = L"";
		pZipFileMgr = NULL;
		pDefaultFontBase = NULL;
		AllPropMap.clear();
		AllCtrlPropMap.clear();
		AllWindowPropMap.clear();
		LayoutWindowMap.clear();
	};
};
// 皮肤包map，KEY：皮肤包本地全路径
typedef map<CStringW, SKIN_FILE_ITEM*>		SKIN_FILE_MAP;
