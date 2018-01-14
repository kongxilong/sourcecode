
#include "stdafx.h"
#include "CA2W_W2A.h"
#include "..\..\Inc\UiFeatureDefs.h"

CA2W_W2A::CA2W_W2A()
{
	m_CharVec.clear();
	m_WcharVec.clear();
}

CA2W_W2A::~CA2W_W2A()
{
	for (CHAR_VEC::iterator pCharItem = m_CharVec.begin(); pCharItem != m_CharVec.end(); pCharItem++)
	{
		char* pszData = *pCharItem;
		SAFE_DELETE(pszData);
	}
	m_CharVec.clear();

	for (WCHAR_VEC::iterator pWCharItem = m_WcharVec.begin(); pWCharItem != m_WcharVec.end(); pWCharItem++)
	{
		WCHAR* pszData = *pWCharItem;
		SAFE_DELETE(pszData);
	}
	m_WcharVec.clear();
}

WCHAR* CA2W_W2A::WideChar(const char* pszMultiByte)
{
	return WideChar((char*)pszMultiByte);
}

WCHAR* CA2W_W2A::WideChar(char* pszMultiByte)
{
	if (pszMultiByte == NULL)
		return NULL;

	DWORD dwNum = ::MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, NULL, 0);
	WCHAR* pWideChar = new WCHAR[dwNum + 2];
	if (pWideChar == NULL)
		return NULL;

	memset(pWideChar, 0, (dwNum + 2) * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, pWideChar, dwNum + 1);

	m_WcharVec.push_back(pWideChar);

	return pWideChar;
}

char* CA2W_W2A::MultiByte(WCHAR* pszWideChar)
{
	if (pszWideChar == NULL)
		return NULL;

	DWORD dwNum = ::WideCharToMultiByte(CP_ACP, 0, pszWideChar, -1, NULL, 0, NULL, NULL);
	char* pMultiByte = new char[dwNum + 2];
	if (pMultiByte == NULL)
		return NULL;

	memset(pMultiByte, 0, dwNum + 2);
	::WideCharToMultiByte(CP_ACP, 0, pszWideChar, -1, pMultiByte, dwNum + 1, NULL, NULL);

	m_CharVec.push_back(pMultiByte);

	return pMultiByte;
}
