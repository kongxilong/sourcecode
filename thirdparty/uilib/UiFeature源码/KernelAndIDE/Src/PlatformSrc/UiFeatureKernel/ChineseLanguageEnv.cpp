
#include "stdafx.h"
#include "ChineseLanguageEnv.h"

CChineseLanguageEnv::CChineseLanguageEnv()
{
	m_lcid = GetThreadLocale();
	SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC));
}

CChineseLanguageEnv::~CChineseLanguageEnv()
{
	SetThreadLocale(m_lcid);
}
