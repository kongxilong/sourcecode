<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<base href="<%=basePath%>">
    
    <title>逍遥游线路搜索</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">

<style type="text/css">
.phone {
	background-color: #FFF;
	height: 20px;
	width: 40px;
}
.box1 {
	background-color: #FFF;
	margin: auto;
	height: 30px;
	width: 1000px;
}
.btn input{
	height: 24px;
	width: 80px;
	color: #FFF;
	background-image: url(images/sousuo.jpg);
}
.btn input:hover{
	background-image: url(images/sousuo2.jpg);
	height: 24px;
	width: 80px;
}

.bottom {
	margin: auto;
	height: 35px;
	width: 200px;
}
.box1 .usersearch {
	background-color: #FFF;
	margin: auto;
	height: 20px;
	width: 1000px;
	clip: rect(auto,auto,auto,auto);
}
.box2 {
	margin: auto;
	height: 80px;
	width: 1000px;
	padding: 3px;
	background-color: #ECFFFF;
}
.box2_1 {
	height: 80px;
	width: 333px;
	background-color: #E8FFFF;
	float:left;
}
.box2_2 {
	background-color: #ECFFFF;
	height: 80px;
	width: 333px;
	float:left;
}
.box2_3 {
	background-color: #ECFFFF;
	height: 80px;
	width: 333px;
	float:right;
}
.box3 {
	height: 30px;
	width: 1000px;
	background-color: #ECFFFF;
	margin-top: -1px;
	margin-right: auto;
	margin-bottom: auto;
	margin-left: auto;
}
.boxtop {
	height: 100px;
	width: 1000px;
	background-color: #FFF;
	margin: auto;
	border-bottom: solid;
	border-bottom-color: #F00;
}
.boxtop_1 {
	height: 100px;
	width: 500px;
	float: left;
}
.boxtop_2 {
	float: right;
	height: 30px;
	width: 500px;
	margin-top: 60px;
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


<body onload="MM_preloadImages('xiaoyaoimg/shouye2.jpg','xiaoyaoimg/xiangzuo2.jpg','xiaoyaoimg/xiaoyou2.jpg','xiaoyaoimg/weiye2.jpg')">
<center>
<div class="boxtop">
  <div class="boxtop_1"><img src="xiaoyaoimg/logo.png" width="135" height="95" alt="images" />
  
  
  </div>
  <div class="boxtop_2"><img src="xiaoyaoimg/kefurexian.png" alt="kefurexian" width="252" height="30" /></div>
 </div>
<table width="1006" border="0" align="center">
  <tr>
    <td width="494" ><p>根据您的选择，找到<a href="#"style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><s:property value="page.getTotalCount()"/></a>条线路  </p></td>
    <td width="502"><p>当前位置&gt;<a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">首页</a>&gt;<a href="#"style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">搜索</a></p></td>
  </tr>
</table>
<div class="box1">
  <input name="search" type="text" class="usersearch" value="当前搜索关键词" />
</div>
<br/>
<div class="box2">
  <div class="box2_1">目的地
    <input type="text" class="text" size="20" />
    <br/>
    <br/>
    线&nbsp;路&nbsp;
    <input type="text" class="text" size="20" />
  </div>
  <div class="box2_2"> 日期
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
<input type="text" style="border:1px solid #999;" onclick="fPopCalendar(event,this,this)" onfocus="this.select()" readonly="readonly" /> 
    <br/>
    <br/>
    酒店级别
    <select id="searchHotelLevelSelect" name="Star">
      <option selected="selected" value="0">不限</option>
      <option value="5">五星级/豪华</option>
      <option value="4">四星级/高档</option>
      <option value="3">三星级/舒适</option>
      <option value="2">二星级以下/经济</option>
    </select>
  </div>
  <div class="box2_3"> 价格
    <select id="searchPricelSelect" name="Price">
      <option selected="selected" value="0">不限</option>
      <option value="5">2000元以下</option>
      <option value="4">2000-10000元</option>
      <option value="3">10000-20000元</option>
      <option value="2">2万元以上</option>
    </select>
    <br/>
    <br/>
    交通
    <select id="searchTransportSelect" name="Transport">
      <option selected="selected" value="0">不限</option>
      <option value="5">汽车</option>
      <option value="4">火车</option>
      <option value="3">飞机</option>
      <option value="2">轮船</option>
    </select>
  </div>
</div>
<div class="box3">关键字
<form action="searchKeywords" method="post"> 
<input type="text" name="searchWords" style="width:400px" size="30"/>
<span class="btn"><input  type=submit name="submit" value="搜索" style="font-size:20px" onMouseOver="this.style.color='red'"/></span>
</form> 
<div class="btn">
  <!--  <input type="button" value="搜 索"  />
 --></div>
</div>
<br/>
</center>
<table width="1007" border="1"  bordercolor="#979797"align="center" cellpadding="1" cellspacing="0" >
  <tr align="center" bgcolor=#ECFFFF>
    <td width="30">编号</td>
    <td width="68">线路名称</td>
    <td width="68">景点介绍</td>
    <td width="69">商家</td>
    <td width="70">住宿</td>
    <td width="80">交通</td>
    <td width="80">天数</td>
    <td width="89">发团周期</td>
    <td width="89">截止时间</td>
    <td width="89">市场价格</td>
    <td width="89">数量</td>
    <td width="89">详细</td>
    <td width="89">预定</td>
    <td width="89">加收藏夹</td>
  </tr>
   <s:iterator value="list" id="route" status = "i"><!-- list -->
  <tr>
   	<td><s:property value="#i.index+1"/></td>
   	<td><s:property value="#route.routeName"/></td>
   	<td><s:property value="#route.keyWord"/></td>		
   	<td><s:property value="#route.username"/></td>
   	<td><s:property value="#route.stayType"/></td>
   	<td><s:property value="#route.transType"/></td>
   	<td><s:property value="#route.totalDays"/></td>
   	<td><s:property value="#route.routeCycleTime"/></td>
   	<td><s:property value="#route.routeEndTime"/></td>
   	<td><s:property value="#route.marketPrice"/></td>
   	<td><s:property value="#route.validAmounts"/></td>
   	
   	<td><s:a href="routeDetail.action?route.routeId=%{#route.routeId}">详细</s:a></td>
   	<td>
   		<%if(session.getAttribute("nameonline") == null){ %>
   		<s:a href="login.jsp">预定</s:a>
   		<%}else{ %>
   		<s:a href="addToDeal.action?route.routeId=%{#route.routeId}">预定</s:a>
   		<%} %>
   	</td>
   	<td>
   		<%if(session.getAttribute("nameonline") == null){ %>
   		<s:a href="login.jsp">加收藏夹</s:a>
   		<%}else{ %>
   		<s:a href="addToCollect.action?route.routeId=%{#route.routeId}">加收藏夹</s:a>
   		<%} %>
   	</td>
  </tr>
  </s:iterator>
  <TR>
    <TD height=24 colspan="10" align="center">
    第<s:property value="page.currentPage"/>页/
	总<s:property value="page.totalPage"/>页
    
	
    <s:if test="page.hasPrePage">
		<s:url var="firstPage" action="searchKeywords.action">
		<s:param name="page.currentPage" value="1"></s:param>
		</s:url>
		<s:a href="%{firstPage}">首页</s:a>
		<s:url var="prePage" action="searchKeywords.action">
		<s:param name="page.currentPage" value="page.currentPage-1"></s:param>
		</s:url>
		<s:a href="%{prePage}">上一页</s:a>
		<!-- <a href="searchKeywords.action?page.currentPage=1?searchWords=session.getAttribute("searchWords")">首页</a> -->
		<!-- <a href="searchKeywords.action?page.currentPage=%{#page.currentPage - 1}?searchWords=session.getAttribute("searchWords")">上一页</a> -->
	</s:if>
	<s:else>
		<EM>首页</EM>
		<EM>上一页</EM>
	</s:else>
	<s:if test="page.hasNextPage">
		<s:url var="nextPage" action="searchKeywords.action">
		<s:param name="page.currentPage" value="page.currentPage+1"></s:param>
		</s:url>
		<s:a href="%{nextPage}">下一页</s:a>
		<s:url var="lastPage" action="searchKeywords.action">
		<s:param name="page.currentPage" value="page.totalPage"></s:param>
		</s:url>
		<s:a href="%{lastPage}">尾页</s:a>
	</s:if>
	<s:else>
		<EM>下一页</EM>
		<EM>尾页</EM>
	</s:else>
    </TD>
  </TR>
</table>
<p>&nbsp;</p>
<center>
<div class="bottom"><a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image7','','xiaoyaoimg/shouye2.jpg',1)"><img src="xiaoyaoimg/shouye.jpg" name="Image7" width="46" height="28" border="0" id="Image7" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image4','','xiaoyaoimg/xiangzuo2.jpg',1)"><img src="xiaoyaoimg/xiangzuo.jpg" name="Image4" width="29" height="28" border="0" id="Image4" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image5','','xiaoyaoimg/xiaoyou2.jpg',1)"><img src="xiaoyaoimg/xiaoyou.jpg" name="Image5" width="29" height="28" border="0" id="Image5" /></a>&nbsp;&nbsp;<a href="#a" onmouseout="MM_swapImgRestore()" onmouseover="MM_swapImage('Image6','','xiaoyaoimg/weiye2.jpg',1)"><img src="xiaoyaoimg/weiye.jpg" name="Image6" width="44" height="28" border="0" id="Image6" /></a></div>
</center>
</body>
</html>
