<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<base href="<%=basePath %>">
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>逍遥游，满意的选择</title>
		<meta http-equiv="pragma" content="no-cache">  
		<meta http-equiv="cache-control" content="no-cache"> 
		<meta name="Keywords" content="登录界面">
		<meta name="description" content="">
	
		<style type="text/css">
			*{margin:0px;padding:0px;}
				.top{width:350px;height:120px;position:relative;top:10px;left:110px}
				.top_left{width:170px;height:120px;float:left;border-right:2px solid #e8e8e8;}
				.top_right{width:170px;height:42px;float:right;margin:35px auto;}
				.login{width:1000px;height:380px;border:1px solid;margin:0 auto;border:1px solid;border-color:#e8e8e8;border-width:8px;position:relative;border-radius:
					3px;background:#00000;margin:70px;left:50px}
				.login_left{width:480px;height:380px;float:left}
			  .login_right{width:480px;height:380px;float:right;font-size:14px;color:#999999}
			  .login_c{width:320px;height:280px;float:right;position:absolute;top:70px;right:80px}
			  .user{width:320px;height:64px;}
			  .user_text{width:320px;}
			  .user_in{width:307px;height:34px;line-height:28px;font-size:20px;color:#cccccc;border:1px solid}
			  .userauto{width:320px;height:30px}
			  .bnt{width:320px;height:52px;margin-top:40px}
			  .user_login{width:307px;height:34px;font-weight:bold;font-size:20px;color:#ffffff;background:url('xiaoyaoimg/dlbnt.png');no-repeat 0 0 ;cursor:pointer;overflow:hidden;border:none;}
			  .user_login:hover{background:url('xiaoyaoimg/dlbnt.png') no-repeat 0 -34px;} 
			  
    </style>
    
    <script type="text/javascript"> 
		function trim(str){ //删除左右两端的空格 
			return str.replace(/(^\s*)|(\s*$)/g, ""); 
		} 
	</script>
    <script type="text/javascript">
		function ajaxLogin()
		{
			var xmlhttp;
			var text;
		 	var username=document.getElementById("username").value;
		 	var password=document.getElementById("password").value;
		 	//检测是否为空可以将其交给前台处理;
		 	if(username.length==0){
		 		alert("用户名不能为空");
		 		return false;
		 	}
		 	else if(password.length==0){
		 		alert("密码不能为空");
		 		return false;
		 	}
		}
	</script>  

	</head>
		<body>
		
		<form action="loginUser" method="post">
			<!--盒子 层-->
			<div class="top">
				 <div class="top_left">
				   <img src="xiaoyaoimg/logo.png" alt="逍遥游" width="170" height="120" />
			   </div>
			   <div class="top_right">
				   <img src="xiaoyaoimg/welcome.png" alt="欢迎登录" width="170" height="42" />
				 </div>
			</div>
			
			<div class="login">
				
			  <div class="login_left">
			    <img src="xiaoyaoimg/login_left.png" alt="登录图片" width="480" height="380"/>
			  </div>
			  
			  <div class="login_right">
			     <div class="login_c">
			     	
			     	     <div class="user">
			            <span>用户名</span>
			              <div class="user_text">
			              	<input type="text" id ="username" name="user.username" class="user_in " />
			              </div>
			           </div>
			           
			           <div class="user">
			            <span>密码</span>
			              <div class="user_text">
			              	<input type="password" id="password" name="user.password" class="user_in"/>
			              </div>
			           </div>
			  
			           <div class="userauto">
			              	<input type="checkbox" name="autologin"/><span>自动登录</span>&nbsp;&nbsp;
			              	<a href="#" style="color:#0072bc;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color=''"><blink>忘记密码</blink></a>
			              	&nbsp;&nbsp;<span>还没有</span>
			              	<a href="#" style="color:#0072bc;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color=''"><blink>注册</blink></a>
			              	<span>吗？</span>
			           </div>
			           
			           <div class="btn">
			              	<input type="submit" value="登 &nbsp;&nbsp;录" onclick="ajaxLogin()" class="user_login"/>
			           </div>
			           
			           <div class="" style="width:350px;height:30px;position:absolute;left:4px;bottom:26px;line_height:8px">
			              	<a href="#" style="color:#0072bc;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color=''"><blink>使用合作账号登录 &gt;&gt;</blink></a> 
			           </div>
			     </div>
			  </div>
			</div>
			
			<div style="width:350px;height:70px;position:absolute;left:4px;bottom:26px;">
			</div>
		</form>
	  </body>
</html>
