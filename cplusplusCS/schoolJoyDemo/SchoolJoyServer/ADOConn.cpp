#include "stdafx.h"
#include "ADOConn.h"
void ADOConn::OnInitConnect()
{
	::CoInitialize(NULL);
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect =_T("driver={SQL Server};Server=(local);DATABASE=schooljoy");
		_bstr_t strid=_T("sa");
		_bstr_t strpassword=_T("schooljoy");
		m_pConnection->Open(strConnect,strid,strpassword,adModeUnknown);  
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("Á¬½ÓÊ§°Ü"));
	}
}
_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitConnect();
		m_pRecordset.CreateInstance(_uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}
BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitConnect();
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}
void ADOConn::ExitConnect()
{
	if(m_pRecordset!=NULL)
	{
		m_pRecordset->Close();
		m_pConnection->Close();
	}
	::CoUninitialize();
}