
#include "StdAfx.h"
#include "UiFeatureAes.h"
#include "..\..\Inc\UiFeatureDefs.h"

#define AES_KEY_LEN_BITS			(256)

static char aes_key_buffer[33] = "1i$UQ@3w>7r$#TypO5^:<}*9)E+^5HjU";


CUiFeatureAes::CUiFeatureAes()
{
}

CUiFeatureAes::~CUiFeatureAes()
{
}

// 根据用户输入的字符串，计算AES密钥
bool CUiFeatureAes::CreateAesPassword(char* pszScr, BYTE** ppbyOutKey, int &nOutKeyLen)
{
	nOutKeyLen = 0;
	if (pszScr == NULL || ppbyOutKey == NULL || strlen(pszScr) > USER_PASSWORD_KEY_LEN)
		return false;

	nOutKeyLen = strlen(pszScr);

	*ppbyOutKey = new BYTE[USER_PASSWORD_KEY_LEN];
	if (*ppbyOutKey == NULL)
		return false;

	memset(*ppbyOutKey, 0, USER_PASSWORD_KEY_LEN);

	aes_context AesContext;
	aes_set_key(&AesContext, (uint8*)aes_key_buffer, AES_KEY_LEN_BITS);

	char* pszFrom = pszScr;
	BYTE* pbyTo = *ppbyOutKey;
	uint8 input[16];
	uint8 output[16];

	int nLastEnDataLen = 0;
	for (int nEncryptLen = 0; nEncryptLen < nOutKeyLen; )
	{
		memset(input, 0, 16);
		memset(output, 0, 16);

		int nCopyLen = nOutKeyLen - nEncryptLen;
		nCopyLen = nCopyLen > 16 ? 16 : nCopyLen;

		memcpy(input, pszFrom, nCopyLen);

		aes_encrypt(&AesContext, input, output);

		memcpy(pbyTo, output, 16);

		nEncryptLen += nCopyLen;
		pszFrom += nCopyLen;
		pbyTo += 16;
		nLastEnDataLen += 16;
	}
	nOutKeyLen = nLastEnDataLen;

	return true;
}

// 根据AES密钥得到用户密码
bool CUiFeatureAes::GetUserPassword(BYTE* pbyAesData, int nAesDataLen, string &strUserPassword)
{
	strUserPassword = "";
	if (pbyAesData == NULL || nAesDataLen <= 0)
		return false;

	char *pszOutKey = new char[USER_PASSWORD_KEY_LEN + 1];
	if (pszOutKey == NULL)
		return false;

	memset(pszOutKey, 0, USER_PASSWORD_KEY_LEN + 1);

	aes_context AesContext;
	aes_set_key(&AesContext, (uint8*)aes_key_buffer, AES_KEY_LEN_BITS);

	BYTE* pbyFrom = pbyAesData;
	char* pszTo = pszOutKey;
	uint8 input[16];
	uint8 output[16];

	for (int nDecryptLen = 0; nDecryptLen < nAesDataLen; )
	{
		memset(input, 0, 16);
		memset(output, 0, 16);

		int nCopyLen = nAesDataLen - nDecryptLen;
		nCopyLen = nCopyLen > 16 ? 16 : nCopyLen;

		memcpy(input, pbyFrom, nCopyLen);

		aes_decrypt(&AesContext, input, output);

		memcpy(pszTo, output, nCopyLen);

		nDecryptLen += nCopyLen;
		pbyFrom += nCopyLen;
		pszTo += nCopyLen;
	}

	strUserPassword = pszOutKey;
	SAFE_DELETE_LIST(pszOutKey);
	return true;
}

// 匹配用户输入的密码和AES数据是否配对
bool CUiFeatureAes::Compare(BYTE* pbyAesData, int nAesDataLen, char* pUserPassword)
{
	if (pbyAesData == NULL || nAesDataLen <= 0 || pUserPassword == NULL || strlen(pUserPassword) <= 0)
		return false;

	string strOutKey = "";
	if (!GetUserPassword(pbyAesData, nAesDataLen, strOutKey))
		return false;

	if (lstrcmpA(strOutKey.c_str(), pUserPassword) != 0)
		return false;

	return true;
}
