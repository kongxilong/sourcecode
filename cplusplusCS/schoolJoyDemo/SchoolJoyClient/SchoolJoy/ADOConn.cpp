// ADOConn.cpp: implementation of the ADOConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ADOConn::ADOConn()
{

}

ADOConn::~ADOConn()
{

}

void ADOConn::OnInitDBConnect()
{
	//初始化OLE/COM库环境
	::CoInitialize(NULL);
	try
	{
		//创建connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		//设置连接字符串,必须是BSTR型或者_bstr_t类型
		m_pConnection->Open("Driver=SQL Server;Database=StudentManagement;Server=WQDNAN-PC;","","",adModeUnknown);
	}
	//捕捉异常
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());//显示错误信息
	}
}

_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)//执行查询
{
	try
	{	
		if(m_pConnection==NULL)//连接对象不存在
			OnInitDBConnect();//重新连接
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建记录集对象
		//执行查询，得到记录集
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),
						adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)//捕捉异常
	{
		AfxMessageBox(e.Description());//显示错误信息
	}
	return m_pRecordset;//返回记录集
}

BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)//执行SQL操纵语句
{
	try
	{
		if(m_pConnection==NULL)//连接对象不存在
			OnInitDBConnect();//重新连接
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);//执行
		return true;
	}	
	catch(_com_error e)//捕捉异常
	{
		AfxMessageBox(e.Description());//显示错误信息
		return false;
	}
}

void ADOConn::ExitConnect()//断开连接
{
	if(m_pRecordset!=NULL)
	{
		m_pRecordset->Close();//关闭记录集
		m_pConnection->Close();//关闭连接
	}	
	::CoUninitialize();//释放环境
}
