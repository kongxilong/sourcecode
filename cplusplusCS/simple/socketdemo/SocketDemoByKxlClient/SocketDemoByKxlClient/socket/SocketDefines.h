

#pragma  once

const int g_iBufTimes = 10;    //设定socket缓冲区为默认的10倍
const int g_iPort     = 6686;      //设置默认端口号

typedef struct _hdr    //通信包头
{
	int	      iReqType;	     //请求类型		
	u_short	  usTotalLen;	//数据包长度(头+包体)

}hdr, *phdr;

#define		HEADLEN					sizeof(hdr)     //数据包结构体的大小


/******************包头的请求和回执类型开始*******************/
//通信协议
enum   enumPacketType
{
	ENUM_PACKET_SEND_TEXT_MESSAGE = 1,      //文本信息
    ENUM_PACKET_RECV_TEXT_MESSAGE = 2,     //接收到的回执文本信息
	ENUM_PACKET_RECV_MESSAGE_FROM_SERVER,    //接收从服务器发过来的信息
	ENUM_PACKER_SEND_RELPY_MESSAGE_TO_SERVER,   //收到服务器发过来的消息后给服务器回应
};

/******************包头的请求和回执类型结束*******************/



/******************请求类型开始*******************/
enum   enumREQTYPE
{			
	REQ_NONE = 0,                   //当前无请求
	REQ_TEXT_MESSAGE,				//请求登录
	REQ_REPLY_MESSAGE_FROM_SERVER,    //回复服务器收到信息了
};


/****************请求类型结束*****************/

typedef struct _Package
{
	char   buff[1024];   //用来接收和发送数据的缓存区
}Package;


