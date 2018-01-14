<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'register_fail.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<meta http-equiv="refresh" content="3;url=register_fail.jsp">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
	<style type="text/css">
.bound {
	background-image: url(xiaoyaoimg/wallpaper-3031702.jpg);
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
	color: #000;
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
    <h1><center>验证码错误，请重新输入！</center></h1>
    
    <h2><center>
  3秒后将跳转到注册页面！ <br>
    还没跳转，点击这里<a href="register.jsp">注册</a>
    或者，点击这里回到<a href="main.jsp">逍遥游主页</a>
    </center></h2>
 
  </div>
</div>

  </body>
</html>
