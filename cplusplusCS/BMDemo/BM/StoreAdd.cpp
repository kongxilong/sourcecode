// StoreAdd.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "StoreAdd.h"
#include "afxdialogex.h"


// CStoreAdd 对话框

IMPLEMENT_DYNAMIC(CStoreAdd, CDialogEx)

CStoreAdd::CStoreAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStoreAdd::IDD, pParent)
	, m_name_store(_T(""))
	, m_num_add(_T(""))
	, m_num_alert(_T(""))
{

}

CStoreAdd::~CStoreAdd()
{
}

void CStoreAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IDENTIFY, m_name_store);
	DDX_Text(pDX, IDC_EDIT_NUMADD, m_num_add);
	DDX_Text(pDX, IDC_EDIT_NUMALERT, m_num_alert);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CStoreAdd, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CStoreAdd::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_STOREADD, &CStoreAdd::OnBnClickedBtnStoreadd)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_StoreAdd, &CStoreAdd::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_StoreAdd, &CStoreAdd::OnEndTask)
END_MESSAGE_MAP()


// CStoreAdd 消息处理程序


HBRUSH CStoreAdd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CStoreAdd::OnBnClickedBtnCancle()
{
	CDialogEx::OnCancel();
}


void CStoreAdd::OnBnClickedBtnStoreadd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_name_store.IsEmpty())
	{
		MessageBox(_T("库存物品名称不能为空"));
		return ;
	}
	if(m_name_store.GetLength()>25)
	{
		MessageBox(_T("库存物品名称长度不能大于25个字符"));
		return ;
	}
	if(m_num_add.IsEmpty())
	{
		MessageBox(_T("库存物品变动数量不能为空"));
		return ;
	}
	if(m_num_add.GetLength()>18)
	{
		MessageBox(_T("库存物品变动数量长度不能超过18个字符"));
		return ;
	}
	USES_CONVERSION;
	char *p = T2A(m_num_add);
	int len = m_num_add.GetLength();
	for(int i = 0;i < len;i++)
	{
		if(p[i] <'0'||p[i]>'9')
		{
			MessageBox(_T("库存物品变动数量必须为数字"));
			return;
		}
	}
	if(m_num_alert.IsEmpty())
	{
		if(IDYES == MessageBox(_T("库存物品预警数量为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_num_alert = _T("0");
		}else{
			return ;
		}
	}else if(m_num_alert.GetLength()>18)
	{
		MessageBox(_T("库存物品预警数量长度不能超过18个字符"));
		return ;
	}else{
		p = T2A(m_num_alert);
		int len = m_num_alert.GetLength();
		for(int i = 0;i < len;i++)
		{
			if(p[i] <'0'||p[i]>'9')
			{
				MessageBox(_T("库存物品预警数量必须为数字"));
				return;
			}
		}
	}
	p = T2A(m_name_store);
	strcpy_s(m_storeAddStruct.Store_Name,p);
	p = T2A(m_num_add);
	strcpy_s(m_storeAddStruct.Store_Num,p);
	p = T2A(m_num_alert);
	strcpy_s(m_storeAddStruct.Store_Alert_NUM,p);

	CSock::StartReqStoreAdd(this);
}


void CStoreAdd::OnTimer(UINT_PTR nIDEvent)
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

void CStoreAdd::ShowTip()
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

BOOL CStoreAdd::OnInitDialog()
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

void CStoreAdd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pStoreAdd = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CStoreAdd::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CStoreAdd::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}
