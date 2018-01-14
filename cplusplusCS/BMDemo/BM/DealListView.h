#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"



// CDealListView 窗体视图

class CDealListView : public CFormView
{
	DECLARE_DYNCREATE(CDealListView)

public:
	CDealListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDealListView();

public:
	enum { IDD = IDD_FORMVIEW_DEAL_LIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox			m_timeChoose;
	CDateTimeCtrl		m_dateEnd;
	CDateTimeCtrl		m_dateStart;
	//CString m_keyword;
	
	CListCtrl			m_list;
	CSplitButton		m_split;
	CString				m_strToDLFile;    //下载附件的字符串
	CString				m_strFilePath;    //下载文件保存路径
	CString             m_DLFileTempName;      //下载文件的时间
//	CListCtrl m_list;
public:
	virtual		void	OnInitialUpdate();
	void				ShowTip();
	void				ShowDLStatus();
	void				ResetDLStatus();

	void				InitList();
	void				InitSplit();
	void				InitComb();
	void				InitTipCtrl();
	void                InitSearchRadio();
	afx_msg		void	OnBnClickedBtnEdit();
	
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnDownLoadFile();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	u_long				GetFileSize();    //获得文件的大小
	//int                 RecvFileData(SOCKET s, unsigned short ID,void* ptr);
	void				ValidFileDL();    //校验下载文件的有效性
	//void                SaveFile(u_short nIndexThread, const char* pBuf, u_long ulLen, u_long ulPos,void* ptr);
	BOOL                CreateSocket(SOCKET &sock);  //为下载线程创建套接字
	//void                AssemblyFile(void);         //合并下载好的文件
	//u_long              AssembleLine(FILE* destFile, int startPos, FILE* resoFile);
	void                NameTheDLFile(CString &temp_name);    //为将要下载的文件一个临时文件名
protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	CString         m_search_Keyword;    //搜索关键字
	CStatic			m_tip;
	int             m_time_take_dlFile;    //下载文件所用的时间
	int				m_time_take;
	CStatic			m_dlStatus;
	CProgressCtrl	m_progressCtrl;
	BOOL            m_bDLoading;     //当前正在下载标志
	u_long			m_ulCurTotal;   //当前下载字节数
	u_long			m_ulFileSize;    //文件总得大小
	CString         m_ulFilePath;    //文件路径 
	FileInfo		m_fileDlInfo;    //下载文件的信息
	CString         m_keywords;
	CString         m_strToDelDeal;    //向服务器端发送删除订单的字符串
protected:
	afx_msg			LRESULT		OnResetDLStatus(WPARAM wParam, LPARAM lParam);
	afx_msg			LRESULT		OnValidFile(WPARAM wParam, LPARAM lParam);
public:
	afx_msg			void		OnBnClickedBtnSearchViaDate();
	afx_msg			void		OnBnClickedBtnSearchAllDeal();
	afx_msg			void		OnBnClickedBtnSearchKeywords();
	afx_msg			void		OnCbnSelchangeComboTimechoose();
	afx_msg			void		OnBnClickedRadioChargeMan();
	afx_msg			void		OnBnClickedRadioNameCtm();
protected:
	afx_msg			LRESULT		OnStratDLoad(WPARAM wParam, LPARAM lParam);
	afx_msg			LRESULT		OnEndDLoad(WPARAM wParam, LPARAM lParam);
public:
	
	afx_msg			void		OnDelFile();
	afx_msg			void		OnReverseChoose();
	afx_msg			void		OnChooseNone();
	afx_msg			void		OnChooseAll();
	afx_msg			void		OnDelDeal();
protected:
	afx_msg			LRESULT		OnDelDealOrFile(WPARAM wParam, LPARAM lParam);
public:
	
};


