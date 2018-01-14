<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'register_success.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<meta http-equiv="refresh" content="3;url=login.jsp">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

<style type="text/css">
.bound {
	background-image:url(xiaoyaoimg/successBK.png);
	height: 650px;
	width: aotu;
	background-repeat: no-repeat;
}
.text {
	z-index: 1;
	padding-top: 150px;
	width: 800px;
	margin-top: 0px;
	margin-right: auto;
	margin-bottom: 0px;
	margin-left: auto;	
	
}

h1{
	font-family: "微软雅黑";
	font-size: 24px;
	}
	
h2{
	font-family: "微软雅黑";
	font-size: 18px;
	font-weight: normal;
	}	
	


</style>

  </head>
  
  <body>
   <div class="bound">
  <div class="text">
    <h1><center>恭喜您注册成功！3s后自动返回到登录界面 </center></h1>
    
    <h2><center>
    等不及了<a href="login.jsp">点击这里登录</a>
    </center></h2>
    
   
  </div>
</div>
 
  </body>
</html>
