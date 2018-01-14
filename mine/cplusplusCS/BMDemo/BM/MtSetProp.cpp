// MtSetProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "MtSetProp.h"
#include "afxdialogex.h"
#include "MtNoteNew.h"
#include "MtNoteEdit.h"
#include "MtAndFctSheet.h"

// CMtSetProp 对话框

IMPLEMENT_DYNAMIC(CMtSetProp, CPropertyPage)

CMtSetProp::CMtSetProp()
	: CPropertyPage(CMtSetProp::IDD)
{
	m_strMtInfo = _T("");
	timeTake = 0;
	m_hItemChecked = NULL;
}

CMtSetProp::~CMtSetProp()
{
}

void CMtSetProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MATERIAL, m_treeMaterial);
	//DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	//DDX_Control(pDX, IDC_ET_STATUS, m_status);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	DDX_Control(pDX, IDC_STATIC_TIME_TAKE, m_timeTakeStatus);
}


BEGIN_MESSAGE_MAP(CMtSetProp, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_NEW, &CMtSetProp::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_MTEDIT, &CMtSetProp::OnBnClickedBtnMtEdit)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CMtSetProp::OnBnClickedBtnRefresh)
	ON_MESSAGE(WM_PAITMTTREE, &CMtSetProp::OnPaitMtTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_MATERIAL, &CMtSetProp::OnItemExpandingTreeMt)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_UPDATESTATUS_MtSetProp, &CMtSetProp::OnUpdateStatus)
	ON_MESSAGE(WM_REQNOTENEW_MtSetProp, &CMtSetProp::OnReqNoteNew)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STRATREQMT_MtSetProp, &CMtSetProp::OnStratReqMt)
	ON_MESSAGE(WM_ENDREQMT_MtSetProp, &CMtSetProp::OnEndReqMt)
	ON_MESSAGE(WM_CLEARMTTREE_MtSetProp, &CMtSetProp::OnClearMtTree)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MATERIAL, &CMtSetProp::OnTvnSelchangedTreeMaterial)
	//ON_NOTIFY(NM_CLICK, IDC_TREE_MATERIAL, &CMtSetProp::OnNMClickTreeMaterial)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_MATERIAL, &CMtSetProp::OnNMDblclkTreeMaterial)
	ON_MESSAGE(WM_REQNODEEDIT_MtSetProp, &CMtSetProp::OnReqNodeEdit)
	ON_BN_CLICKED(IDC_BTN_MTDELETE, &CMtSetProp::OnBnClickedBtnMtDelete)
END_MESSAGE_MAP()


// CMtSetProp 消息处理程序

BOOL CMtSetProp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CFont font;
	LOGFONT m_tempfont={20,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_status.SetFont(&font);
	m_timeTakeStatus.SetFont(&font);
	font.Detach();
	
	InitTree();
	return TRUE;
}

void CMtSetProp::InitTree()
{
	m_treeMaterial.SetBkColor(CONCTRLBKCOLOR);
	m_treeMaterial.InsertItem(_T("材料树"),TVI_ROOT,TVI_LAST);
	
	m_hItemCur = m_treeMaterial.GetRootItem();
	m_treeMaterial.SetItemState(m_hItemCur,INDEXTOSTATEIMAGEMASK(0),   TVIS_STATEIMAGEMASK);
	void* dlg = this;
	CSock::StartRefreshMtTree(dlg);
}


void CMtSetProp::OnBnClickedBtnNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CMtNoteNew mtNoteNewDlg(this);
	//mtNoteNewDlg.m_persion
	mtNoteNewDlg.DoModal();
}


void CMtSetProp::OnBnClickedBtnMtEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	if(NULL == m_hItemChecked){
		m_status.SetWindowText(_T("请先双击选中一个节点，选择一个有效的节点，根节点除外！"));
		return;
	}
	CString curSelNode = NULL;
	curSelNode = m_treeMaterial.GetItemText(m_hItemChecked); 
	if(curSelNode == "材料树" || curSelNode.IsEmpty())
	{
		m_status.SetWindowText(_T("请先选择一个有效的节点，根节点除外！"));
		return;
	}
	CMtNoteEdit mtSetEditDlg(this);
	mtSetEditDlg.m_nodeOrgName = curSelNode;
	mtSetEditDlg.DoModal();
}


void CMtSetProp::OnBnClickedBtnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_status.SetWindowText(_T("正在向服务器请求初始化材料树！"));
	m_hItemCur = m_treeMaterial.GetRootItem();
	void* dlg = this;
	CSock::StartRefreshMtTree(dlg);
}


afx_msg LRESULT CMtSetProp::OnPaitMtTree(WPARAM wParam, LPARAM lParam)
{	
	m_strArr.RemoveAll();
	TransferSecondNode();
	PaintTree(m_hItemCur);		//填充树视
	m_status.SetWindowText(_T("加载完毕"));
	m_treeMaterial.EnableWindow(TRUE);  //使树视图有效
	return 0;
}

