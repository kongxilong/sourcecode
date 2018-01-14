#include "stdafx.h"
#include "BM.h"
#include "ADOExcel.h"


CADOExcel::CADOExcel(void)
{
	m_fileType = "xls";
}


CADOExcel::~CADOExcel(void)
{
}

void CADOExcel::SetExcelFilePath(CString pathOfExcel)
{
	m_path = pathOfExcel;
}
void CADOExcel::SetFileType(CString fileType)
{
	m_fileType = fileType;
}

BOOL CADOExcel::OnInitConnect()
{
	
	
	::CoInitialize(NULL);
	try
	{
		m_pConnection.CreateInstance(__uuidof(Connection));
		CString strSQL;
		if( "xls" == m_fileType){
			strSQL.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties='Excel 8.0;HDR=YES'"),m_path); //m_fn表示输入的excel文件路径名
		} 
		
		if( "xlsx" == m_fileType){
			strSQL.Format(_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s;Extended Properties='Excel 12.0;HDR=YES'"),m_path);    //m_fn表示输入的excel文件路径名
		} 
		//strSQL.Format(_T("Driver=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties=Excel 8.0"),m_path); //m_fn表示输入的excel文件路径名
		m_pConnection->Open((_bstr_t)strSQL,"","",adModeUnknown);
		
	}
	catch(_com_error e)
	{
		CString err;
		err.Format(_T("Excel连接失败，错误信息为：%s"),e.ErrorMessage());
		AfxMessageBox(err);
		return FALSE;
	}
	return TRUE;
}

_RecordsetPtr& CADOExcel::GetRecordSet(CString str)
{
	_bstr_t bstrSQL = (_bstr_t)str;
	try
	{
		if(m_pConnection==NULL)
			OnInitConnect();
		m_pRecordset.CreateInstance(_uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		CString erro;
		erro.Format(_T("获取记录集失败，错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
		AfxMessageBox(erro);
		m_pRecordset = NULL;
		return m_pRecordset;
	}
	return m_pRecordset;
}


BOOL CADOExcel::ExecuteSQL(CString str)
{
	BOOL nflag = TRUE;
	_bstr_t bstrSQL = (_bstr_t)str; 
	try
	{
		if(NULL == m_pConnection)
		{
			nflag = OnInitConnect();
		}
		m_pConnection->BeginTrans();
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		m_pConnection->CommitTrans();
	}
	catch(_com_error e)
	{
		m_pConnection->RollbackTrans();
		CString erro;
		erro.Format(_T("执行SQL语句失败的错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
		AfxMessageBox(erro);
		nflag = FALSE;
	}
	return nflag;
}
void CADOExcel::ExitConnect()
{
	if(m_pRecordset!=NULL)
	{
		m_pRecordset->Close();
		m_pConnection->Close();
	}
	::CoUninitialize();
}