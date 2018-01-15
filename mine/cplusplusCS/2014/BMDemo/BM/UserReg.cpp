// UserReg.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "UserReg.h"
#include "afxdialogex.h"


// CUserReg 对话框

IMPLEMENT_DYNAMIC(CUserReg, CDialogEx)

CUserReg::CUserReg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserReg::IDD, pParent)
{

}

CUserReg::~CUserReg()
{
}

void CUserReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserReg, CDialogEx)
END_MESSAGE_MAP()


// CUserReg 消息处理程序
