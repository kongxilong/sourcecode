<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>路线评价</title>
<style>
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
.box2 {
	background-color: #EFEFEF;
	height: 40px;
	width: 950px;
	border-bottom-width: 2px;
	border-bottom-style: solid;
	border-bottom-color: #F60;
	margin-top: 50px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box2_title {
	font-size: 20px;
	line-height: 37px;
	color: #FFF;
	background-color: #F60;
	margin: auto;
	float: left;
	width: 150px;
	height: 40px;
	font-weight: bolder;
	text-align: center;
}
.box2_seller {
	float: right;
	height: 30px;
	width: 300px;
	margin-top: 10px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box3 {
	height: 300px;
	width: 948px;
	margin-top: 20px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
	border: 1px solid #DDD;
}
.btn {
	font-size: 24px;
	height: 38px;
	width: 170px;
	margin-right: 30px;
	margin-left: 600px;
}
.bottom_btn {
	height: 40px;
	width: 950px;
	margin-top: 100px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box3_left {
	float: left;
	height: 250px;
	width: 250px;
	margin-top: 20px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: 20px;
}
.box3_left_picture {
	height: 200px;
	width: 200px;
	margin-top: auto;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box3_left_text {
	height: 30px;
	width: 250px;
	margin-top: 20px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.box3_right {
	height: 250px;
	width: 600px;
	margin-top: 20px;
	margin-left: 40px;
	float: left;
	background-color: #EEF8FD;
	border: 1px solid #DAF1FC;
}
.box3_right_bottom {
	height: 30px;
	width: 300px;
	margin-top: 20px;
	margin-right: auto;
	margin-left: auto;
	float: right;
}
.text {
	height: 170px;
	width: 530px;
	margin-top: 20px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: 35px;
}
</style>
</head>

<body>
<center>
<form action="addRouteComments.action" method="post">
<div class="box1">
  <div class="logo"><img src="xiaoyaoimg/logo.png" alt="" width="187" height="85" /></div>
  <div class="logo2"></div>
</div>
<div class="box2">
  <div class="box2_title">路线评价</div>
  <div class="box2_seller">卖家：<s:property value="route.username"/></div>
</div>
<div class="box3">
  <div class="box3_left">
    <div class="box3_left_picture"><img src=<s:property value="route.imageDir"/> alt="" width="240" height="135" />
    </div>
    <div class="box3_left_text">
    
    路线名称：<s:property value="route.routeName"/>
    </div>
  </div>
  <div class="box3_right"> 
  <textarea name="routeCommentsDTO.comment" rows="" class="text"></textarea>
    <div class="box3_right_bottom">     
         <label>
      <input type="radio" name="routeCommentsDTO.radio" value="公开评价" id="RadioGroup1_0" />
      公开评价</label>   
    <label>
      <input type="radio" name="routeCommentsDTO.radio" value="匿名评价" id="RadioGroup1_1" checked />
      匿名评价</label>
   </div>
  </div> 
</div>
<div class="bottom_btn">
  <input type="submit" class="btn" value="提交评价" />
   </div>
</form>   
</center>
</body>
</html>

