<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="js/top.js" charset="utf-8"></script>
<title>意见管理</title>
<link href="css/top.css" rel="stylesheet" type="text/css" />
<style>
.head {
	width:100%;
	background:#f7f7f7;
	height:30px;
	font-size:16px;
	font-family:"微软雅黑";
	margin-top: -1px;
	margin-right: auto;
	margin-bottom: 0px;
	margin-left: auto;
}
.all {
	margin: auto;
	height: 1090px;
	width: 1100px;
}
.box1 {
	height: 100px;
	width: 1000px;
	top: 10px;
	margin: auto;
	background-color: #FFF;
}
.box1_1 {
	height: 100px;
	width: 400px;
	background-color: #FFF;
	float: left;
}
.logotext {
	height: 50px;
	width: 250px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
	background-color: #FFF;
	float: left;
	margin-top: 50px;
}
.logo {
	float: left;
	height: 100px;
	width: 100px;
	background-color: #FFF;
	border-right: solid;
	border-color: #CCC;
}
.boxall {
	background-color: #FFF;
	margin: auto;
	height: 950px;
	width: 1100px;
}
.boxall_1 {
	background-color: #FFF;
	margin: auto;
	height: 30px;
	width: 1100px;
	font-size: 20px;
	font-weight: bolder;
	color: #C00;
}
.line {
	margin: auto;
	height: 0px;
	width: 1100px;
	border-top-style: solid;
	border-right-style: none;
	border-bottom-style: none;
	border-left-style: none;
	border-top-color: #004A80;
	background-color: #FFF;
}
.boxall_table {
	background-color: #ECFFFF;
	height: 900px;
	width: 1095px;
	margin: auto;
}
.bottom {
	margin: auto;
	height: 35px;
	width: 200px;
}
</style>
<script type="text/javascript">
function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}
</script>
</head>

<body onload="MM_preloadImages('xiaoyaoimg/top2.png','xiaoyaoimg/shouye2.jpg','xiaoyaoimg/xiangzuo2.jpg','xiaoyaoimg/xiaoyou2.jpg','xiaoyaoimg/weiye2.jpg')">
<div id="nav"><a href="#" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image1','','xiaoyaoimg/top2.png',1)"><img src="xiaoyaoimg/top1.png" name="Image1" width="34" height="34" border="0" id="Image1" /></a></div>
<div class="all">
  <div class="head" ><script language=JavaScript>

var enabled = 0; today = new Date();
var day; var date;
if(today.getDay()==0) day = " 星期日"
if(today.getDay()==1) day = " 星期一"
if(today.getDay()==2) day = " 星期二"
if(today.getDay()==3) day = " 星期三"
if(today.getDay()==4) day = " 星期四"
if(today.getDay()==5) day = " 星期五"
if(today.getDay()==6) day = " 星期六"
document.fgColor = "000000";
date = "今天是：" + (today.getYear()) + "年" + (today.getMonth() + 1 ) + "月" + today.getDate() + "日" + day +"";
document.write(date);

