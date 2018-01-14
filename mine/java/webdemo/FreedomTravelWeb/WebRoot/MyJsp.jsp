<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<% 
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'MyJsp.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
       <center ><h1 color="blue">订单信息如下：</h1></center>
       <s:iterator value="list" id="route" ><!-- list -->
  	<tr>
   	<td><s:property value="#route.routeId"/></td>
   	<td><s:property value="#route.routeName"/></td>
   	<td><s:property value="#route.keyWord"/></td>		
   	<td><s:property value="#route.username"/></td>
   	<td><s:property value="#route.stayType"/></td>
   	<td><s:property value="#route.transType"/></td>
   	<td><s:property value="#route.totalDays"/></td>
   	<td><s:property value="#route.routeCycleTime"/></td>
   	<td><s:property value="#route.routeEndTime"/></td>
   	<td><s:property value="#route.marketPrice"/></td>
   	<td><img src="<s:property value="#route.imageDir"/> " width="75" height="75"/></td>
   	<td><br></td>
   	</tr>
  	</s:iterator>
  </body>
</html>
