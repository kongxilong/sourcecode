// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// BM.h : BM 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CBMApp:
// 有关此类的实现，请参阅 BM.cpp
//
#include "Sock.h"

class CBMApp : public CWinAppEx
{
public:
	CBMApp();


// 重写
public:
	virtual BOOL LoginInitInstance();   //在登录成功之后初始化主框架 并显示主窗口
	virtual BOOL InitInstance();		//默认初始化程序
	virtual int ExitInstance();			

// 实现
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	static HMODULE hMod ;         //这里主要是使用了rich edit2.0 control控件 需要初始化
	//CSock   m_sockOnly;             
	SOCKET	m_sHost;		  //全局唯一一个实例化的sock对象
public:
	void        InitSocket();       //初始化套接字
	
};

extern CBMApp theApp;
