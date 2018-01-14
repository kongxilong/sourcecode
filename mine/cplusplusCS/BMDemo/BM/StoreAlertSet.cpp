// StoreAlertSet.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "StoreAlertSet.h"
#include "afxdialogex.h"


// CStoreAlertSet 对话框

IMPLEMENT_DYNAMIC(CStoreAlertSet, CDialogEx)

CStoreAlertSet::CStoreAlertSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStoreAlertSet::IDD, pParent)
	, m_goods_name(_T(""))
	, m_goods_alert_num(0)
{

}

CStoreAlertSet::~CStoreAlertSet()
{
}

void CStoreAlertSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Text(pDX, IDC_EDIT_GOODS_NAME, m_goods_name);
	DDX_Text(pDX, IDC_EDIT_GOOS_ALERT_NUM, m_goods_alert_num);
	DDV_MinMaxInt(pDX, m_goods_alert_num, 0, 20000);
}


BEGIN_MESSAGE_MAP(CStoreAlertSet, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_STARTTASK_StoreAlertSet, &CStoreAlertSet::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_StoreAlertSet, &CStoreAlertSet::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CStoreAlertSet::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CStoreAlertSet::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CStoreAlertSet 消息处理程序


void CStoreAlertSet::OnTimer(UINT_PTR nIDEvent)
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

void CStoreAlertSet::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void CStoreAlertSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pStoreAlertSet = NULL;
	CDialogEx::OnClose();
}


HBRUSH CStoreAlertSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);	
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

BOOL CStoreAlertSet::OnInitDialog()
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

afx_msg LRESULT CStoreAlertSet::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CStoreAlertSet::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void CStoreAlertSet::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CStoreAlertSet::OnBnClickedBtnSave()
{
	UpdateData(TRUE);
	USES_CONVERSION;
	char *p = T2A(m_goods_name);
	memset(&m_goods_alert_num_set_strutct,0,sizeof(m_goods_alert_num_set_strutct));
	strcpy_s(m_goods_alert_num_set_strutct.Store_Name,p);
	CString alert_num = NULL;
	alert_num.Format(_T("%d"),m_goods_alert_num);
	p = T2A(alert_num);
	strcpy_s(m_goods_alert_num_set_strutct.Store_Alert_NUM,p);
	CSock::StartReqGoodsAlertNumSet(this);
}
