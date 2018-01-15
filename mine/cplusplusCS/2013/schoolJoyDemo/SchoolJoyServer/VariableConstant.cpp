// VariableConstant.cpp: implementation of the VariableConstant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "VariableConstant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VariableConstant::VariableConstant()
{

}

VariableConstant::~VariableConstant()
{

}
int VariableConstant::heart_request = 1;//心跳请求信息
int VariableConstant::hole_request = 2;//打洞请求
int VariableConstant::search_fragment = 3;//查询分片信息
int VariableConstant::update_fragment = 4; //更新分片信息
int VariableConstant::fragment_transmit =5;//分片传输请求
int VariableConstant::SERVER_LISTEN_PORT = 8091;//服务器监听的端口
int VariableConstant::MAX_ACCAPT = 500;//服务器能最大接收的连接数；
int VariableConstant::BUFFER_SIZE =4096;
int VariableConstant::SUCCESS_CODE = 0;//执行成功返回代码
int VariableConstant::ERROR_CODE = 1;//执行失败返回代码
int VariableConstant::search_detail_fragment =6;//查询某个分片的物理位置
long VariableConstant::fragment_longbytes =1048576*60;
CString VariableConstant::xml_file_path="d:\\serverCollege\\";