
#pragma once
#include <Windows.h>
#include "..\..\..\3rd\AES\AES.h"

class CUiFeatureAes
{
public:
	CUiFeatureAes();
	~CUiFeatureAes();

	// 根据用户输入的字符串，计算AES密钥
	bool CreateAesPassword(char* pszScr, BYTE** ppbyOutKey, int &nOutKeyLen);
	// 根据AES密钥得到用户密码
	bool GetUserPassword(BYTE* pbyAesData, int nAesDataLen, string &strUserPassword);
	// 匹配用户输入的密码和AES数据是否配对
	bool Compare(BYTE* pbyAesData, int nAesDataLen, char* pUserPassword);

};
