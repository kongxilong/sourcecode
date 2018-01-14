<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>逍遥游购物车</title>
<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script> 

<style type="text/css">
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
}
.box1_1 {
	height: 100px;
	width: 250px;
	background-color: #FFF;
	float: left;
}
.box1_2 {
	background-color: #FFF;
	float: right;
	height: 100px;
	width: 750px;
}
.box2 {
	height: 30px;
	width: 1000px;
	margin: auto;
}
.text {
	font-family: "Microsoft Yahei";
	color: #333;
	margin: auto;
	height: 30px;
	width: 80px;
	float: left;
}
.text2 {
	float: left;
	height: 30px;
	width: 300px;
	font-family: "Microsoft Yahei";
}
.logotext {
	height: 50px;
	width: 145px;
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
.sinput {
	width:400px;
	height:34px;
	line-height:21px;
	color:b3b3b3;
	border:4px solid #4d90fe;
	border-radius:2px 0 0 2px;
	background-color:#fbfbfb;
	margin: auto;
	float: left;
}
.sbtn {
	width:73px;
	height:42px;
	border-radius:0 2px 2px 0;
	border:1px solid #4d90fe;
	background-color:#4d90fe;
	cursor:pointer;
	display:inline-block;
	font-size:12px;
	color:#f3f7fc;
	margin-top: 0px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: 10px;
}
.sbtn:hover {
	background:#4084f2
}
.box3 {
	background-color: #FFF;
	height: 30px;
	width: 1000px;
	border-top-width: 4px;
	border-right-width: 4px;
	border-bottom-width: 4px;
	border-left-width: 4px;
	border-top-style: solid;
	border-right-style: none;
	border-bottom-style: none;
	border-left-style: none;
	border-top-color: #004A80;
	border-right-color: #6FC;
	border-bottom-color: #18F6FC;
	border-left-color: #6FC;
	margin: auto;
}
.box4 {
	background-color: #FFF;
	height: 356px;
	width: 1000px;
	margin-top: auto;
	margin-right: 0px;
	margin-bottom: auto;
	margin-left: auto;
}




.delete {
	background-color: #CF9;
	height: 40px;
	width: 60px;
	margin: auto;
	top: 10px;
}
.bottom {
	margin: auto;
	height: 35px;
	width: 200px;
}
</style>

<style type="text/css">
table{ border-collapse:collapse; border-spacing:0; } 
a{
	color:#3A3A3A;
	text-decoration:none;
	color:#575757;
	font-size: 16px;
} 


.table_box {
	margin: auto;
	background-color: #FFF;
}
table{ border-collapse:collapse; border-spacing:0; } 

.L_user .mailspage .mail table{
	width:745px;
	text-align:center;
	background-color: #FFF;
} 
.L_user .mailspage .mail .table_box{
	width:745px;
	border:1px solid #DDDDDD;
	margin:0 0 0px 10px;
	background-color: #FFF;
} 
.L_user .mailspage .mail .firsttr{border-top:none;} 
.L_user .mailspage .mail td{ height:30px; line-height:30px;} 
.L_user .mailspage{
	width:768px;
	float:left;
	overflow:hidden;
	height: 350px;
} 
.L_user .mailspage .mail .markbtn a{
	width:60px;
	height:20px;
	display:inline-block;
	text-align:center;
	line-height:25px;
	vertical-align:middle;
	background-color: #FFF;
	margin: auto;
	color: #F00;
} 
.L_user .mailspage .mail .firsttr{ background:#FFF; border-bottom:1px solid #DDDDDD;border-top:1px solid #DDDDDD; height:20px; line-height:20px;}
</style> 


<style type="text/css">

#apDiv3 {
	position:absolute;
	left:397px;
	top:520px;
	width:49px;
	height:40px;
	z-index:1;
	background-color: #FFFFFF;
}
</style>
</head>

<body>
<center>
<div class="head" > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      您好！欢迎来到 <a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！ 
      <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a> 
      <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a> 
      <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a> 
      <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a> 
      <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a> </div>
</br>
<div class="box1">
  <div class="box1_1">
    <div class="logo"><img src="xiaoyaoimg/logo.png" width="100" height="95" alt="logo" /> </div>
    <div class="logotext"><img src="xiaoyaoimg/gouwuche.png" width="136" height="42" alt="购物车" /></div>
  </div>
  <div class="box1_2">
  <br/><br/><br/>
    <form action="#" method="get">
      <input type="text" name="s" class="sinput" placeholder="输入 回车搜索" autofocus x-webkit-speech/>
      <input type="submit" value="搜索" class="sbtn" style="font-size:25px" />
    </form>
  </div>
</div>
<br/>
<div class="box2">
  <div class="text" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">全部商品
  
  </div>
  <div class="text2" >购物车总数：<a href="#" style="color:#00F;text-decoration:none"onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">7</a>
  </div>
</div>
<div class="box3">



</div>
<div class="box4">
  <div class="L_user clear-fix"> 


<div class="mailspage"> 

<div class="mail"> 
<div class="table_box"> 
<table> 
<tr id="titletr"> 
<td height="38" class="firsttr"> <input type="checkbox" id="allcheckbox" />
  全选</td> 
<td class="firsttr"><p class="markbtn" ><a href="javascript:void(0)" class="deletebtn">删除</a>&nbsp;</td> 
<td class="firsttr"><input name="支付" type="button" value="确认支付" /></td> 
<td class="firsttr">&nbsp;</td> 
<td class="firsttr">&nbsp;</td> 
<td class="firsttr">&nbsp;</td> 
</tr> 
<tr> 
<td>选择</td> 
<td>序号</td> 
<td>路线</td> 
<td>旅行社</td> 
<td>发团时间</td> 
<td>旅行时间</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>1</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>2</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>3</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>4</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>5</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>6</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr> 
<tr> 
<td><input type="checkbox" /></td> 
<td>7</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
<td>XXX</td> 
</tr>
</table> 


</div> 
</div> 

</div> 


</div>



</div>
<div class="bottom"><a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image7','','xiaoyaoimg/shouye2.jpg',1)"><img src="xiaoyaoimg/shouye.jpg" name="Image7" width="46" height="28" border="0" id="Image7" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image4','','xiaoyaoimg/xiangzuo2.jpg',1)"><img src="xiaoyaoimg/xiangzuo.jpg" name="Image4" width="29" height="28" border="0" id="Image4" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image5','','xiaoyaoimg/xiaoyou2.jpg',1)"><img src="xiaoyaoimg/xiaoyou.jpg" name="Image5" width="29" height="28" border="0" id="Image5" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image6','','xiaoyaoimg/weiye2.jpg',1)"><img src="xiaoyaoimg/weiye.jpg" name="Image6" width="44" height="28" border="0" id="Image6" /></a></div>
<!--全选删除js -->
<script type="text/javascript">//删除选中： 
$(".markbtn .deletebtn").click(function(){ 
$(".table_box tr").each(function(){ 
if($(this).find("input").attr("checked")&&$(this).index()!=0){ 
$(this).remove(); 
} 
}) 
}) 
$("#allcheckbox").click(function(){ 
if($(this).attr("checked")){ 
$(".table_box td input").attr("checked","checked"); 
}else{ 
$(".table_box td input").attr("checked","") 
} 
}) 

$(".table_box input").not("#allcheckbox").click(function(){ 
$(".table_box input").not("#allcheckbox").each(function(){ 
if($(".table_box input[type='checkbox']:checked").not("#allcheckbox").length==$(".table_box tr").not("#titletr").length){ 
$("#allcheckbox").attr("checked","checked"); 
}else{ 
$("#allcheckbox").attr("checked",""); 
} 
}) 

})</script>
</center>
</body>
</html>
