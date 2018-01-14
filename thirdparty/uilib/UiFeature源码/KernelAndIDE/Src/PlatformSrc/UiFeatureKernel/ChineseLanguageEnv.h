
#pragma once
#include <Windows.h>

// 改变系统语言环境专用的类
class CChineseLanguageEnv
{
public:
	CChineseLanguageEnv();
	~CChineseLanguageEnv();

private:
	LCID m_lcid;
};
