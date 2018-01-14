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
<title>无标题文档</title>
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

.daohang {
	background-color: #F00;
	height: 38px;
	width: 1000px;
	margin: auto;
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
	width: 300px;
	background-color: #FFF;
	float: left;
}
.box1_2 {
	background-color: #FFF;
	float: right;
	height: 100px;
	width: 750px;
}
.logotext {
	height: 50px;
	width: 173px;
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

.all {
	background-color: #FFF;
	margin: auto;
	height: 500px;
	width: 1000px;
	border: 1px solid #00F;
}

.all .box1 {
	font-size: 20px;
	color: #C00;
	background-color: #FFF;
	height: 25px;
	width: 1000px;
	margin-top: 3px;
	margin-bottom: 10px;
	border-bottom-width: 1px;
	border-bottom-style: solid;
	border-bottom-color: #CCC;
}
.question {
	margin: auto;
	height: 30px;
	width: 1000px;
}
.btnbox {
	background-color: #FFF;
	margin: auto;
	height: 40px;
	width: 100px;
}
</style>
<script src="SpryAssets/SpryMenuBar.js" type="text/javascript"></script>
<link href="SpryAssets/SpryMenuBarHorizontal.css" rel="stylesheet" type="text/css" />
</head>

<body>
<div class="head" > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  您好！欢迎来到 <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！ <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a> <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a> <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a> <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a> <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a> </div>
<br>
<div class="box1">
  <div class="box1_1">
    <div class="logo"><img src="xiaoyaoimg/logo.png" width="97" height="93" alt="logo" /> </div>
    <div class="logotext"><img src="xiaoyaoimg/wuzhongxin.png" width="173" height="45" alt="服务中心" /></div>
  </div>
</div>
<br>

<div class="daohang">
  <ul id="MenuBar1" class="MenuBarHorizontal">
    <li><a href="suggest_home.jsp">首页</a>    </li>
    <li><a href="#">功能建议</a></li>
    <li><a href="suggest_complaint.jsp">我要投诉</a></li>
    <li><a class="MenuBarItemSubmenu" href="#">近期活动</a>
      <ul>
        <li><a class="MenuBarItemSubmenu MenuBarItemSubmenu" href="#">热门线路</a>
          <ul>
            <li><a href="#">三亚</a></li>
            <li><a href="#">马尔代夫</a></li>
            <li><a href="#">巴厘岛</a></li>
          </ul>
        </li>
<li><a href="#">优惠推荐</a></li>
</ul>
    </li>
    <li><a href="#" class="MenuBarItemSubmenu">常见问答</a>
      <ul>
        <li><a href="#">会员积分</a></li>
        <li><a href="#">会员制度</a></li>
      </ul>
    </li>
  </ul>
</div>
<br>

<form action="fuctionSuggest" method="post">
<div class="all">
  <div class="box1">&nbsp;&nbsp;我要功能提建议</div>
  <div class="question">&nbsp;&nbsp;问题分类 &nbsp;&nbsp;
    <select name="suggestion.suggestType" id="suggestType"><!-- size="1"> -->
      <option>网速过慢</option>
      <option>预订过程</option>
      <option>个人积分</option>
      <option>会员管理</option>
      <option>业务资源</option>
    </select>
  </div>&nbsp;&nbsp;
  内容
  :<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <label for="question"></label>
  <textarea name="suggestion.suggestContent" id="suggestion.suggestContent" cols="70" rows="10"></textarea>
  <br>
  <br>
  &nbsp;&nbsp;您的邮箱：
  <input name="emailadd" type="text" style="height:25px;width:240px;font-size:18px" />
  <br>
  <br>
  &nbsp;&nbsp;验证码：
  <input type="text" class="user_in"  size="6"  name="code" style="width:80px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"/>
  &nbsp;&nbsp;
  <img border=0 title="点击刷新" name="valimg" id="valimg" src="image.jsp" style="width:80px;height:50px;line-height:50px;vertical-align:middle">
  <span id="codeMsg"></span>
	
  
  
  <br> <br> <br> <br> <br>
  <div class="btnbox">
  <input type="submit" value="提交" />
    <input type="reset" value="清空" />
  </div>
</div>
</form>

<script type="text/javascript">
var MenuBar1 = new Spry.Widget.MenuBar("MenuBar1", {imgDown:"SpryAssets/SpryMenuBarDownHover.gif", imgRight:"SpryAssets/SpryMenuBarRightHover.gif"});
</script>
</body>
</html>
