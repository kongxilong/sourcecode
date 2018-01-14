// OnlineUserList.h: interface for the OnlineUserList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEUSERLIST_H__D64BD6E2_FE59_4EDB_A4F8_493313C56D87__INCLUDED_)
#define AFX_ONLINEUSERLIST_H__D64BD6E2_FE59_4EDB_A4F8_493313C56D87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <list>
using std::list;
#include "UserBean.h"
#include "afxsock.h"
#include "afxmt.h"
class OnlineUserList  
{
public:
	OnlineUserList();
	virtual ~OnlineUserList();
   
	void static addUser(UserBean userInfo);
	bool static canFind(UserBean userInfo);
    list<UserBean> getuserList();
	void static removeUser(UserBean userInfo);
	static CPtrArray socketList;
	static list<UserBean> userList;
	void removeSocket(CString ipAddress,int port);
	CSocket* canFind(CString ipAddress,int port);
	CCriticalSection critical;
	CSocket* addClientSocket();
};

#endif // !defined(AFX_ONLINEUSERLIST_H__D64BD6E2_FE59_4EDB_A4F8_493313C56D87__INCLUDED_)
