// StoreInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "StoreInfo.h"
#include "afxdialogex.h"
#include "StoreAdd.h"
#include "StoreOut.h"
#include "StoreAlertSet.h"

// CStoreInfo 对话框

IMPLEMENT_DYNAMIC(CStoreInfo, CDialogEx)

CStoreInfo::CStoreInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStoreInfo::IDD, pParent)
{

}

CStoreInfo::~CStoreInfo()
{
}

void CStoreInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STOREINFO, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CStoreInfo, CDialogEx)
	//ON_COMMAND(ID_BUTTON_STOCKINFO, &CStoreInfo::OnButtonStockInfo)
	ON_BN_CLICKED(IDC_BTN_STOREADD, &CStoreInfo::OnBnClickedBtnStoreAdd)
	ON_BN_CLICKED(IDC_BTN_STOREOUT, &CStoreInfo::OnBnClickedBtnStoreOut)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_StoreInfo, &CStoreInfo::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_StoreInfo, &CStoreInfo::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CStoreInfo::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ALERT_SET, &CStoreInfo::OnBnClickedBtnAlertSet)
END_MESSAGE_MAP()


// CStoreInfo 消息处理程序


void CStoreInfo::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	//m_list.InsertColumn(0, _T("编号"), LVCFMT_LEFT, rect.Width()/4); 
	m_list.InsertColumn(0, _T("名称"), LVCFMT_LEFT, rect.Width()/3);    
    m_list.InsertColumn(1, _T("库存数量"), LVCFMT_LEFT, rect.Width()/3);
	m_list.InsertColumn(2, _T("预警数量"), LVCFMT_LEFT, rect.Width()/3);
    

	//m_list.InsertItem(0, _T("文件一"));    
    //m_list.SetItemText(0, 1, _T("0"));    
   // m_list.SetItemText(0, 2, _T("0"));      
}


BOOL CStoreInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();

	InitList();
	return TRUE;
}


void CStoreInfo::OnBnClickedBtnStoreAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CStoreAdd dlg;
	dlg.DoModal();
}


void CStoreInfo::OnBnClickedBtnStoreOut()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	  name_goods = NULL;   //将要
	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，库存物品名字为：'%s'。确认继续吗"),name_goods);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			name_goods = m_list.GetItemText(i,0);
		}
	}
	if(name_goods.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个库存物品进行编辑"));
		return;
	}
	CStoreOut dlg;
	dlg.m_name_goods = name_goods;
	dlg.DoModal();
}


HBRUSH CStoreInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CStoreInfo::OnTimer(UINT_PTR nIDEvent)
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


void CStoreInfo::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pStoreInfo = NULL;
	CDialogEx::OnClose();
}



void CStoreInfo::ShowTip()
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

afx_msg LRESULT CStoreInfo::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CStoreInfo::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void CStoreInfo::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartFindAllStore(this);
}


void CStoreInfo::OnBnClickedBtnAlertSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	  name_goods = NULL;   //将要
	CString   num_alert = NULL;
	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将设置第一个选中项，库存物品名字为：'%s'。确认继续吗"),name_goods);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			name_goods = m_list.GetItemText(i,0);
			num_alert = m_list.GetItemText(i,2);
		}
	}
	if(name_goods.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个库存物品进行编辑"));
		return;
	}
	CStoreAlertSet dlg;
	dlg.m_goods_name = name_goods;
	USES_CONVERSION;
	char *p = T2A(num_alert);
	dlg.m_goods_alert_num = atoi(p);
	dlg.DoModal();
}
