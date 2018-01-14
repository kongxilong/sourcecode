/*!
 @file		Common.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-5
 @brief	    基础函数的一些定义实现
 */

#include "..\..\stdafx.h"
#include "Common.h"

Common::Common()
{

}

Common::~Common()
{

}

std::string  Common::CString2String(const CString& src)
{
	std::string dest;
	int wlen = src.GetLength();

#ifdef _UNICODE		
	char* buf = new char[wlen*2+1] ;
	int len = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)src, wlen, buf, wlen*2+1, NULL, NULL);
	buf[len] = '\0';
	dest.erase();
	dest.append(buf,len);
	delete [] buf;
	return dest;
#else
	dest = (LPCTSTR)src;	
	return dest;
#endif	
}

CString   Common::String2CString(const char* src)
{
    CString des(_T(""));
	if(!src)
	{
		return des;
	}
    return CString(src);
}

CString Common::String2CString(char* src)
{
	return String2CString(const_cast<char*>(src));
}

CString Common::String2CString(const std::string& src)
{
	return String2CString(src.data());
}


//获得当前应用程序示例的路径
CString Common::GetAppPath()
{
	CString sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind ('\\');
	sPath = sPath.Left (nPos);
	return sPath;
}

//可以输出格式化的字符串
void Common::OutputDebugInfo(const TCHAR* format,...)
{		
#ifdef DEBUG
	va_list args;
	va_start(args, format);
	TCHAR	buf[1024];
	memset(buf, 0, sizeof(TCHAR) * 1024);
	_vsntprintf(buf, 1024, format, args);
	va_end(args);

	CString sText = CString(buf);
	OutputDebugInfo(sText);
#endif
}


//控制台输出信息
void Common::OutputDebugInfo(const CString& text)
{
#ifdef DEBUG
	CString outString;
	SYSTEMTIME now;
	GetLocalTime(&now);
	outString.Format(_T("%d-%d-%d:%d---%s \n"),now.wHour,  now.wMinute,now.wSecond,now.wMilliseconds, text.GetString());
	OutputDebugString(outString);
#endif
}