void CMtSetProp::PaintTree(HTREEITEM hItem)		//填充树视
{
	int size = m_strArr.GetSize();
	for(int i = 0; i < size; i++)
	{
		CString strSubDir = m_strArr.GetAt(i);      //从二级目录数组中取出一个目录(包含了若干三级目录)
		CString strFind = _T("\\");					//二级目录与三级目录的分隔符
		int rootPos = strSubDir.Find(strFind);	//找到分隔符位置
		if (-1 != rootPos)  //如果找到了分隔符
		{
			CString strSecondNode = strSubDir.Mid(0,rootPos);
			HTREEITEM hChild = m_treeMaterial.InsertItem(strSecondNode,hItem,TVI_LAST);
			strSubDir = strSubDir.Mid(rootPos+1);
			while(TRUE)  //读取三级目录 填充树视图
			{
				int index = strSubDir.Find(_T("|"));
				if(-1 == index)
				{
					break;
				}else{
					CString strThirdNode = strSubDir.Mid(0,index);
					m_treeMaterial.InsertItem(strThirdNode,hChild,TVI_LAST);
					strThirdNode.Empty();
					strSubDir = strSubDir.Mid(index+1);
					continue;
				}
			}
		}
	}
	m_treeMaterial.Expand(hItem,TVE_EXPAND);	
}

void CMtSetProp::TransferSecondNode()
{
	m_strArr.RemoveAll();
	CString _strTreeInfo = m_strMtInfo;
	CString temp;
	while(TRUE)
	{
		CString strFind = _T("<");
		int _strBegin = _strTreeInfo.Find(strFind);
		if(_strBegin == -1)  //没找到
		{
			break;
		}else{   //找到了字符串开始的位置 
			 int _strEnd = _strTreeInfo.Find(_T(">"));
			 if(_strEnd == -1)  //没找到
			 {
				 break;
			 }else{ //找到了字符串结束的位置  开始截取
				 temp = _strTreeInfo.Mid(_strBegin+1,(_strEnd -_strBegin-1));
				 m_strArr.Add(temp);
				 temp.Empty();
				 _strTreeInfo = _strTreeInfo.Mid(_strEnd+1);
				 continue;
			 }
		}
	}
}

void CMtSetProp::OnItemExpandingTreeMt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;		//选中的树项句柄

	if (m_hItemCur == hTreeItem)							//展开项为当前项
	{
		return;												//折叠与展开之间切换
	}else
	{
		m_hItemCur = hTreeItem;								//修改当前树项为展开项
		m_treeMaterial.SelectItem(m_hItemCur);					//设置该项为选中状态
	}

	HTREEITEM hChild = m_treeMaterial.GetChildItem(hTreeItem);	//第一个第二级目录;
	BOOL bHasThirdDir = FALSE;								//初始值设置为没有三级目录
	while (hChild != NULL)
	{
		 bHasThirdDir = m_treeMaterial.ItemHasChildren(hChild);	//三级节点
		if (TRUE == bHasThirdDir)							//存在三级节点
		{
			break;
		}		
 		hChild = m_treeMaterial.GetNextSiblingItem(hChild);		//下一个二级目录
	}
	
	//存在第三级节点
	if (TRUE == bHasThirdDir)
	{
		return;//折叠与展开之间切换
	}


	//不存在第三级节点
	m_treeMaterial.EnableWindow(FALSE);				//在请求子节点期间禁止其他操作
	m_nodeName = m_treeMaterial.GetItemText(hTreeItem);
	CSock::StartExpandMtTree(this);
	
	//删除第二级节点
	hChild = m_treeMaterial.GetChildItem(hTreeItem);	//第一个二级节点					
	while (hChild != NULL)
	{
		HTREEITEM hTreeItemTemp	= m_treeMaterial.GetNextSiblingItem(hChild);//下一个二级节点
		m_treeMaterial.DeleteItem(hChild);
		hChild = hTreeItemTemp;
	}
	*pResult = 0;
}


HBRUSH CMtSetProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);
		pDC->SetTextColor(RGB(21,21,21));
		return b_static;
		
	}
	
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(BACKGROUNDCOLOR); 
		HBRUSH b=CreateSolidBrush(BACKGROUNDCOLOR); 
		return b;
	}
	return hbr;
}


afx_msg LRESULT CMtSetProp::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	CString status = (LPCTSTR)lParam;
	m_status.SetWindowText(status);
	return 0;
}


afx_msg LRESULT CMtSetProp::OnReqNoteNew(WPARAM wParam, LPARAM lParam)
{
	CSock::StartReqMtNodeNew(this);
	return 0;
}




void CMtSetProp::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPropertyPage::OnClose();
}

