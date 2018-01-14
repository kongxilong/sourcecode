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
	width: 1007px;
	margin: auto;
	border-radious:2px;
}
.btn {
	margin: auto;
	height: 25px;
	width: 100px;
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
	float: right;
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
.all_box1 {
	background-color: #FFF;
	margin: auto;
	height: 500px;
	width: 1000px;
	border: 4px solid #CFF;
}
.outborder {
	height: 508px;
	width: 1010px;
	border: 2px solid #00F;
	margin: auto;
}
.all_left {
	height: 492px;
	width: 650px;
	float: left;
	background-color: #FFF;
	border-top-width: thin;
	border-right-width: thin;
	border-bottom-width: thin;
	border-left-width: thin;
	border-top-style: none;
	border-right-style: solid;
	border-bottom-style: none;
	border-left-style: none;
	border-top-color: #00F;
	border-right-color: #00F;
	border-bottom-color: #00F;
	border-left-color: #00F;
}
.pictures {
	height: 282px;
	width: 650px;
	background-color: #FFF;
	margin-top: 20px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.all_left_bottom {
	background-color: #FFF;
	height: 198px;
	width: 650px;
	font-size: 20px;
}
.all_left_title {
	background-color: #FFF;
	height: 30px;
	width: 650px;
	margin: auto;
	font-size: 20px;
	color: #C00;
	font-weight: bolder;
	border-bottom-width: 1px;
	border-bottom-style: none;
	border-bottom-color: #666;
}
.shortline {
	height: 0px;
	width: 250px;
	border-bottom-width: 3px;
	border-bottom-style: solid;
	border-bottom-color: #F00;
	margin-top: 8px;
}
.all_right {
	background-color: #FFF;
	margin: auto;
	height: 400px;
	width: 300px;
	float: right;
}
.all_right_1 {
	background-color: #FFF;
	margin: auto;
	height: 300px;
	width: 300px;
	font-family: Helvetica, Arial, sans-serif;
	font-size: 16px;
	color: #00F;
}
.all_right_title {
	height: 30px;
	width: 300px;
	border-bottom-width: 1px;
	border-bottom-color: #666;
	color: #C00;
	font-weight: bolder;
	font-size: 20px;
}
.weibo {
	width:200px;
	height:185px;
	font-size:14px;
	font-family:"微软雅黑";
	float:left;
}
.weibo1 {
	width:80px;
	height:80px;
	float:left;
	margin:0 0 0 15px;
	text-decoration: none;
	background:url(xiaoyaoimg/weixin.jpg) no-repeat 0 0;
	display:block;
}
.weibo1_sao {
	width:65px;
	height:80px;
	font-size:15px;
	font-family:"微软雅黑";
	color:black;
	margin:15px 5px 4px 15px;
	float:left;
}
.weibo2 {
	width:168px;
	height:45px;
	text-decoration: none;
	background:url(xiaoyaoimg/xinlang.png) no-repeat 0 0;
	display:block;
	margin:90px 5px 4px 15px;
}
.all_right_2 {
	background-color: #FFF;
	height: 198px;
	width: 300px;
	font-size: 16px;
	color: #00F;
}
.weibo3 {
	width:168px;
	height:47px;
	text-decoration: none;
	background:url(xiaoyaoimg/tengxun.png) no-repeat 0 0;
	display:block;
	margin:5px 5px 4px 15px;
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
* {
	margin:0;
	padding:0;
}
body {
	background:#FFF;
	color:#333;
	font:12px/1.6em Helvetica, Arial, sans-serif;
}
h1, h2, h3, h4, h5, h6 {
	font-size:1em;
}
a {
	color:#0287CA;
	text-decoration:none;
}
a:hover {
	text-decoration:underline;
}
ul, li {
	list-style:none;
}
fieldset, img {
	border:none;
}
legend {
	display:none;
}
em, strong, cite, th {
	font-style:normal;
	font-weight:normal;
}
input, textarea, select, button {
	font:12px Helvetica, Arial, sans-serif;
}
table {
	border-collapse:collapse;
}
html {
	overflow:-moz-scrollbars-vertical;
}
#ibanner_btn .current {
	height:28px;
	border:1px solid #FF5300;
	color:#FF5300;
	font-size:28px;
	line-height:28px;
}
#ibanner {
	position:relative;
	width:645px;
	height:250px;
	overflow:hidden;
	margin:auto;
}
#ibanner_pic {
}
#ibanner_pic a {
	position:absolute;
	top:0;
	display:block;
	width:645px;
	height:250px;
	overflow:hidden;
}
#ibanner_btn {
	position:absolute;
	z-index:9999;
	right:5px;
	bottom:5px;
	font-weight:700;
	font-family:Arial;
}
#ibanner_btn span {
	display:block;
	float:left;
	margin-left:4px;
	padding:0 5px;
	background:#000;
	cursor:pointer;
}
#ibanner_btn .normal {
	height:20px;
	margin-top:8px;
	border:1px solid #999;
	color:#999;
	font-size:16px;
	line-height:20px;
}
</style>
<script type="text/javascript">
function $(id) { return document.getElementById(id); }
function addLoadEvent(func){
var oldonload = window.onload;
if (typeof window.onload != 'function') {
window.onload = func;
} else {
window.onload = function(){
oldonload();
func();
}
}
}
function addBtn() {
if(!$('ibanner')||!$('ibanner_pic')) return;
var picList = $('ibanner_pic').getElementsByTagName('a');
if(picList.length==0) return;
var btnBox = document.createElement('div');
btnBox.setAttribute('id','ibanner_btn');
var SpanBox ='';
for(var i=1; i<=picList.length; i++ ) {
var spanList = '<span class="normal">'+i+'</span>';
SpanBox += spanList;
}
btnBox.innerHTML = SpanBox;
$('ibanner').appendChild(btnBox);
$('ibanner_btn').getElementsByTagName('span')[0].className = 'current';
for (var m=0; m<picList.length; m++){
var attributeValue = 'picLi_'+m
picList[m].setAttribute('id',attributeValue);
}
}
function moveElement(elementID,final_x,final_y,interval) {
if (!document.getElementById) return false;
if (!document.getElementById(elementID)) return false;
var elem = document.getElementById(elementID);
if (elem.movement) {
clearTimeout(elem.movement);
}
if (!elem.style.left) {
elem.style.left = "0px";
}
if (!elem.style.top) {
elem.style.top = "0px";
}
var xpos = parseInt(elem.style.left);
var ypos = parseInt(elem.style.top);
if (xpos == final_x && ypos == final_y) {
moveing = false;
return true;
}
if (xpos < final_x) {
var dist = Math.ceil((final_x - xpos)/10);
xpos = xpos + dist;
}
if (xpos > final_x) {
var dist = Math.ceil((xpos - final_x)/10);
xpos = xpos - dist;
}
if (ypos < final_y) {
var dist = Math.ceil((final_y - ypos)/10);
ypos = ypos + dist;
}
if (ypos > final_y) {
var dist = Math.ceil((ypos - final_y)/10);
ypos = ypos - dist;
}
elem.style.left = xpos + "px";
elem.style.top = ypos + "px";
var repeat = "moveElement('"+elementID+"',"+final_x+","+final_y+","+interval+")";
elem.movement = setTimeout(repeat,interval);
}
function classNormal() {
var btnList = $('ibanner_btn').getElementsByTagName('span');
for (var i=0; i<btnList.length; i++){
btnList[i].className='normal';
}
}
function picZ() {
var picList = $('ibanner_pic').getElementsByTagName('a');
for (var i=0; i<picList.length; i++){
picList[i].style.zIndex='1';
}
}
var autoKey = false;
function iBanner() {
if(!$('ibanner')||!$('ibanner_pic')||!$('ibanner_btn')) return;
$('ibanner').onmouseover = function(){autoKey = true};
$('ibanner').onmouseout = function(){autoKey = false};

var btnList = $('ibanner_btn').getElementsByTagName('span');
var picList = $('ibanner_pic').getElementsByTagName('a');
if (picList.length==1) return;
picList[0].style.zIndex='2';
for (var m=0; m<btnList.length; m++){
btnList[m].onmouseover = function() {
for(var n=0; n<btnList.length; n++) {
if (btnList[n].className == 'current') {
var currentNum = n;
}
}
classNormal();
picZ();
this.className='current';
picList[currentNum].style.zIndex='2';
var z = this.childNodes[0].nodeValue-1;
picList[z].style.zIndex='3';
if (currentNum!=z){
picList[z].style.left='650px';
moveElement('picLi_'+z,0,0,10);
}
}
}
}
setInterval('autoBanner()', 5000);
function autoBanner() {
if(!$('ibanner')||!$('ibanner_pic')||!$('ibanner_btn')||autoKey) return;
var btnList = $('ibanner_btn').getElementsByTagName('span');
var picList = $('ibanner_pic').getElementsByTagName('a');
if (picList.length==1) return;
for(var i=0; i<btnList.length; i++) {
if (btnList[i].className == 'current') {
var currentNum = i;
}
}
if (currentNum==(picList.length-1) ){
classNormal();
picZ();
btnList[0].className='current';
picList[currentNum].style.zIndex='2';
picList[0].style.zIndex='3';
picList[0].style.left='650px';
moveElement('picLi_0',0,0,10);
} else {
classNormal();
picZ();
var nextNum = currentNum+1;
btnList[nextNum].className='current';
picList[currentNum].style.zIndex='2';
picList[nextNum].style.zIndex='3';
picList[nextNum].style.left='650px';
moveElement('picLi_'+nextNum,0,0,10);
}
}
addLoadEvent(addBtn);
addLoadEvent(iBanner);
</script>
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
    <li><a href="#">首页</a> </li>
    <li><a href="suggest_fuction.jsp">功能建议</a></li>
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
<div class="outborder">
  <div class="all_box1">
    <div class="all_left">
      <div class="pictures"><!--ibanner end-->
        
        <div id="ibanner">
          <div id="ibanner_pic"> <a href="/jscss/"><img src="xiaoyaoimg/one.jpg" alt="" width="645" height="250" /></a> <a href="/sort/index.shtml"><img src="xiaoyaoimg/two.jpg" alt="" width="645" height="250" /></a> <a href="/other/lastupdate.shtml"><img src="xiaoyaoimg/three.jpg" alt="" width="645" height="250" /></a> <a href="/other/top100.shtml"><img src="xiaoyaoimg/four.jpg" alt="" width="645" height="250" /></a> </div>
        </div>
      </div>
      <div class="all_left_bottom">
        <div class="all_left_title">功能建议
          <input name="input" type="button" class="btn" value="我要提建议" />
          <div class="shortline"></div>
        </div>
      </div>
    </div>
    <div class="all_right">
      <div class="all_right_1"> <br>
        <div class="all_right_title">最新动态
          <div class="shortline"></div></div>
          <br><li>关注我们逍遥游微博吧!</li>
          <br>
          <div class="weibo"> <a href="#" class="weibo1"></a>
            <div class="weibo1_sao"> &nbsp;&nbsp;<span style="font-size:14px;font-family:"微软雅黑";">扫一扫</span> <br>
              <span style="font-size:14px;font-family:"微软雅黑";">关注我们</span> </div>
            <a href="#" class="weibo2"></a> <a href="#" class="weibo3"></a> </div>
        </div>
 
      <div class="all_right_2">
        <div class="all_right_title"> 常见问题
          <div class="shortline"></div>
        </div>
        <br><li>如何注册会员？</li>
        <li>线路如何预订？</li>
        <li>资格如何审查</li>
      </div>
    </div>
  </div>
</div>
<script type="text/javascript">
var MenuBar1 = new Spry.Widget.MenuBar("MenuBar1", {imgDown:"SpryAssets/SpryMenuBarDownHover.gif", imgRight:"SpryAssets/SpryMenuBarRightHover.gif"});
</script>
</body>
</html>
