
// sqlitedemo.h : sqlitedemo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CsqlitedemoApp:
// �йش����ʵ�֣������ sqlitedemo.cpp
//

class CsqlitedemoApp : public CWinAppEx
{
public:
	CsqlitedemoApp();


// ��д
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();
// ʵ��

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CsqlitedemoApp theApp;