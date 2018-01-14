// ServSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "ServSearch.h"
#include "afxdialogex.h"
#include "ServiceSearch.h"

// CServSearch 对话框

IMPLEMENT_DYNAMIC(CServSearch, CDialogEx)

CServSearch::CServSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServSearch::IDD, pParent)
	, m_keyword(_T(""))
{

}

CServSearch::CServSearch(CServiceSearch* pServiceSearch,CWnd* pParent)
	: CDialogEx(CServSearch::IDD, pParent)
{
	m_pServiceSearch = pServiceSearch;
}
 

CServSearch::~CServSearch()
{
	m_pServiceSearch = NULL;
}

void CServSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEYWORD, m_keyword);
}


BEGIN_MESSAGE_MAP(CServSearch, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_FINDALL, &CServSearch::OnBnClickedBtnFindall)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CServSearch::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_RADIO_SERV_DATE, &CServSearch::OnBnClickedRadioServDate)
	ON_BN_CLICKED(IDC_RADIO_CTM_NAME, &CServSearch::OnBnClickedRadioCtmName)
END_MESSAGE_MAP()


// CServSearch 消息处理程序
BOOL CServSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadDefaultChioceFromIni();

	return TRUE;
}

HBRUSH CServSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b;
	}
	return hbr;
}


void CServSearch::OnBnClickedBtnFindall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pServiceSearch->FindAllServ();
	CDialogEx::OnOK();
}


void CServSearch::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pServiceSearch->m_searchByKeyWord.Empty();
	m_pServiceSearch->m_searchByKeyWord = _T("条件搜索,");
	UpdateData(TRUE);
	if(m_keyword.IsEmpty())   //如果搜索为空将搜索全部员工资料
	{
		if(IDYES == MessageBox(_T("搜索关键字为空，将搜索全部服务，确认请继续"),_T("提示"),MB_YESNO))
		{
			m_pServiceSearch->FindAllServ();
			CDialogEx::OnOK();
		}else{
			return;
		}
	}else{
		CString _strFind = _T(",");
		int flag = m_keyword.Find(_strFind);
		if(-1 != flag)   //包含逗号
		{
			MessageBox(_T("关键字不能包含逗号！"));
			m_keyword.Empty();
			UpdateData(FALSE);
			return;
		}
		if(m_keyword.GetLength()>30)
		{
			MessageBox(_T("关键字不能超过30个字符"));
			return;
		}
		if(1 == ((CButton*)GetDlgItem(IDC_RADIO_SERV_DATE))->GetCheck())
		{
			m_pServiceSearch->m_searchByKeyWord += _T("服务日期,")+ m_keyword;
		}else
		{
			m_pServiceSearch->m_searchByKeyWord += _T("客户名称,")+ m_keyword;
		}
	}
	void* dlg = m_pServiceSearch;
	CSock::StartFindServByKeyWord(dlg);
	CDialogEx::OnOK();
}

void CServSearch::LoadDefaultChioceFromIni()
{
	CString chioce = NULL;
	CFileFind finder;
	BOOL isFind = finder.FindFile(_T(".//BMClient.ini"));
	if(isFind)
	{
		GetPrivateProfileString(_T("服务搜索"),_T("默认条件"),_T("-1"),chioce.GetBuffer(10),10,_T(".//BMClient.ini")); 
		if(_T("-1") == chioce)
		{
			CheckDlgButton(IDC_RADIO_SERV_DATE,1);
		}else{
			if(_T("服务日期") == chioce)
			{
				CheckDlgButton(IDC_RADIO_SERV_DATE,1);
			}else{
				CheckDlgButton(IDC_RADIO_CTM_NAME,1);
			}
		}
	}else{   //不存在该文件  加载默认配置
		CheckDlgButton(IDC_RADIO_SERV_DATE,1);
	}
}


void CServSearch::OnBnClickedRadioServDate()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("服务日期");
	WritePrivateProfileString(_T("服务搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}


void CServSearch::OnBnClickedRadioCtmName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("客户名称");
	WritePrivateProfileString(_T("服务搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}
