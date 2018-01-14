// UserBean.h: interface for the UserBean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERBEAN_H__7834F902_95DA_4078_B109_06B271F1B98B__INCLUDED_)
#define AFX_USERBEAN_H__7834F902_95DA_4078_B109_06B271F1B98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class UserBean  
{
public:
	UserBean();
	virtual ~UserBean();
	void setPort(int port);
	void setClientAddress(CString clientAddress);
	int getPort();
	CString getClientAddress();
	void add(UserBean userBean);
public:
	CString clientAddress;
	int clientPort; 
};

#endif // !defined(AFX_USERBEAN_H__7834F902_95DA_4078_B109_06B271F1B98B__INCLUDED_)
