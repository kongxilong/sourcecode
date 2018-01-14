// FragmentBean.h: interface for the FragmentBean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAGMENTBEAN_H__590CD190_284B_400C_8D24_E661A519279B__INCLUDED_)
#define AFX_FRAGMENTBEAN_H__590CD190_284B_400C_8D24_E661A519279B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct{
	CString fileName;
	int orderId;
	long startIndex;
	long endIndex;
	long totalFragment;
	CString clientAddress;
	int clientPort;
	CString natAddress;
	int natPort;
}FragmentBean;

#endif // !defined(AFX_FRAGMENTBEAN_H__590CD190_284B_400C_8D24_E661A519279B__INCLUDED_)
