// FactoryEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "FactoryEdit.h"
#include "afxdialogex.h"


// CFactoryEdit 对话框

IMPLEMENT_DYNAMIC(CFactoryEdit, CDialogEx)

CFactoryEdit::CFactoryEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFactoryEdit::IDD, pParent)
{

}

CFactoryEdit::CFactoryEdit(CFctSet* pFactorySet,CWnd* pParent)
	: CDialogEx(CFactoryEdit::IDD, pParent)
	, m_addr_fctEdit(_T(""))
	, m_contact_person_fctEdit(_T(""))
	, m_main_product_fctEdit(_T(""))
	, m_name_fctEdit(_T(""))
	, m_phone_fctEdit(_T(""))
	, m_ps_fctEdit(_T(""))
{
	m_pFactorySet = pFactorySet;
}


CFactoryEdit::~CFactoryEdit()
{
	m_pFactorySet = NULL;
}

void CFactoryEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_ADDR_FCT, m_addr_fct);
	//  DDX_Text(pDX, IDC_EDIT_CONTACT_PERSON_FCT, m_contact_person_fct);
	//  DDX_Text(pDX, IDC_EDIT_MAIN_PRODUCT_FCT, m_main_product);
	DDX_Text(pDX, IDC_EDIT_MAIN_PRODUCT_FCT, m_main_product_fctEdit);
	DDX_Text(pDX, IDC_EDIT_NAME_FCT, m_name_fctEdit);
	DDX_Text(pDX, IDC_EDIT_PHONE_FCT, m_phone_fctEdit);
	DDX_Text(pDX, IDC_EDIT_PS_FCT, m_ps_fctEdit);
	DDX_Text(pDX, IDC_EDIT_ADDR_FCT, m_addr_fctEdit);
	DDX_Text(pDX, IDC_EDIT_CONTACT_PERSON_FCT, m_contact_person_fctEdit);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CFactoryEdit, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_FctEdit, &CFactoryEdit::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_FctEdit, &CFactoryEdit::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CFactoryEdit::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CFactoryEdit::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CFactoryEdit 消息处理程序


HBRUSH CFactoryEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CFactoryEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			ShowTip();
			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CFactoryEdit::ShowTip()
{
	m_time_take++;
	CString tip;
	tip.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(tip);
	if(m_time_take == 60)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请检查与服务器的链接状态或重试"));
	}
}

void CFactoryEdit::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pFactoryEdit = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CFactoryEdit::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CFactoryEdit::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void CFactoryEdit::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CFactoryEdit::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_addr_fctEdit.IsEmpty())
	{
		if(IDYES == MessageBox(_T("厂家地址为空,确认继续吗"),_T("提示"),MB_YESNO)){
			m_addr_fctEdit = _T("空");
		}else
		{
			return;
		}
	}
	if(m_addr_fctEdit.GetLength()>100)
	{
		MessageBox(_T("厂家地址过长，不能超过100个字符"));
		return;
	}
	if(m_contact_person_fctEdit.IsEmpty())
	{
		if(IDYES == MessageBox(_T("厂家联系人为空,确认继续吗"),_T("提示"),MB_YESNO)){
			m_contact_person_fctEdit = _T("空");
		}else
		{
			return;
		}
	}
	if(m_contact_person_fctEdit.GetLength()>25)
	{
		MessageBox(_T("厂家联系人不能超过25个字符"));
		return;
	}
	if(m_main_product_fctEdit.IsEmpty())
	{
		if(IDYES == MessageBox(_T("主要产品为空,确认继续吗"),_T("提示"),MB_YESNO)){
			m_main_product_fctEdit = _T("空");
		}else
		{
			return;
		}
	}
	if(m_main_product_fctEdit.GetLength()>100)
	{
		MessageBox(_T("厂家联系人不能超过100个字符"));
		return;
	}
	if(m_ps_fctEdit.IsEmpty())
	{
		if(IDYES == MessageBox(_T("厂家备注为空,确认继续吗"),_T("提示"),MB_YESNO)){
			m_ps_fctEdit = _T("空");
		}else
		{
			return;
		}
	}
	if(m_ps_fctEdit.GetLength()>200)
	{
		MessageBox(_T("厂家备注过长，不能超过200个字符"));
		return;
	}
	if(m_phone_fctEdit.IsEmpty())
	{
		if(IDYES == MessageBox(_T("联系电话为空,确认继续吗"),_T("提示"),MB_YESNO)){
			m_phone_fctEdit = _T("无");
		}else
		{
			return;
		}
	}else{
		if(m_phone_fctEdit.GetLength() != 11){
			MessageBox(_T("联系电话位数不对"));
			m_phone_fctEdit = _T("无");
			return;
		}else{
			USES_CONVERSION;
			char *p = T2A(m_phone_fctEdit);
			for(int i = 0;i<11;i++)
			{
				if(p[i] <'0'||p[i]>'9')
				{
					MessageBox(_T("联系电话不能包含除数字以外的其他字符"));
					m_phone_fctEdit = _T("无");
					return;
				}
			}
		}
	}
	CSock::StartReqFctEdit(this);
}


BOOL CFactoryEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
	return TRUE;
}