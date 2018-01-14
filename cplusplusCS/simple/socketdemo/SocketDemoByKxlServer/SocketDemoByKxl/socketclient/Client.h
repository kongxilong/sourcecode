
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommucationProtocol.h"

class CSocketDemoByKxlDlg;
class CClient  
{
public:
	CClient(SOCKET s, CSocketDemoByKxlDlg* pServer);   //有参构造函数
	virtual ~CClient();

protected:
	CClient();
	//CString m_user;                                 //用于记录用户的操作
	///////****用于安全的存储传输的数据 而不能只使用一个指针 那样限制了程序的灵活性****/////////
public:
 
	//////***用于安全的存储传输的数据 而不能只使用一个指针 这样限制了程序的灵活性******/////////
public:
	inline SOCKET	GetSocket(void)							//获取套接字
	{
		return m_sClient;	
	}

	CSocketDemoByKxlDlg* GetServer(void)						//获取CServerDlg类指针
	{
		return m_pServer;
	}
    

public:

	BOOL	SendData(void);							//发送数据
	BOOL	RecvData(void);							//接收数据
	BOOL    RecvTextMessage(int len);               //接收用户发送的文本信息
	BOOL    RecvReplyTextMessageFromClient(int len);
    BOOL    SendTextMessage(int type);                      //给用户发送消息
	///////******以下为一些其他的函数 跟发送和接收文件无关****************//////////
public:	
	BOOL    RecvDataFromClient(SOCKET s,char * p_recv,int len,CString wrong_message,CString success_message);
	BOOL    SendSocketMessageToClient(SOCKET s, char* p_send,int data_len,CString _err_message);

	void    GetCurTime(CString& time);
	void    GetIPBySocket(SOCKET socket,CString& IP);
	void	WriteSysLog(CString message);  //向服务器写日志
    
    void    SetReqType(int reqType)
	{
		m_nReqCur = reqType;
	}

	void    SetSendMessageBuff(const CString& newMessage)
	{
		m_sSend = newMessage;
	}

private:
	SOCKET                 m_sClient;         //客户端连接套接字
	CSocketDemoByKxlDlg*   m_pServer;         //服务器主窗口指针
	int	                   m_nReqCur;		  //当前请求类型

	CString                m_sRecv;          //接收缓冲
	CString                m_sSend;          //发送缓冲
};

