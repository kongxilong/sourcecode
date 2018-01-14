<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>新手指南</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
<style type="text/css">
			*{margin:0px;padding:0px;}
			  .head{width:100%;background:#f7f7f7;height:30px;margin:0px auto;font-size:16px;font-family:"微软雅黑";}
			  
			  .topdiv{width:1210px;height:125px;margin:0 auto;}
				.top{width:363px;height:120px;margin:5px;float:left}
				.top_left{width:170px;height:120px;float:left;border-right:2px solid #e8e8e8;}
				.top_right{width:189px;height:120px;float:right;padding:45px 0 0 0}	
				.sousuo{width:525px;height:40px;padding:40px 10px 0 50px;float:left;}
			.search_form{ height:40px;display:inline-block;padding:25px 0 0 0} 
        .search_form:hover{-webkit-box-shadow:0 0 3px #999;-moz-box-shadow:0 0 3px #999} 
        .sinput{float:left; width:400px; height:34px; line-height:21px; padding:3px 7px;
        	      color:b3b3b3; border:4px solid #4d90fe; border-radius:2px 0 0 2px; background-color:#fbfbfb;} 
        .sbtn{float:left; width:90px; height:48px; padding:0 12px; margin-left:-1px;
        	    border-radius:0 2px 2px 0; border:1px solid #4d90fe; background-color:#4d90fe;
        	    cursor:pointer; display:inline-block; font-size:12px; vertical-align:middle; color:#f3f7fc;position:absolute;} 
        .sbtn:hover{background:#4084f2}
        .t_right{width:150px;height:120px;float:left;}	
				
.text {
	clear: both;
}

h1{
	font:"微软雅黑";
	font-size:24px;
	margin-top:80px;
	margin-left:120px;
	margin-bottom:20px;
	}


.inf {
	margin-right: 150px;
	margin-left: 150px;
}
.tip {
	font-family: "黑体";
	font-size: 20px;
	font-weight: bold;
	color: #06F;
	background-color: #CCC;
	padding: 10px;
	border-top-style: none;
	border-right-style: none;
	border-bottom-style: none;
	border-left-style: none;
}
.tip_sign {
	padding-top: 10px;
}
p{
	padding:10px;
	font:"黑体";
	}
</style>

  </head>
  
  <body>
   <div class="head" >
			  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;您好！欢迎来到
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！
		     <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a>			
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" 
			   	onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a>
</div>
		 
<div class="topdiv">
			   <div class="top" >
				   <div class="top_left">
				      <img src="xiaoyaoimg/logo.png" alt="逍遥游" width="170" height="120" />
			     </div>
			     <div class="top_right">
				      <img src="xiaoyaoimg/wel.png" alt="欢迎来到" width="189" height="47" />
			     </div>
			   </div>
		     <div class="sousuo">
 							<form action="#" method="get"> 
							  <input type="text" name="s" class="sinput" placeholder="输入 回车搜索" autofocus x-webkit-speech>
							  <input type="submit" value="搜索" class="sbtn" style="font-size:25px" >
			   </form>
         </div>
  <div class="t_right">
				      <img src="xiaoyaoimg/best.png" alt="best" width="150" height="117" />
				  </div>
		 </div> 
<div class="text">
    <h1>新手指南</h1>
    <hr size="2" color="#0066FF" />
  <div class="inf">
    <div class="tip"> <a name="register" id="register"></a>如何注册</div>
	   
	    <div class="tip_sign">
          <p>1、打开逍遥游主页点击注册</p>
            <img src="xiaoyaoimg/tip_sign1.png" width="679" height="39" /> 
            <p>2、进入注册界面填写个人信息</p>
            &nbsp;&nbsp;&nbsp;&nbsp;<img src="xiaoyaoimg/tip_sign2.png" width="600" height="296" />
            <p>3、点击提交按钮提交注册</p>
            <p><img src="xiaoyaoimg/tip_sign3.png" width="607" height="131" /> </p>
   	   </div>
         	
   		<div class="tip"><a name="deal" id="deal"></a>如何提交订单</div>
    <div class="tip_deal">
      <p>1、会员在登录后搜索路线进入路线详情页面</p>
         <img src="xiaoyaoimg/tip_deal1.png" width="700" height="204" />
    <p>2、浏览信息后点击“立即预定”</p>
    <img src="xiaoyaoimg/tip_deal2.png" width="700" height="118" />
    <p>3、进入我的订单页面确认信息后点击“提交订单”</p>
    <img src="xiaoyaoimg/tip_deal3.png" width="700" height="363" /> </div>
    
    <div class="tip"><a name="pay" id="pay"></a>如何支付</div>
    <div class="tip_pay">
    <p>1、提交订单后进入支付页面</p>
    <img src="xiaoyaoimg/tip_pay1.png" width="700" height="381" />
	<p>2、选择支付方式</p>
    <p>3、输入支付密码完成支付</p>
    </div>
  </div>
</div>
  </body>
</html>
