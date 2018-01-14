// ThreadOperate.h: interface for the ThreadOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADOPERATE_H__BE764657_CCD5_4502_A713_948B4CC177B7__INCLUDED_)
#define AFX_THREADOPERATE_H__BE764657_CCD5_4502_A713_948B4CC177B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Afxsock.h"
//#include "register.h"
//
//#include "ConnectSet.h"
//class CSearch;
//class ConnectSet;
class ThreadOperate  
{
public:
	ThreadOperate();
	virtual ~ThreadOperate();
	void startMainThread( void* cw);
	void startRequireInfoThread( void* cw);
	void startConnectServerThread( void* cw);
	void startSearchThread( void* cw);
	void startRegisterThread(void* cw);
	void startUploadThread(void * cw);
	void UpdateSpeed(void *Mydlg);
	void StartLoginThread(void* cw);
	void StartQuitThread(void* cw);
	long GetOffset(void);                                   //获得断点位置
	CString GetSpeed();
	long SetOffset(long offset);
public:
	long _startOffset;										//起始位置，用于速度更新
	long _endOffset;										//终止位置，用于速度更新
	long Offset;                                            //断点位置
	//CConnectSet connectset;
	//CRegister _register;
private:
	CDialog *dialog;
	
};

struct FILEINFO
{   
	char fileName[100];
	char fileType[10];
	char fileownerIP[50];
	char filepath[150];
	float fileSize;
	char username[100];
};
struct USERINFO
{
	char username[50];
	char password[20];
	char userIP[50];
};

#endif // !defined(AFX_THREADOPERATE_H__BE764657_CCD5_4502_A713_948B4CC177B7__INCLUDED_)

