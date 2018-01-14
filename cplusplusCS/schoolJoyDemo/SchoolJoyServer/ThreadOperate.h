// ThreadOperate.h: interface for the ThreadOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADOPERATE_H__D4BEB85D_F13C_4510_8E31_11A825831EC7__INCLUDED_)
#define AFX_THREADOPERATE_H__D4BEB85D_F13C_4510_8E31_11A825831EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Afxsock.h"
class ThreadOperate  
{
public:
	ThreadOperate();
	virtual ~ThreadOperate();
    void   startMainThread( void* cw);
	void   startshowUserThread( void* cw);
	void   startshowResourcesThread( void* cw);
	
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

#endif // !defined(AFX_THREADOPERATE_H__D4BEB85D_F13C_4510_8E31_11A825831EC7__INCLUDED_)
