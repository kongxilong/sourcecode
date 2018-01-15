// EmpInfoSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "EmpInfoSearch.h"
#include "afxdialogex.h"


// CEmpInfoSearch 对话框

IMPLEMENT_DYNAMIC(CEmpInfoSearch, CDialogEx)

CEmpInfoSearch::CEmpInfoSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmpInfoSearch::IDD, pParent)
	, m_keyWord(_T(""))
{

}

CEmpInfoSearch::CEmpInfoSearch(CEmpInfoView* pView,CWnd* pParent)
	: CDialogEx(CEmpInfoSearch::IDD, pParent)
{
	m_pEmpInfoView = pView;
}


CEmpInfoSearch::~CEmpInfoSearch()
{
	m_pEmpInfoView = NULL;
}

void CEmpInfoSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY_WORD, m_keyWord);
}


BEGIN_MESSAGE_MAP(CEmpInfoSearch, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEARCH_ALL, &CEmpInfoSearch::OnBnClickedBtnSearchAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SEARCH_BY_KEYWORD, &CEmpInfoSearch::OnBnClickedBtnSearchByKeyword)
	ON_BN_CLICKED(IDC_BTN_GIVEUP, &CEmpInfoSearch::OnBnClickedBtnGiveup)
	ON_BN_CLICKED(IDC_RADIO_NAME, &CEmpInfoSearch::OnBnClickedRadioName)
	ON_BN_CLICKED(IDC_RADIO_NUM, &CEmpInfoSearch::OnBnClickedRadioNum)
	ON_BN_CLICKED(IDC_RADIO_ADDR, &CEmpInfoSearch::OnBnClickedRadioAddr)
	ON_BN_CLICKED(IDC_RADIO_POS, &CEmpInfoSearch::OnBnClickedRadioPos)
END_MESSAGE_MAP()


// CEmpInfoSearch 消息处理程序


void CEmpInfoSearch::OnBnClickedBtnSearchAll()
{
	// TODO: 在此添加控件通知处理程序代码
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_WAIT));  //圆圈 表示等待
	//m_pEmpInfoView->m_list.DeleteAllItems();

	void* dlg = m_pEmpInfoView;

	CSock::StartSearchAllEmpInfo(dlg);
	CDialogEx::OnOK();
}




HBRUSH CEmpInfoSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CEmpInfoSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//GetDlgItem(IDC_RADIO_NAME)->SetCheck(TRUE);
	LoadDefaultChioceFromIni();
	
	return TRUE;
}

void CEmpInfoSearch::OnBnClickedBtnSearchByKeyword()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pEmpInfoView->m_searchByKeyWord.Empty();
	UpdateData(TRUE);
	if(m_keyWord.IsEmpty())   //如果搜索为空将搜索全部员工资料
	{
		if(IDYES == MessageBox(_T("搜索关键字为空，将搜索全部员工，确认请继续。否则请输入搜索条件！"),_T("提示"),MB_YESNO))
		{
			void* dlg = m_pEmpInfoView;
			CSock::StartSearchAllEmpInfo(dlg);
			CDialogEx::OnOK();
		}else{
			return;
		}
	}else{
		CString _strFind = _T(",");
		int flag = m_keyWord.Find(_strFind);
		if(-1 != flag)   //包含逗号
		{
			MessageBox(_T("关键字不能包含逗号！"));
			m_keyWord.Empty();
			UpdateData(FALSE);
			return;
		}
		if(1 == ((CButton*)GetDlgItem(IDC_RADIO_NAME))->GetCheck())
		{
			m_pEmpInfoView->m_searchByKeyWord = _T("员工姓名,")+ m_keyWord;
		}else if(1 == ((CButton*)GetDlgItem(IDC_RADIO_NUM))->GetCheck())
		{
			m_pEmpInfoView->m_searchByKeyWord = _T("员工工号,")+ m_keyWord;
		}else if(1 == ((CButton*)GetDlgItem(IDC_RADIO_POS))->GetCheck())
		{
			m_pEmpInfoView->m_searchByKeyWord = _T("职务,")+ m_keyWord;
		}else if(1 == ((CButton*)GetDlgItem(IDC_RADIO_ADDR))->GetCheck())
		{
			m_pEmpInfoView->m_searchByKeyWord = _T("所属地点,")+ m_keyWord;
		}
	}
	//WriteDefaultChoiceToIni();
	void* dlg = m_pEmpInfoView;
	CSock::StartSearchEmpByKeyWord(dlg);
	//CSock::StartSearchAllEmpInfo(dlg);
	CDialogEx::OnOK();
}


void CEmpInfoSearch::OnBnClickedBtnGiveup()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CEmpInfoSearch::WriteDefaultChoiceToIni()
{
	CString toFind = _T(",");
	int index = m_pEmpInfoView->m_searchByKeyWord.Find(toFind);
	CString temp = m_pEmpInfoView->m_searchByKeyWord.Mid(0,index);
	WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}

void CEmpInfoSearch::LoadDefaultChioceFromIni()
{
	CString chioce = NULL;
	CFileFind finder;
	BOOL isFind = finder.FindFile(_T(".//BMClient.ini"));
	if(isFind)
	{
		GetPrivateProfileString(_T("员工搜索"),_T("默认条件"),_T("-1"),chioce.GetBuffer(10),10,_T(".//BMClient.ini")); 
		if(_T("-1") == chioce)
		{
			CheckDlgButton(IDC_RADIO_NAME,1);
			WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),_T("员工姓名"),_T(".//BMClient.ini")); 
		}else{
			if(_T("员工姓名") == chioce)
			{
				CheckDlgButton(IDC_RADIO_NAME,1);
			}else if(_T("员工工号") == chioce){
				CheckDlgButton(IDC_RADIO_NUM,1);
			}else if(_T("职务") == chioce){
				CheckDlgButton(IDC_RADIO_POS,1);
			}else{
				CheckDlgButton(IDC_RADIO_ADDR,1);
			}
		}
	}else{   //不存在该文件  加载默认配置
		CheckDlgButton(IDC_RADIO_NAME,1);
		WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),_T("员工姓名"),_T(".//BMClient.ini")); 
	}
}

void CEmpInfoSearch::OnBnClickedRadioName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("员工姓名");
	WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}


void CEmpInfoSearch::OnBnClickedRadioNum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("员工工号");
	WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}


void CEmpInfoSearch::OnBnClickedRadioAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("所属地点");
	WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}


void CEmpInfoSearch::OnBnClickedRadioPos()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = _T("职务");
	WritePrivateProfileString(_T("员工搜索"),_T("默认条件"),temp,_T(".//BMClient.ini")); 
}
