// OnlineUserList.cpp: implementation of the OnlineUserList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "OnlineUserList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OnlineUserList::OnlineUserList()
{

}

OnlineUserList::~OnlineUserList()
{

}
CPtrArray OnlineUserList::socketList;
list<UserBean> OnlineUserList::userList = list<UserBean>();
list<UserBean> OnlineUserList::getuserList(){
	return userList;
}

void OnlineUserList::removeSocket(CString ipAddress,int port){
	AfxSocketInit(NULL);
	critical.Lock();
	CString localAddress ;
	UINT localPort ;
	for(int i=0;i<socketList.GetSize();i++){
		CSocket *pClientSocket =  (CSocket*)socketList.GetAt(i);
		pClientSocket->GetPeerName(localAddress,localPort);
		if ((localAddress==ipAddress) && (localPort == port)){ 
			socketList.RemoveAt(i);
			delete pClientSocket;
			i--;
		}
	}
	critical.Unlock();
}

CSocket* OnlineUserList::addClientSocket(){
	AfxSocketInit(NULL) ;
	CSocket *pSockClient = new CSocket;
	if ( !pSockClient )
	critical.Lock();
	socketList.Add ( pSockClient );
	critical.Unlock();
	return pSockClient;
}

CSocket* OnlineUserList::canFind(CString ipAddress,int port){
	AfxSocketInit(NULL);
	critical.Lock();
	CString localAddress ;
	UINT localPort ;
	for(int i=0;i<socketList.GetSize();i++){
		CSocket *pClientSocket =  (CSocket*)socketList.GetAt(i);
		pClientSocket->GetPeerName(localAddress,localPort);
		if ((localAddress==ipAddress) && (localPort == port)){ 
			critical.Unlock();
			return pClientSocket;
		}
	}
	critical.Unlock();
	return NULL;
}
bool OnlineUserList::canFind(UserBean userInfo){
	std::list<UserBean>::iterator it;
	for(it = userList.begin();it!=userList.end();it++){
		UserBean tempUser = *it;
		if(tempUser.getClientAddress()==userInfo.getClientAddress()&&tempUser.getPort()==userInfo.getPort()){
			return true;
			break;
		}
	}
	return false;
}
void  OnlineUserList::addUser(UserBean userInfo){
	userList.push_back(userInfo);
}
void OnlineUserList::removeUser(UserBean userInfo){
	std::list<UserBean>::iterator it =userList.begin();
	while(it!=userList.end()){
		UserBean tempUser = *it;
		if((tempUser.getClientAddress()==userInfo.getClientAddress())){
			userList.erase(it++);
			break;
		}else
			++it;
	}
}