void CMtSetProp::EnableParentDlgShutDownBtn(BOOL nEnable)   //禁用属性页关闭窗口按钮
{
	CMtAndFctSheet * p_parentDlg = (CMtAndFctSheet *)GetParent();
	//TRUE表示可用 FALSE表示不可用   
	p_parentDlg->SetCloseBtnEable(nEnable);

}



void CMtSetProp::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			m_status.SetWindowText(_T("请求超时，请检查与服务器连接状态"));
			OnEndReqMt(0,0);
			CSock::m_pNotePublic = NULL;  //清除指针
			break;
		}
	case 2:
		{
			ShowTimeTakeStatus();
			break;
		}
	default:
		break;
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CMtSetProp::ShowTimeTakeStatus()
{
	//m_timeTakeStatus.ShowWindow(SW_SHOW);
	timeTake++;
	CString time;
	time.Format(_T("用时：%d秒"),timeTake);
	m_timeTakeStatus.SetWindowText(time);
}

afx_msg LRESULT CMtSetProp::OnStratReqMt(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(FALSE);
	//SetTimer(1,20*1000,NULL);
	//SetTimer(2,1000,NULL);
	timeTake = 0;
	m_timeTakeStatus.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CMtSetProp::OnEndReqMt(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(TRUE);
	//KillTimer(1);
	//KillTimer(2);
	m_timeTakeStatus.SetWindowText(_T(""));
	timeTake = 0;
	return 0;
}


afx_msg LRESULT CMtSetProp::OnClearMtTree(WPARAM wParam, LPARAM lParam)
{
	//删除第二级节点
	HTREEITEM hChild = m_treeMaterial.GetChildItem(m_hItemCur);	//第一个二级节点					
	while (hChild != NULL)
	{
		HTREEITEM hTreeItemTemp	= m_treeMaterial.GetNextSiblingItem(hChild);//下一个二级节点
		m_treeMaterial.DeleteItem(hChild);
		hChild = hTreeItemTemp;
	}
	return 0;
}

/*
void CMtSetProp::OnTvnSelchangedTreeMaterial(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
		*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hitemTemp = m_treeMaterial.GetSelectedItem();
	BOOL b= m_treeMaterial.GetCheck(hitemTemp);
	m_treeMaterial.SetCheck(hitemTemp,!b);
	AfxMessageBox(m_treeMaterial.GetItemText(hitemTemp));

}
*/
/*
void CMtSetProp::OnNMClickTreeMaterial(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	
	CPoint point;
    UINT uFlag;   //接收有关点击测试的信息的整数
    HTREEITEM hTree;
    BOOL bCheck;
    GetCursorPos(&point);    //获取屏幕鼠标坐标
	m_treeMaterial.ScreenToClient(&point);           //转化成客户坐标
	//point.Offset(-150,50);
	hTree = m_treeMaterial.HitTest(point,&uFlag);    //返回与CtreeCtrl关联的光标的当前位置和句柄
	if((NULL!=hTree))
	{
		CString str = m_treeMaterial.GetItemText(hTree);
		 AfxMessageBox(str);
	}
    if ((NULL!=hTree) && (TVHT_ONITEMSTATEICON == uFlag))  //点中复选框
    {
          bCheck = m_treeMaterial.GetCheck(hTree);      //获取当前复选状态
		  m_hItemChecked = hTree;
		  CString str = m_treeMaterial.GetItemText(m_hItemChecked);
		  AfxMessageBox(str);
		  //SetChildCheck(hTree,!bCheck);                  //设置子项复选状态
     }
	 
}
*/

void CMtSetProp::OnNMDblclkTreeMaterial(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hitemTemp = m_treeMaterial.GetSelectedItem();
	BOOL b= m_treeMaterial.GetCheck(hitemTemp);
	m_treeMaterial.SetCheck(hitemTemp,!b);
	m_hItemChecked = hitemTemp;
	//AfxMessageBox(m_treeMaterial.GetItemText(hitemTemp));
}


afx_msg LRESULT CMtSetProp::OnReqNodeEdit(WPARAM wParam, LPARAM lParam)
{
	CSock::StartReqMtNodeEdit(this);
	return 0;
}


void CMtSetProp::OnBnClickedBtnMtDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if(NULL == m_hItemChecked){
		m_status.SetWindowText(_T("请先双击选中一个节点，选择一个有效的节点，根节点除外！"));
		return;
	}
	CString curSelNode = NULL;
	curSelNode = m_treeMaterial.GetItemText(m_hItemChecked); 
	if(curSelNode == "材料树" || curSelNode.IsEmpty())
	{
		m_status.SetWindowText(_T("请先选择一个有效的节点，根节点除外！"));
		return;
	}
	m_nodeName = curSelNode;
	CString _strTip;
	_strTip.Format(_T("您选择删除的节点是：%s"),m_nodeName);
	if(IDYES == MessageBox(_strTip,_T("提示"),MB_YESNO))  //如果确定要删除
	{
		CSock::StartReqDeleteNode(this);
	}else{
		return;
	}
}
