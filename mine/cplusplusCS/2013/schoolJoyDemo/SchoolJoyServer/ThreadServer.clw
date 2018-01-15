; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CThreadServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ThreadServer.h"

ClassCount=3
Class1=CThreadServerApp
Class2=CThreadServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_THREADSERVER_DIALOG

[CLS:CThreadServerApp]
Type=0
HeaderFile=ThreadServer.h
ImplementationFile=ThreadServer.cpp
Filter=N

[CLS:CThreadServerDlg]
Type=0
HeaderFile=ThreadServerDlg.h
ImplementationFile=ThreadServerDlg.cpp
Filter=D
LastObject=IDC_LIST_USERLIST
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ThreadServerDlg.h
ImplementationFile=ThreadServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_THREADSERVER_DIALOG]
Type=1
Class=CThreadServerDlg
ControlCount=4
Control1=IDC_EDIT_LOG,edit,1352728708
Control2=IDC_STATIC,button,1342177287
Control3=IDC_LIST_USERLIST,SysListView32,1350631425
Control4=IDC_STATIC,button,1342177287

