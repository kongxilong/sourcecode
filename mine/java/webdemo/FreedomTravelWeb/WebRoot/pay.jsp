<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>逍遥游支付界面</title>
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
  	    <div class="logo2"><img src="xiaoyaoimg/wodeshouyingtai.PNG" width="240" height="44" alt="我的收银台" /></div>
</div>

<div class="line"></div>
<s:iterator value="dealList" id="deal">

<form action="payDeal.action?dealTemp.dealId=<s:property value="#deal.dealId"/>" method="post">

<div class="box2"> <br>
  <table width="596" border="0" align="left">
    <tr>
      <td width="200">购买线路名称：<s:property value="#deal.getRouteName()"/></td>
      <td width="123">&nbsp;</td>
      <td width="121">卖家名称：<s:property value="#deal.getUsername()"/></td>
      <td width="231">&nbsp;</td>
    </tr>
    <tr>
      <td height="27">总共金额 ：<s:property value="#deal.getTotalPrice()"/>元</td>
      <td>&nbsp;</td>
      <td><a href="" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;订单详情&#93;</span></blink></a></td>
      <td>&nbsp;</td>
    </tr>
  </table>
</div>
<div class="box3"> <br>
  <div class="box3_1">付款方式</div>
  <div class="box3_2">
    <div class="box3_2_1">
      <ul class="bankcards">
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/icbc.jpg" alt="中国工商银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国银行" />
          <img src="xiaoyaoimg/bc.jpg" alt="中国银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="农业银行" />
          <img src="xiaoyaoimg/ab.jpg" alt="农业银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国邮政储蓄" />
          <img src="xiaoyaoimg/yb.jpg" alt="中国邮政储蓄" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国建设银行" />
          <img src="xiaoyaoimg/ccb.jpg" alt="中国建设银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国交通银行" />
          <img src="xiaoyaoimg/jb.jpg" alt="中国交通银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/pfb.jpg" alt="浦发银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/nb.jpg" alt="南京银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/msb.jpg" alt="中国民生银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/gzb.jpg" alt="广州银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/jsb.jpg" alt="江苏银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/gb.jpg" alt="广发银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/zb.jpg" alt="招商银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/hb.jpg" alt="华夏银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/msb.jpg" alt="中国工商银行" width="126" height="36" class="banklogo" /> </li><li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/gdb.jpg" alt="中国光大银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/yl.jpg" alt="银联手机支付行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/hfb.jpg" alt="恒丰银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/zsb.jpg" alt="浙商银行" width="126" height="36" class="banklogo" /> </li>
        <li class="onebank">
          <input name="btn" type="radio"  value="中国工商银行" />
          <img src="xiaoyaoimg/visa.jpg" alt="visa" width="126" height="36" class="banklogo" /> </li>
          
          
      </ul>
    </div>  
    请输入支付密码：
    <input type="password" name="dealTemp.password"/>
    <div class="box3_2_2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
    <input  type="submit" class="btn" value="确认支付"/>
	
    </div>

  </div>
</div>
</form>
</s:iterator>
</body>
</html>
