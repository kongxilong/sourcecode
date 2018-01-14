<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html >
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>逍遥游路线发布</title>
<script type="text/javascript" charset="utf-8" src="js/jquery.js"></script>
<script type="text/javascript" charset="utf-8" src="js/ajaxfileupload.js"></script><!--<script type="text/javascript" charset="utf-8" src="js/imageUpload.js"></script>-->

<style type="text/css">
.head {
	width:100%;
	background:#f7f7f7;
	height:30px;
	margin:0px auto;
	font-size:16px;
	font-family:"微软雅黑";
}
.logo_welcome {
	height: 40px;
	width: 1000px;
	top: 10px;
	margin-top: auto;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.logo_welcome_1 {
	height: 100px;
	width: 300px;
	background-color: #FFF;
	float: left;
}
.logo_welcome_2 {
	background-color: #FFF;
	float: left;
	height: 30px;
	width: 100px;
	margin-top: 50px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: 100px;
}
.logotext {
	height: 50px;
	width: 145px;
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
.box2 {
	height: 30px;
	width: 1000px;
	margin: auto;
}
.all {
	background-color: #FFF;
	height: 1000px;
	width: 1000px;
	margin: auto;
}
.box1 {
	background-color: #FFF;
	height: 30px;
	width: 1000px;
	font-size: 24px;
	margin: auto;
}
.line {
	margin: auto;
	height: 0px;
	width: 1000px;
	border-top-style: solid;
	border-right-style: none;
	border-bottom-style: none;
	border-left-style: none;
	border-top-color: #004A80;
	background-color: #FFF;
}
.box2 {
	background-color: #FFF;
	height: 580px;
	width: 1000px;
	margin: auto;
	top: 2px;
}
.text {
	background-color: #FFF;
}
.box4 {
	height: 1000px;
	width: 1000px;
}
.top {
	background-color: #ECFFFF;
	margin: auto;
	height: 28px;
	width: 1000px;
}
</style>
<style type="text/css">
table.spin {
	display:inline-table;
*display:inline;
	vertical-align:middle
}
button.spinPlus, button.spinMinus {
	display:block;
	height:10px;
	width:20px;
	font-size:5pt;
	line-height:10pt;
	padding:0;
	border-width:2px;
	background-color: #FFF;
	background-image: url(file:///D|/%E6%88%91%E7%9A%84%E6%96%87%E6%A1%A3/Documents/%E6%9C%AA%E5%91%BD%E5%90%8D%E7%AB%99%E7%82%B9%202/%E5%90%91%E4%B8%8A.jpg);
	background-repeat: no-repeat;
	background-position: center 0;
}
button.spinMinus {
	background-position:center -10px;
	background-color: #FFF;
	background-image: url(file:///D|/%E6%88%91%E7%9A%84%E6%96%87%E6%A1%A3/Documents/%E6%9C%AA%E5%91%BD%E5%90%8D%E7%AB%99%E7%82%B9%202/%E5%90%91%E4%B8%8B.jpg);
}
</style>
<script type="text/javascript">
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}
function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
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

<body onLoad="MM_preloadImages('xiaoyaoimg/dingjihuidingbu2.png')">
<center>
<div class="all">
  <form action="addRoute" method="post" enctype="multipart/form-data">
    <div class="head" > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      您好！欢迎来到 <a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！
     <%	
     		if(session.getAttribute("nameonline")==null) {
     %> 
       <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a>
        <a href="register.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a> 
        <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a> 
        <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
         <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a> </div>
    <%
    		}else{
     %>
     <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;<%=session.getAttribute("nameonline")%>&#93;</span></blink></a>

        <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a> 
        <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
         <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a> </div>
    <%
    		} 
    %>
    <br>
    <br>
    <div class="logo_welcome">
      <div class="logo_welcome_1">
        <div class="logo"><img src="xiaoyaoimg/logo.png" width="100" height="95" alt="logo" /> </div>
        <div class="logotext"><img src="xiaoyaoimg/xianlufabu.png" width="170" height="40" alt="线路发布"></div>
      </div>
    </div>
    
    <!--第一个盒子 --> 
    <br>

    <div class="box1">1. 线路基本信息<a name="topstyle" id="topstyle"></a></div>
    <div class="line"></div>
    <br>
    <div class="box2">
      <p>线路名称：
        <input type="text" class="text" size="20" name="route.routeName" />
      </p>
      <p>线路地点：
        <input type="text" class="text" size="20" name="route.cityType"/>
      </p>
      <p>线路图片：
        <input type="file" class="text" size="20" name="upload" id="upload"/>
      </p>
      <p>线路关键字：
        <input type="text" class="text" size="20" name="route.keyWord"/>
      </p>
      <p>线路库存：
        <input type="text" class="text" size="20" name="route.validAmounts" />
      </p>
      <p>住宿：
        <select id="searchHotelLevelSelect" name="route.stayType">
          <option selected="selected" value="不限">不限</option>
          <option value="五星级/豪华">五星级/豪华</option>
          <option value="四星级/高档">四星级/高档</option>
          <option value="三星级/舒适">三星级/舒适</option>
          <option value="二星级一下/经济">二星级以下/经济</option>
        </select>
      </p>
      <tr>
        <td height="40">天数：
          <input type="text"  size="5" name="route.totalDays"/></td>
      </tr>
      <p> 是否接送：
        &nbsp;
        <input type="radio" name="route.stayRadio" id="button1" value="是" />
        <label for="是"></label>
        &nbsp;是&nbsp;&nbsp;
        <input type="radio" name="route.stayRadio" id="button2" value="否" />
        否</p>
      
        <!--日历 --> 
        <!--日期选择器js --> 
        <!--日期选择器js --> 
        <script type="text/javascript"> 
var gMonths=new Array("一月","二月","三月","四月","五月","六月","七月","八月","九月","十月","十一月","十二月"); 
var WeekDay=new Array("日","一","二","三","四","五","六"); 
var strToday="今天"; 
var strYear="年"; 
var strMonth="月"; 
var strDay="日"; 
var splitChar="-"; 
var startYear=2000; 
var endYear=2050; 
var dayTdHeight=12; 
var dayTdTextSize=12; 
var gcNotCurMonth="#E0E0E0"; 
var gcRestDay="#FF0000"; 
var gcWorkDay="#444444"; 
var gcMouseOver="#79D0FF"; 
var gcMouseOut="#F4F4F4"; 
var gcToday="#444444"; 
var gcTodayMouseOver="#6699FF"; 
var gcTodayMouseOut="#79D0FF"; 
var gdCtrl=new Object(); 
var goSelectTag=new Array(); 
var gdCurDate=new Date(); 
var giYear=gdCurDate.getFullYear(); 
var giMonth=gdCurDate.getMonth()+1; 
var giDay=gdCurDate.getDate(); 
function $(){var elements=new Array();for(var i=0;i<arguments.length;i++) {var element=arguments[i];if(typeof(arguments[i])=='string'){element=document.getElementById(arguments[i]);}if(arguments.length==1){return element;}elements.Push(element);}return elements;} 
Array.prototype.Push=function(){var startLength=this.length;for(var i=0;i<arguments.length;i++){this[startLength+i]=arguments[i];}return this.length;} 
String.prototype.HexToDec=function(){return parseInt(this,16);} 
String.prototype.cleanBlank=function(){return this.isEmpty()?"":this.replace(/\s/g,"");} 
function checkColor(){var color_tmp=(arguments[0]+"").replace(/\s/g,"").toUpperCase();var model_tmp1=arguments[1].toUpperCase();var model_tmp2="rgb("+arguments[1].substring(1,3).HexToDec()+","+arguments[1].substring(1,3).HexToDec()+","+arguments[1].substring(5).HexToDec()+")";model_tmp2=model_tmp2.toUpperCase();if(color_tmp==model_tmp1 ||color_tmp==model_tmp2){return true;}return false;} 
function $V(){return $(arguments[0]).value;} 
function fPopCalendar(evt,popCtrl,dateCtrl){evt.cancelBubble=true;gdCtrl=dateCtrl;fSetYearMon(giYear,giMonth);var point=fGetXY(popCtrl);with($("calendardiv").style){left=point.x+"px";top=(point.y+popCtrl.offsetHeight+1)+"px";visibility='visible';zindex='99';position='absolute';}$("calendardiv").focus();} 
function fSetDate(iYear,iMonth,iDay){var iMonthNew=new String(iMonth);var iDayNew=new String(iDay);if(iMonthNew.length<2){iMonthNew="0"+iMonthNew;}if(iDayNew.length<2){iDayNew="0"+iDayNew;}gdCtrl.value=iYear+splitChar+iMonthNew+splitChar+iDayNew;fHideCalendar();} 
function fHideCalendar(){$("calendardiv").style.visibility="hidden";for(var i=0;i<goSelectTag.length;i++){goSelectTag[i].style.visibility="visible";}goSelectTag.length=0;} 
function fSetSelected(){var iOffset=0;var iYear=parseInt($("tbSelYear").value);var iMonth=parseInt($("tbSelMonth").value);var aCell=$("cellText"+arguments[0]);aCell.bgColor=gcMouseOut;with(aCell){var iDay=parseInt(innerHTML);if(checkColor(style.color,gcNotCurMonth)){iOffset=(innerHTML>10)?-1:1;}iMonth+=iOffset;if(iMonth<1){iYear--;iMonth=12;}else if(iMonth>12){iYear++;iMonth=1;}}fSetDate(iYear,iMonth,iDay);} 
function Point(iX,iY){this.x=iX;this.y=iY;} 
function fBuildCal(iYear,iMonth){var aMonth=new Array();for(var i=1;i<7;i++){aMonth[i]=new Array(i);}var dCalDate=new Date(iYear,iMonth-1,1);var iDayOfFirst=dCalDate.getDay();var iDaysInMonth=new Date(iYear,iMonth,0).getDate();var iOffsetLast=new Date(iYear,iMonth-1,0).getDate()-iDayOfFirst+1;var iDate=1;var iNext=1;for(var d=0;d<7;d++){aMonth[1][d]=(d<iDayOfFirst)?(iOffsetLast+d)*(-1):iDate++;}for(var w=2;w<7;w++){for(var d=0;d<7;d++){aMonth[w][d]=(iDate<=iDaysInMonth)?iDate++:(iNext++)*(-1);}}return aMonth;} 
function fDrawCal(iYear,iMonth,iCellHeight,iDateTextSize){var colorTD=" bgcolor='"+gcMouseOut+"' bordercolor='"+gcMouseOut+"'";var styleTD=" valign='middle' align='center' style='height:"+iCellHeight+"px;font-weight:bolder;font-size:"+iDateTextSize+"px;";var dateCal="";dateCal+="<tr>";for(var i=0;i<7;i++){dateCal+="<td"+colorTD+styleTD+"color:#990099'>"+WeekDay[i]+"</td>";}dateCal+="</tr>";for(var w=1;w<7;w++){dateCal+="<tr>";for(var d=0;d<7;d++){var tmpid=w+""+d;dateCal+="<td"+styleTD+"cursor:pointer;' onclick='fSetSelected("+tmpid+")'>";dateCal+="<span id='cellText"+tmpid+"'></span>";dateCal+="</td>";}dateCal+="</tr>";}return dateCal;} 
function fUpdateCal(iYear,iMonth){var myMonth=fBuildCal(iYear,iMonth);var i=0;for(var w=1;w<7;w++){for(var d=0;d<7;d++){with($("cellText"+w+""+d)){parentNode.bgColor=gcMouseOut;parentNode.borderColor=gcMouseOut;parentNode.onmouseover=function(){this.bgColor=gcMouseOver;};parentNode.onmouseout=function(){this.bgColor=gcMouseOut;};if(myMonth[w][d]<0){style.color=gcNotCurMonth;innerHTML=Math.abs(myMonth[w][d]);}else{style.color=((d==0)||(d==6))?gcRestDay:gcWorkDay;innerHTML=myMonth[w][d];if(iYear==giYear && iMonth==giMonth && myMonth[w][d]==giDay){style.color=gcToday;parentNode.bgColor=gcTodayMouseOut;parentNode.onmouseover=function(){this.bgColor=gcTodayMouseOver;};parentNode.onmouseout=function(){this.bgColor=gcTodayMouseOut;};}}}}}} 
function fSetYearMon(iYear,iMon){$("tbSelMonth").options[iMon-1].selected=true;for(var i=0;i<$("tbSelYear").length;i++){if($("tbSelYear").options[i].value==iYear){$("tbSelYear").options[i].selected=true;}}fUpdateCal(iYear,iMon);} 
function fPrevMonth(){var iMon=$("tbSelMonth").value;var iYear=$("tbSelYear").value;if(--iMon<1){iMon=12;iYear--;}fSetYearMon(iYear,iMon);} 
function fNextMonth(){var iMon=$("tbSelMonth").value;var iYear=$("tbSelYear").value;if(++iMon>12){iMon=1;iYear++;}fSetYearMon(iYear,iMon);} 
function fGetXY(aTag){var oTmp=aTag;var pt=new Point(0,0);do{pt.x+=oTmp.offsetLeft;pt.y+=oTmp.offsetTop;oTmp=oTmp.offsetParent;}while(oTmp.tagName.toUpperCase()!="BODY");return pt;} 
function getDateDiv(){var noSelectForIE="";var noSelectForFireFox="";if(document.all){noSelectForIE="onselectstart='return false;'";}else{noSelectForFireFox="-moz-user-select:none;";}var dateDiv="";dateDiv+="<div id='calendardiv' onclick='event.cancelBubble=true' "+noSelectForIE+" style='"+noSelectForFireFox+"position:absolute;z-index:99;visibility:hidden;border:1px solid #999999;'>";dateDiv+="<table border='0' bgcolor='#E0E0E0' cellpadding='1' cellspacing='1' >";dateDiv+="<tr>";dateDiv+="<td><input type='button' id='PrevMonth' value='<' style='height:20px;width:20px;font-weight:bolder;' onclick='fPrevMonth()'>";dateDiv+="</td><td><select id='tbSelYear' style='border:1px solid;' onchange='fUpdateCal($V(\"tbSelYear\"),$V(\"tbSelMonth\"))'>";for(var i=startYear;i<endYear;i++){dateDiv+="<option value='"+i+"'>"+i+strYear+"</option>";}dateDiv+="</select></td><td>";dateDiv+="<select id='tbSelMonth' style='border:1px solid;' onchange='fUpdateCal($V(\"tbSelYear\"),$V(\"tbSelMonth\"))'>";for(var i=0;i<12;i++){dateDiv+="<option value='"+(i+1)+"'>"+gMonths[i]+"</option>";}dateDiv+="</select></td><td>";dateDiv+="<input type='button' id='NextMonth' value='>' style='height:20px;width:20px;font-weight:bolder;' onclick='fNextMonth()'>";dateDiv+="</td>";dateDiv+="</tr><tr>";dateDiv+="<td align='center' colspan='4'>";dateDiv+="<div style='background-color:#cccccc'><table width='100%' border='0' cellpadding='3' cellspacing='1'>";dateDiv+=fDrawCal(giYear,giMonth,dayTdHeight,dayTdTextSize);dateDiv+="</table></div>";dateDiv+="</td>";dateDiv+="</tr><tr><td align='center' colspan='4' nowrap>";dateDiv+="<span style='cursor:pointer;font-weight:bolder;' onclick='fSetDate(giYear,giMonth,giDay)' onmouseover='this.style.color=\""+gcMouseOver+"\"' onmouseout='this.style.color=\"#000000\"'>"+strToday+":"+giYear+strYear+giMonth+strMonth+giDay+strDay+"</span>";dateDiv+="</tr></tr>";dateDiv+="</table></div>";return dateDiv;} 
with(document){onclick=fHideCalendar;write(getDateDiv());} 
</script> 
        <script type="text/javascript"> 
var gMonths=new Array("一月","二月","三月","四月","五月","六月","七月","八月","九月","十月","十一月","十二月"); 
var WeekDay=new Array("日","一","二","三","四","五","六"); 
var strToday="今天"; 
var strYear="年"; 
var strMonth="月"; 
var strDay="日"; 
var splitChar="-"; 
var startYear=2000; 
var endYear=2050; 
var dayTdHeight=12; 
var dayTdTextSize=12; 
var gcNotCurMonth="#E0E0E0"; 
var gcRestDay="#FF0000"; 
var gcWorkDay="#444444"; 
var gcMouseOver="#79D0FF"; 
var gcMouseOut="#F4F4F4"; 
var gcToday="#444444"; 
var gcTodayMouseOver="#6699FF"; 
var gcTodayMouseOut="#79D0FF"; 
var gdCtrl=new Object(); 
var goSelectTag=new Array(); 
var gdCurDate=new Date(); 
var giYear=gdCurDate.getFullYear(); 
var giMonth=gdCurDate.getMonth()+1; 
var giDay=gdCurDate.getDate(); 
function $(){var elements=new Array();for(var i=0;i<arguments.length;i++) {var element=arguments[i];if(typeof(arguments[i])=='string'){element=document.getElementById(arguments[i]);}if(arguments.length==1){return element;}elements.Push(element);}return elements;} 
Array.prototype.Push=function(){var startLength=this.length;for(var i=0;i<arguments.length;i++){this[startLength+i]=arguments[i];}return this.length;} 
String.prototype.HexToDec=function(){return parseInt(this,16);} 
String.prototype.cleanBlank=function(){return this.isEmpty()?"":this.replace(/\s/g,"");} 
function checkColor(){var color_tmp=(arguments[0]+"").replace(/\s/g,"").toUpperCase();var model_tmp1=arguments[1].toUpperCase();var model_tmp2="rgb("+arguments[1].substring(1,3).HexToDec()+","+arguments[1].substring(1,3).HexToDec()+","+arguments[1].substring(5).HexToDec()+")";model_tmp2=model_tmp2.toUpperCase();if(color_tmp==model_tmp1 ||color_tmp==model_tmp2){return true;}return false;} 
function $V(){return $(arguments[0]).value;} 
function fPopCalendar(evt,popCtrl,dateCtrl){evt.cancelBubble=true;gdCtrl=dateCtrl;fSetYearMon(giYear,giMonth);var point=fGetXY(popCtrl);with($("calendardiv").style){left=point.x+"px";top=(point.y+popCtrl.offsetHeight+1)+"px";visibility='visible';zindex='99';position='absolute';}$("calendardiv").focus();} 
function fSetDate(iYear,iMonth,iDay){var iMonthNew=new String(iMonth);var iDayNew=new String(iDay);if(iMonthNew.length<2){iMonthNew="0"+iMonthNew;}if(iDayNew.length<2){iDayNew="0"+iDayNew;}gdCtrl.value=iYear+splitChar+iMonthNew+splitChar+iDayNew;fHideCalendar();} 
function fHideCalendar(){$("calendardiv").style.visibility="hidden";for(var i=0;i<goSelectTag.length;i++){goSelectTag[i].style.visibility="visible";}goSelectTag.length=0;} 
function fSetSelected(){var iOffset=0;var iYear=parseInt($("tbSelYear").value);var iMonth=parseInt($("tbSelMonth").value);var aCell=$("cellText"+arguments[0]);aCell.bgColor=gcMouseOut;with(aCell){var iDay=parseInt(innerHTML);if(checkColor(style.color,gcNotCurMonth)){iOffset=(innerHTML>10)?-1:1;}iMonth+=iOffset;if(iMonth<1){iYear--;iMonth=12;}else if(iMonth>12){iYear++;iMonth=1;}}fSetDate(iYear,iMonth,iDay);} 
function Point(iX,iY){this.x=iX;this.y=iY;} 
function fBuildCal(iYear,iMonth){var aMonth=new Array();for(var i=1;i<7;i++){aMonth[i]=new Array(i);}var dCalDate=new Date(iYear,iMonth-1,1);var iDayOfFirst=dCalDate.getDay();var iDaysInMonth=new Date(iYear,iMonth,0).getDate();var iOffsetLast=new Date(iYear,iMonth-1,0).getDate()-iDayOfFirst+1;var iDate=1;var iNext=1;for(var d=0;d<7;d++){aMonth[1][d]=(d<iDayOfFirst)?(iOffsetLast+d)*(-1):iDate++;}for(var w=2;w<7;w++){for(var d=0;d<7;d++){aMonth[w][d]=(iDate<=iDaysInMonth)?iDate++:(iNext++)*(-1);}}return aMonth;} 
function fDrawCal(iYear,iMonth,iCellHeight,iDateTextSize){var colorTD=" bgcolor='"+gcMouseOut+"' bordercolor='"+gcMouseOut+"'";var styleTD=" valign='middle' align='center' style='height:"+iCellHeight+"px;font-weight:bolder;font-size:"+iDateTextSize+"px;";var dateCal="";dateCal+="<tr>";for(var i=0;i<7;i++){dateCal+="<td"+colorTD+styleTD+"color:#990099'>"+WeekDay[i]+"</td>";}dateCal+="</tr>";for(var w=1;w<7;w++){dateCal+="<tr>";for(var d=0;d<7;d++){var tmpid=w+""+d;dateCal+="<td"+styleTD+"cursor:pointer;' onclick='fSetSelected("+tmpid+")'>";dateCal+="<span id='cellText"+tmpid+"'></span>";dateCal+="</td>";}dateCal+="</tr>";}return dateCal;} 
function fUpdateCal(iYear,iMonth){var myMonth=fBuildCal(iYear,iMonth);var i=0;for(var w=1;w<7;w++){for(var d=0;d<7;d++){with($("cellText"+w+""+d)){parentNode.bgColor=gcMouseOut;parentNode.borderColor=gcMouseOut;parentNode.onmouseover=function(){this.bgColor=gcMouseOver;};parentNode.onmouseout=function(){this.bgColor=gcMouseOut;};if(myMonth[w][d]<0){style.color=gcNotCurMonth;innerHTML=Math.abs(myMonth[w][d]);}else{style.color=((d==0)||(d==6))?gcRestDay:gcWorkDay;innerHTML=myMonth[w][d];if(iYear==giYear && iMonth==giMonth && myMonth[w][d]==giDay){style.color=gcToday;parentNode.bgColor=gcTodayMouseOut;parentNode.onmouseover=function(){this.bgColor=gcTodayMouseOver;};parentNode.onmouseout=function(){this.bgColor=gcTodayMouseOut;};}}}}}} 
function fSetYearMon(iYear,iMon){$("tbSelMonth").options[iMon-1].selected=true;for(var i=0;i<$("tbSelYear").length;i++){if($("tbSelYear").options[i].value==iYear){$("tbSelYear").options[i].selected=true;}}fUpdateCal(iYear,iMon);} 
function fPrevMonth(){var iMon=$("tbSelMonth").value;var iYear=$("tbSelYear").value;if(--iMon<1){iMon=12;iYear--;}fSetYearMon(iYear,iMon);} 
function fNextMonth(){var iMon=$("tbSelMonth").value;var iYear=$("tbSelYear").value;if(++iMon>12){iMon=1;iYear++;}fSetYearMon(iYear,iMon);} 
function fGetXY(aTag){var oTmp=aTag;var pt=new Point(0,0);do{pt.x+=oTmp.offsetLeft;pt.y+=oTmp.offsetTop;oTmp=oTmp.offsetParent;}while(oTmp.tagName.toUpperCase()!="BODY");return pt;} 
function getDateDiv(){var noSelectForIE="";var noSelectForFireFox="";if(document.all){noSelectForIE="onselectstart='return false;'";}else{noSelectForFireFox="-moz-user-select:none;";}var dateDiv="";dateDiv+="<div id='calendardiv' onclick='event.cancelBubble=true' "+noSelectForIE+" style='"+noSelectForFireFox+"position:absolute;z-index:99;visibility:hidden;border:1px solid #999999;'>";dateDiv+="<table border='0' bgcolor='#E0E0E0' cellpadding='1' cellspacing='1' >";dateDiv+="<tr>";dateDiv+="<td><input type='button' id='PrevMonth' value='<' style='height:20px;width:20px;font-weight:bolder;' onclick='fPrevMonth()'>";dateDiv+="</td><td><select id='tbSelYear' style='border:1px solid;' onchange='fUpdateCal($V(\"tbSelYear\"),$V(\"tbSelMonth\"))'>";for(var i=startYear;i<endYear;i++){dateDiv+="<option value='"+i+"'>"+i+strYear+"</option>";}dateDiv+="</select></td><td>";dateDiv+="<select id='tbSelMonth' style='border:1px solid;' onchange='fUpdateCal($V(\"tbSelYear\"),$V(\"tbSelMonth\"))'>";for(var i=0;i<12;i++){dateDiv+="<option value='"+(i+1)+"'>"+gMonths[i]+"</option>";}dateDiv+="</select></td><td>";dateDiv+="<input type='button' id='NextMonth' value='>' style='height:20px;width:20px;font-weight:bolder;' onclick='fNextMonth()'>";dateDiv+="</td>";dateDiv+="</tr><tr>";dateDiv+="<td align='center' colspan='4'>";dateDiv+="<div style='background-color:#cccccc'><table width='100%' border='0' cellpadding='3' cellspacing='1'>";dateDiv+=fDrawCal(giYear,giMonth,dayTdHeight,dayTdTextSize);dateDiv+="</table></div>";dateDiv+="</td>";dateDiv+="</tr><tr><td align='center' colspan='4' nowrap>";dateDiv+="<span style='cursor:pointer;font-weight:bolder;' onclick='fSetDate(giYear,giMonth,giDay)' onmouseover='this.style.color=\""+gcMouseOver+"\"' onmouseout='this.style.color=\"#000000\"'>"+strToday+":"+giYear+strYear+giMonth+strMonth+giDay+strDay+"</span>";dateDiv+="</tr></tr>";dateDiv+="</table></div>";return dateDiv;} 
with(document){onclick=fHideCalendar;write(getDateDiv());} 
</script>
		<p>报团开始时间：
        <input type="text"  name="route.routeStartTime" style="border:1px solid #999;" onClick="fPopCalendar(event,this,this)" onFocus="this.select()" readonly="readonly" />
        点击选择 </p>
      <p>报团截止时间：
        <input type="text" name="route.routeEndTime"style="border:1px solid #999;" onClick="fPopCalendar(event,this,this)" onFocus="this.select()" readonly="readonly" />
        点击选择 </p>
      <p>发团周期：
        <select id="SelectDatePeriod" name="route.routeCycleTime">
          <option value="每天">每天</option>
          <option value="每周">每周</option>
          <option value="每月">每月</option>
        </select>
      </p>
      <p>价格：
        <input type="text" name="route.marketPrice"class="text" size="20" />
      </p>
      <p>交通方式:
        <input type="text" name="route.transType" class="text" size="20" />
      </p>
      <div class="box1">2.其他信息</div>
      <div class="line"></div>
      <div class="box4">
        <p>优惠信息:
          <input type="text" name="route.discountInfo" class="text" size="20" />
        </p>
        <p>线路详细描述：</p>
        <p>
          <label for="details"></label>
          <textarea name="route.routeDetailInfo" id="details3" cols="70" rows="10"></textarea>
        </p>
        <p>报名须知</p>
        <p>
          <label for="details4"></label>
          <textarea name="route.needToknow" id="details" cols="70" rows="10"></textarea>
        </p>
        <p>其他线路：</p>
        <p>
          <label for="details6"></label>
          <textarea name="route.otherRoutes" id="details4" cols="70" rows="10"></textarea>
        </p>
        <p>
          <input type="submit" name="sumbit" id="submit" value="提交" />
          <input type="reset" name="clear" id="clear" value="清空" />
          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#top" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image6','','xiaoyaoimg/dingjihuidingbu2.png',1)"><img src="xiaoyaoimg/dianjhuidingb.png" name="Image6" width="130" height="28" border="0"></a></p>
      </div>
      <p>&nbsp; </p>
    </div>
    <p>&nbsp;</p>
  
 
</div>

 </center>
 </form>
</body>
</html>
