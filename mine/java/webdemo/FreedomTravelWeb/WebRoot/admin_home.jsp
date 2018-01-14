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
<title>信息管理主页面</title>
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
.box2 {
	background-color: #FFF;
	height: 350px;
	width: 1000px;
	margin: auto;
}
.bottomcolor {
	background-color: #ECFFFF;
	margin: auto;
	height: 30px;
	width: 1000px;
	border-bottom-width: 3px;
	border-bottom-style: solid;
	border-bottom-color: #00F;
}
.items1 {
	height: 200px;
	width: 245px;
	float: left;
	margin-top: 60px;
	background-image: url(xiaoyaoimg/users.jpg);
	background-repeat: no-repeat;
	background-position: right bottom;
	border: 1px solid #00F;
}
.images {
	background-color: #C6C;
	height: 120px;
	width: 200px;
	margin-top: 22px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: 22px;
}
.items2 {
	height: 200px;
	width: 245px;
	float: left;
	margin-top: 60px;
	background-image: url(xiaoyaoimg/dealogo.jpg);
	background-repeat: no-repeat;
	background-position: right bottom;
	border-top-width: 1px;
	border-right-width: 1px;
	border-bottom-width: 1px;
	border-left-width: 1px;
	border-top-style: solid;
	border-right-style: none;
	border-bottom-style: solid;
	border-left-style: none;
	border-top-color: #00F;
	border-right-color: #00F;
	border-bottom-color: #00F;
	border-left-color: #00F;
}
.items3 {
	height: 200px;
	width: 245px;
	float: left;
	margin-top: 60px;
	background-image: url(xiaoyaoimg/xianlu.jpg);
	background-repeat: no-repeat;
	background-position: right bottom;
	border-top-width: 1px;
	border-right-width: 1px;
	border-bottom-width: 1px;
	border-left-width: 1px;
	border-top-style: solid;
	border-right-style: none;
	border-bottom-style: solid;
	border-left-style: solid;
	border-top-color: #00F;
	border-right-color: #00F;
	border-bottom-color: #00F;
	border-left-color: #00F;
}
.items4 {
	height: 200px;
	width: 245px;
	float: left;
	margin-top: 60px;
	background-image: url(xiaoyaoimg/suggestionlogo.jpg);
	background-repeat: no-repeat;
	background-position: right bottom;
	border-top-width: 1px;
	border-right-width: 1px;
	border-bottom-width: 1px;
	border-left-width: 1px;
	border-top-style: solid;
	border-right-style: solid;
	border-bottom-style: solid;
	border-left-style: solid;
	border-top-color: #00F;
	border-right-color: #00F;
	border-bottom-color: #00F;
	border-left-color: #00F;
}
.all {
	margin: auto;
	height: 1090px;
	width: 1100px;
}
</style>
<script type="text/javascript">
function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}
function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}
</script>
</head>

<body onload="MM_preloadImages('xiaoyaoimg/userlist2.png','xiaoyaoimg/deallist2.png','xiaoyaoimg/routelist2.png','xiaoyaoimg/suggestionlist2.png')">
<div class="all">
  <div class="head" > <script language=JavaScript>
<!--
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
// -->
</script> 
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>管理员：<%=session.getAttribute("adminOnline")%></span></blink></a>你好！ <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink></blink></a><a href="admin_exit.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;点击退出&#93;</span></blink></a> ,
    
    
    
    访问 <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink></blink></a><a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;逍遥游主页&#93;</span></blink></a> </div>
  <br>
  <div class="box1">
    <div class="box1_1">
      <div class="logo"><img src="xiaoyaoimg/logo.png" width="97" height="93" alt="logo" /> </div>
      <div class="logotext"><img src="xiaoyaoimg/information.png" width="174" height="46" alt="信息管理" /></div>
    </div>
  </div>
  <div class="box2">
    <div class="items1"><a href="seeUserList.action" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image7','','xiaoyaoimg/userlist2.png',1)"><img src="xiaoyaoimg/userlist.PNG" name="Image7" width="200" height="120" border="0" class="images" id="Image7" /></a></div>
    <div class="items2"><a href="seeDealList.action" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image6','','xiaoyaoimg/deallist2.png',1)"><img src="xiaoyaoimg/deallist.PNG" name="Image6" width="200" height="120" border="0" class="images" id="Image6" /></a></div>
    <div class="items3"><a href="seeRouteList.action" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image8','','xiaoyaoimg/routelist2.png',1)"><img src="xiaoyaoimg/routelist.PNG" name="Image8" width="200" height="120" border="0" class="images" id="Image8" /></a></div>
    <div class="items4"><a href="seeSuggestionList.action" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image9','','xiaoyaoimg/suggestionlist2.png',1)"><img src="xiaoyaoimg/suggestionlist.PNG" name="Image9" width="200" height="120" border="0" class="images" id="Image9" /></a></div>
  </div>
  <div class="bottomcolor"></div>
</div>
</body>
</html>
