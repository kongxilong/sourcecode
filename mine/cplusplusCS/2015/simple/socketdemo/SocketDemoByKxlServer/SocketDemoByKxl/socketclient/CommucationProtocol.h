

//用于定义通信协议

#pragma  once 


const int    g_iServerPort	= 6686;            //通信端口
const int    g_iBufSize   	= (1024 * 60);		//缓冲区的大小

//设置系统的发送和接收数据缓冲区为默认的10倍，在xp32下为 8k*10
const int    g_iBufTimes = 10;

enum REQTYPE
{
	REQ_NONE = 0,           //当前并无请求
	REQ_MESSAGE = 1,	    //普通消息
	REQ_SEND_TEXT_MESSAGE_TO_CLINET,    //发送消息给客户端
};


typedef struct _hdr    //通信包头
{
	int	      iReqType;	     //请求类型		
	u_short	  usTotalLen;	//数据包长度(头+包体)

}hdr, *phdr;

const int   g_iHeadLen = (sizeof(hdr));    //包头的长度


typedef struct _SysLogInfo
{
	char showInfo[1024];
	void* p_voidTemp;
}SysLogInfo,*p_SysLogInfo;



//数据包类型
////////******以下为接收到的数据包请求类型******////////////
enum  enumPacketType
{
    ENUM_RECV_PACKET_TEXT_MESSAGE = 1,      //接收文本请求信息  
    ENUM_REPLY_PACKET_TEXT_MESSAGE = 2,     //回执的文本信息
	ENUM_SEND_MESSAGE_TO_CLIENT = 3,       //给客户端发消息
	ENUM_RECV_REPLY_FROM_CLIENT = 4,       //接收到客户端反馈
};
  

typedef struct _Package
{
	char   buff[1024];   //用来接收和发送数据的缓存区
}Package;





