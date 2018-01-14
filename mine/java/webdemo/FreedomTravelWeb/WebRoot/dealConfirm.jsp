<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>逍遥游确认交易界面</title>
<style type="text/css">
.box1 {
	height: 85px;
	width: 1000px;
	margin: auto;
}
.logo {
	margin: auto;
	float: left;
	height: 85px;
	width: 200px;
	border-right-width: 1px;
	border-right-style: solid;
	border-right-color: #CCC;
}
.logo2 {
	float: left;
	height: 50px;
	width: 250px;
	margin-top: 25px;
}
.line {
	margin: auto;
	height: 10px;
	width: 1000px;
	border-bottom-width: thin;
	border-bottom-style: solid;
	border-bottom-color: #CCC;
}
.box2 {
	background-color: #E4F8C3;
	margin: auto;
	height: 90px;
	width: 1006px;
	border-bottom-width: 3px;
	border-bottom-style: solid;
	border-bottom-color: #87A74D;
}
.box3 {
	margin: auto;
	height: 600px;
	width: 1000px;
}
.box3_1 {
	height: 40px;
	width: 1000px;
	font-size: 20px;
	margin-top: auto;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box3_2 {
	margin: auto;
	height: 500px;
	width: 1000px;
	border: 3px solid #BCCCEE;
}
.box3_2_1 {
	background-color: #EBEFF8;
	height: 350px;
	width: 950px;
	margin-top: 30px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
bankcards {
	height: 300px;
	width: 950px;
	list-style-type: none;
}
.onebank {
	margin: 0px;
	float: left;
	height: 38px;
	width: 200px;
	list-style-type: none;
	background-color: #EBEFF8;
	padding: 10px;
}
.box3_2_2 {
	height: 60px;
	width: 1000px;
	background-color: #EBEFF8;
	margin-top: 60px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.banklogo {
	margin-bottom: -10px;
	height: 38px;
	width: 128px;
}
.password {
	line-height: 18px;
	height: 20px;
	width: 200px;
	margin-left: 50px;
	margin-top: 10px;
}
.btn {
	height: 30px;
	width: 80px;
	margin-left: 20px;
}
</style>
</head>

<body>

<div class="box1">
		<div class="logo"><img src="xiaoyaoimg/logo.png" alt="" width="187" height="85" /></div>
</div>

<div class="line"></div>
<center>
<h1>温馨提示，输入密码后将付款给卖家。即交易完成！</h1>
<form action="finishDeal.action?dealTemp.dealId=<s:property value="dealTemp.dealId"/>" method="post">
  	请输入确认交易结束密码：
 	<input type="password" name="dealTemp.password"/>
    <div class="box3_2_2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
    <input  type="submit" class="btn" value="完成交易"/></div>
</form>
 </center>
</body>
</html>