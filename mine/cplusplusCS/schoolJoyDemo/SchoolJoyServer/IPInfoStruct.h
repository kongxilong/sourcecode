// IPInfoStruct.h: interface for the IPInfoStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPINFOSTRUCT_H__05B9BDE7_3BDC_4778_A3C5_45C425370211__INCLUDED_)
#define AFX_IPINFOSTRUCT_H__05B9BDE7_3BDC_4778_A3C5_45C425370211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef struct{
char sourceIP[32];
	int sourcePort;
	char destIP[32];
	int destPort;
	char source_natIP [32];
	int source_natPort;
	char dest_natIP[32];
	int dest_natPort;

}IPInfoStruct;

#endif // !defined(AFX_IPINFOSTRUCT_H__05B9BDE7_3BDC_4778_A3C5_45C425370211__INCLUDED_)
