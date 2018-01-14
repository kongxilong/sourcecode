/*!
 @file		Common.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-5
 @brief	    基础函数的一些定义
 */
#include <string>
#pragma once

class Common
{
public:
	Common();
	~Common();
public:
	static std::string   CString2String(const CString& src);
	static CString       String2CString(const std::string& src);
	static CString       String2CString(const char* src);
	static CString       String2CString(char* src);
	static CString       GetAppPath();
	static void          OutputDebugInfo(const CString& info);
	static void          OutputDebugInfo(const TCHAR* format,...);
};