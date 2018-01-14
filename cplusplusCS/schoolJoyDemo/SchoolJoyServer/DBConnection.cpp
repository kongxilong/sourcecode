// DBConnection.cpp: implementation of the DBConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "DBConnection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBConnection::DBConnection()
{
	::CoInitialize(NULL);
}

DBConnection::~DBConnection()
{
	::CoUninitialize(); 
}

_ConnectionPtr DBConnection::getConnection(){
	HRESULT hr;
	hr=m_pConnection.CreateInstance(_uuidof(Connection));//实例化应用程序的Connection对象
	if(FAILED(hr)){
		AfxMessageBox("Connection句柄创建失败");
	}
	try{
		_bstr_t strConnect ="Provider=SQLOLEDB;Data Source=10.6.11.243;Initial Catalog=P2PDBMS;uid=sa; pwd=sa";//建立到数据库连接的字符串
		hr=m_pConnection->Open(strConnect,"","",adModeUnknown);//连接指定数据库
	}
	catch(_com_error e){
		AfxMessageBox(e.ErrorMessage());
	}
	return m_pConnection;
}
