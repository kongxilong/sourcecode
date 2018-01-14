// VariableConstant.h: interface for the VariableConstant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLECONSTANT_H__1E7A0731_F1D5_4524_9BF2_8A0750D9147F__INCLUDED_)
#define AFX_VARIABLECONSTANT_H__1E7A0731_F1D5_4524_9BF2_8A0750D9147F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class VariableConstant  
{
public:
	VariableConstant();
	virtual ~VariableConstant();
	 int static heart_request;//心跳请求
	 int static hole_request;//打洞请求
	 int static update_fragment;//更新分片信息
	 int static fragment_transmit;//分片传输请求
	 int static search_fragment;//查询分片信息
	 int static SERVER_LISTEN_PORT; //服务器端监听端口
	 int static MAX_ACCAPT;//最多接收的客户端口的数目
     int static BUFFER_SIZE;
	 int static SUCCESS_CODE;//执行成功返回参数
	 int static ERROR_CODE;//执行失败返回参数
	 int static search_detail_fragment;//查询某个分片的位置
	 long static fragment_longbytes; //每个分段的长度
	 CString static server_address;
	 CString static xml_file_path;
};

#endif // !defined(AFX_VARIABLECONSTANT_H__1E7A0731_F1D5_4524_9BF2_8A0750D9147F__INCLUDED_)
