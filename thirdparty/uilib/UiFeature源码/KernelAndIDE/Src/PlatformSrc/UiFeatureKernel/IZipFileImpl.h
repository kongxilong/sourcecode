
#pragma once
#include "..\..\Inc\IZipFile.h"
#include "UiFeatureAes.h"
#include <atlstr.h>
using namespace ATL;


//////////////////////////////////////////////////////////////////////////
// 文件头标示3字节：0xE1F2D3
// 扩展文件头
// ||【1字节】皮肤包结构版本号【1字节】密码长度（密码最大长度255）【N字节】密码||
// 皮肤文件文件结构
// ||【4字节】文件个数----（【1字节】文件类型----【2字节】单个文件名称长度----文件名称----【4字节】原文件内容长度----【4字节】压缩后文件内容长度----压缩文件内容）||
// 文件尾标示3字节：0xD1F2E3
//////////////////////////////////////////////////////////////////////////

// 不需要压缩的文件后缀名
typedef vector<CStringW>				NO_COMPRESS_FILE_VEC;

// 密码信息
struct VER_PASSWORD
{
	SKIN_VERSION skinVer;
	string strSrcPassword;
	int nDstLen;
	BYTE* pbyDstPassword;

	VER_PASSWORD()
	{
		skinVer = SV_0;
		strSrcPassword.clear();
		nDstLen = 0;
		pbyDstPassword = NULL;
	}
};


class IZipFileImpl : public IZipFile
{
public:
	IZipFileImpl();
	virtual ~IZipFileImpl();

	// 取得皮肤包版本号
	virtual SKIN_VERSION GetSkinFileVersion(LPCWSTR pZipFilePath, int &nPasswordLen);
	// 验证用户皮肤包密码
	virtual bool ChekUserPassword_SV_1(const char *pUserPassword, LPCWSTR pZipFilePath);

	virtual void RemoveFile(ZIP_FILE *pRemove);
	// 加入一个本地文件到当前的zip文件列表中
	virtual ZIP_FILE* AppendLocalFileToZipFile(LPCWSTR pLocalFilePath, LPCWSTR pKeyFileName);

//// 读取zip文件 //////////////////////////////////////////////////////////////////////
	// 读取zip文件
	virtual bool ReadZipFile(LPCWSTR pZipFilePath);
	// 取得解压缩文件后的文件列表
	virtual ZIP_FILE_MAP *GetUnZipFileMap();
	virtual ZIP_FILE *FindUnZipFile(LPCWSTR pFileName);

	// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
	virtual bool ReadZipFileItem(ZIP_FILE* pZipItem);

//// 创建zip文件 //////////////////////////////////////////////////////////////////////
	// 初始化zip文件，pSrcFileDir：需要压缩的源文件目录
	virtual bool WriteZipInit(LPCWSTR pSrcFileDir, LPCWSTR pSaveZipFile, SKIN_VERSION skinVer, char* pszPassword);
	// 写入一个文件，pFilePath：必须是相对于WriteZipInit函数的pSrcFileDir路径的相对路径
	virtual bool WriteZipAppendFile(LPCWSTR pFileName, ZIP_FILE_TYPE ZipType);
	// 写入一段buffer，pFileName：必须是相对于WriteZipInit函数的pSrcFileDir路径的相对路径
	virtual bool WriteZipAppendBuffer(LPCWSTR pFileName, BYTE *pBuffer, int nBufferLen, ZIP_FILE_TYPE ZipType);
	// 向zip文件写入一段数据
	virtual bool WriteZipAppendStruct(ZIP_FILE *pNormalFile);
	virtual bool WriteZipEnd(bool bEntToClear);

//////////////////////////////////////////////////////////////////////////
	// 测试函数，将解压后的文件写入本地一个指定目录
	virtual void TestWriteUnZipFile(LPCWSTR pszOutDir);

	void Clear();
	bool FindFileName(LPCWSTR pFileName);

private:
	bool CreatePassword_SV_1(VER_PASSWORD &Password);
	bool GerPassword_SV_1(VER_PASSWORD &Password);
	bool ComparePassword_SV_1(VER_PASSWORD &Password);
	void ReleasePassword(VER_PASSWORD &Password);

private:
	ZIP_FILE_MAP m_ZipFileMap;
	// 皮肤路径
	SKIN_FILE_LIST m_SkinFileList;

//////////////////////////////////////////////////////////////////////////
	// 写入zip用
	CStringW m_strSrcFileDir;
	CStringW m_strSaveZipFile;

	VER_PASSWORD m_Password;
};
