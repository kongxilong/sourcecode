// UserBean.cpp: implementation of the UserBean class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "UserBean.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UserBean::UserBean()
{

}

UserBean::~UserBean()
{

}
void UserBean::setClientAddress(CString clientAddress){
	this->clientAddress = clientAddress;
}
void UserBean::setPort(int clientPort){
	this->clientPort = clientPort;
}

CString UserBean::getClientAddress(){
	return this->clientAddress;
}
int UserBean::getPort(){
	return clientPort;
}
void UserBean::add(UserBean userBean){
}