</script> 
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>管理员:<%=session.getAttribute("adminOnline") %></span></blink></a>你好！ <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink></blink></a><a href="admin_exit.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;点击退出&#93;</span></blink></a> <a href="admin_home.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;信息管理主页&#93;</span></blink></a> <a href="admin_userList.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;用户管理&#93;</span></blink></a> <a href="admin_dealList.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;订单管理&#93;</span></blink></a> <a href="admin_routeList.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;路线管理&#93;</span></blink></a> <a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;逍遥游主页&#93;</span></blink></a> </div>
  <br>
  <div class="box1">
    <div class="box1_1">
      <div class="logo"><img src="xiaoyaoimg/logo.png" width="97" height="93" alt="logo" /> </div>
      <div class="logotext"><img src="xiaoyaoimg/information.png" width="174" height="46" alt="信息管理" /></div>
    </div>
  </div>
  <br>
  <br>
  <div class="boxall">
    <div class="boxall_1"><a href="#" style="color:#00F; text-decoration:none; font-size: 24px;"onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">信息管理</a>&gt;意见管理</div>
    <div class="line"></div>
    <br>
    <div class="boxall_table"> 
      <br>
      <td width="494" align="center"><p>总找到<a href="#"><s:property value="page.getTotalCount()"/></a>条信息&gt;<a href="seeSuggestionList.action">刷新</a>  </p></td>
      <br>
    
      <table border="thin" align="center" cellpadding="0" cellspacing="0">
        <tr>
          <td width="68">意见编号</td>
          <td width="82">意见分类</td>
          <td width="435">意见内容</td>
          <td width="82">处理状态</td>
          <td width="110">处理人</td>
          <td width="99">处理</td>
          <td width="99">删除</td>
        </tr>
        <s:iterator value="suggestionList" id="sugt" status="i">
        <tr onMouseOut="this.bgColor='#ECFFFF';this.borderColor='#efefef'"; onMouseOver="this.bgColor='#cccccc'; this.borderColor='#000033'">
          <td height="20"><s:property value="#i.index+1"/></td>
          <td><s:property value="#sugt.suggestType"/></td>
          <td><s:property value="#sugt.suggestContent"/></td>
          <td><s:property value="#sugt.verifyState"/></td>
          <td><s:property value="#sugt.verifyAdminName"/></td>
          <td><s:a href="admin_suggestHandle.action?suggestion.suggestionId=%{#sugt.suggestionId}">处理</s:a></td>
          <td><s:a href="admin_suggestDelete.action?suggestion.suggestionId=%{#sugt.suggestionId}">删除</s:a></td>
        </tr>
        </s:iterator>
        
	    <TR>
	    <TD height=24 colspan="10" align="center">
	    第<s:property value="page.currentPage"/>页/
		总<s:property value="page.totalPage"/>页
    	
	    <s:if test="page.hasPrePage">
			<s:url var="firstPage" action="seeSuggestionList.action">
			<s:param name="page.currentPage" value="1"></s:param>
			</s:url>
			<s:a href="%{firstPage}">首页</s:a>
			<s:url var="prePage" action="seeSuggestionList.action">
			<s:param name="page.currentPage" value="page.currentPage-1"></s:param>
			</s:url>
			<s:a href="%{prePage}">上一页</s:a>
			</s:if>
		<s:else>
			<EM>首页</EM>
			<EM>上一页</EM>
		</s:else>
		<s:if test="page.hasNextPage">
			<s:url var="nextPage" action="seeSuggestionList.action">
			<s:param name="page.currentPage" value="page.currentPage+1"></s:param>
			</s:url>
			<s:a href="%{nextPage}">下一页</s:a>
			<s:url var="lastPage" action="seeSuggestionList.action">
			<s:param name="page.currentPage" value="page.totalPage"></s:param>
			</s:url>
			<s:a href="%{lastPage}">尾页</s:a>
		</s:if>
		<s:else>
			<EM>下一页</EM>
			<EM>尾页</EM>
		</s:else>
	    </TD>
	  </TR>
       
      </table>
      <br>
      <div class="bottom"><a href="#" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image8','','xiaoyaoimg/shouye2.jpg',1)"><img src="xiaoyaoimg/shouye.jpg" name="Image8" width="46" height="28" border="0" id="Image8" /></a>&nbsp;&nbsp;<a href="#" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image4','','xiaoyaoimg/xiangzuo2.jpg',1)"><img src="xiaoyaoimg/xiangzuo.jpg" name="Image4" width="29" height="28" border="0" id="Image4" /></a>&nbsp;&nbsp;<a href="#" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image5','','xiaoyaoimg/xiaoyou2.jpg',1)"><img src="xiaoyaoimg/xiaoyou.jpg" name="Image5" width="29" height="28" border="0" id="Image5" /></a>&nbsp;&nbsp;<a href="#" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image7','','xiaoyaoimg/weiye2.jpg',1)"><img src="xiaoyaoimg/weiye.jpg" name="Image7" width="44" height="28" border="0" id="Image7" /></a></div>
    </div>
  </div>
</div>
</body>
</html>
