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
		<meta name="Keywords" content="注册界面">
		<meta name="description" content="">
		
		<style type="text/css">
			*{margin:0px;padding:0px;}
			  .head{width:1150px;height:30px;position:relative;top:10px;left:110px;font-size:14px;}
				.top{width:350px;height:120px;position:relative;top:1px;left:110px}
				.top_left{width:170px;height:120px;float:left;border-right:2px solid #e8e8e8;}
				.top_right{width:170px;height:42px;float:right;margin:35px auto;}		
				.signinf{width:1100px;height:1250px;border:1px solid;border-color:#e8e8e8;border-width:2px;position:relative;border-radius:
					3px;background:#00000;top:10px;left:110px}
				.word{width:600px;height:6px;float:left;position:relative;top:50px;left:60px;font-size:18px}
				.line{width:600px;height:6px;float:left;position:relative;top:65px;left:60px}
				.part_one{width:500px;height:305px;position:relative;top:80px;left:100px;}
			  .user{width:490px;height:26px;position:relative;top:25px;line-height:26px;left:10px;font-size:20px;}
			  .left_right{width:110px;height:30px;float:left;}
			  .user_left{width:100px;height:30px;float:right;}
				.user_right{width:350px;height:26px;line-height:26px;float:right;}
				.read{width:600px;height:26px;position:relative;top:80px;line-height:26px;left:240px;font-size:20px;}
			  .bnt{width:400px;height:52px;position:relative;top:80px;line-height:26px;left:240px;font-size:30px;}			
    </style>
	</head>
	<script src="js/registerCheck.js" charset="utf-8" type="text/javascript"></script>
  <body>
  	  <div class="head">&ldquo;&rdquo;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 您好！欢迎来到 
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！
		       <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a>			
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a>
			 </div>
  		
  		
			   <div class="top" >
				    <div class="top_left">
				      <img src="xiaoyaoimg/logo.png" alt="逍遥游" width="170" height="120" />
			     	</div>
			   		<div class="top_right">
				     	<img src="xiaoyaoimg/weltosign.png" alt="欢迎注册" width="170" height="42" />
				   	</div>
			   </div>
			  
			   <form action="saveUser" method="post" >
			   <div class="signinf">
			   	   <div class="word">
			   	       <span style="color:#004a80">1.设置您的账户信息</span>	
			       </div>
			   	   <div class="line">
			   	       <hr width="750px" size="1" color="#004a80"/>
			       </div>
			       <br>
			      
			       <div class="part_one">
			       		<div class="user">
			       			<div class="left_right">
			       				<div class="user_left" >
			             			&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">用户名:</span>
			          			</div>
			        		</div>
			        	
			          	<div class="user_right" >
			   	         	<input type="text" id="username" name="user.username"
			   	         	onblur="checkUsername()" 
			   	         	class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"/><span id="usernameMsg"></span>
			          	</div>
			       </div>
			       <br>
			       <div class="user">
			       		<div class="left_right">
			          		<div class="user_left" >
			             		&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">密码:</span>
			          		</div>
			        	</div>
			        
			        	<div class="user_right" >
			   	         	<input type="password" id="password" name="user.password" 
			   	         	class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         	onblur="checkPassword()"/><span id="passwordMsg"></span>
			          	</div>
			       </div>
			       <br>
			       <div class="user">
			       		<div class="left_right">
			          		<div class="user_left" >
			             		<span style="color:red">&#42</span><span style="color:black">确认密码:</span>
			          		</div>
			        	</div>
			        	<div class="user_right" >
			   	         	<input type="password" id="confirmpassword"
			   	         	class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         	onblur="checkConfirmpassword()"/><span id="confirmpasswordMsg"></span>
			          	</div>
			      </div>
			       <br>
			       <div class="user">
			       		<div class="left_right">
			          		<div class="user_left" >
			             		&nbsp;&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">Email:</span>
			          		</div>
			        	</div>
			          	<div class="user_right" >
			   	         	<input type="text" id="email" name="user.email"
			   	         	class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         	onblur="checkEmail()"/><span id="emailMsg"></span>
			          	</div>
			        </div>
			        <br>
			        <div class="user">
			       		<div class="left_right">
			          		<div class="user_left" >
			             		<span style="color:red">&#42</span><span style="color:black">会员类型:</span>
			          		</div>
			        	</div>
			        	<div class="user_right" >
			          		<div style="float:left">
			   	         		<select id="membershipstyle" name="user.membershipstyle"
			   	         		style="width:150px;font-size:20px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         		>
			   	         			<option value="普通会员" selected="selected">普通会员</option>
			   	         			<option value="VIP">VIP</option>
			   	         		</select>	   	         
			            	</div>
			         	</div>
			         </div>
			        <br>
			        <br>
			        <br>
         </div>
          <br>
			       <br>
			       <br>
         
         
         
         
           <div class="word">
			   	       <span style="color:#004a80">2.姓名和联系方式</span>	
			       </div>
			   	   <div class="line">
			   	       <hr width="750px" size="1" color="#004a80"/>
			       </div>
			       <br>
			       <div class="part_one">
			       <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             <span style="color:red">&#42</span><span style="color:black">真实姓名:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="realname" name="user.realname"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         onblur="checkRealname()"/><span id="realnameMsg"></span>	
			          </div>
			       </div>
			       <br>
			         <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">性别:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="radio" id="sex" name="user.sex" value="女" class="user_in" checked="checked"/>男
			   	          <input type="radio" id="sex" name="user.sex" value="女" class="user_in" />女	
			          </div>
			       </div>
			       <br>
			      <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:black">职位:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="postion" name="user.postion"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"/>
			          </div>
			      </div>
			       <br>
			        <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             <span style="color:black">固定电话:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="fixedphonenum" name="user.fixedphonenum"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"/>
			          </div>
			        </div>
			       <br>
			      <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">手机:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="cellphone" name="user.cellphone" 
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         onblur="checkCellphone()"/><span id="cellphoneMsg"></span>
			          </div>
			      </div>
			       <br>
			       <br>
			       <br>
			       <br>
         </div>
          <br>
			       <br>
			       <br>
         
         
         
         
         
           <div class="word">
			   	       <span style="color:#004a80">3.公司名称和主营业务</span>	
			       </div>
			   	   <div class="line">
			   	       <hr width="750px" size="1" color="#004a80"/>
			       </div>
			       <br>
			       <div class="part_one">
			       <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             <span style="color:red">&#42</span><span style="color:black">公司名称:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="companyname" name="user.companyname"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         onblur="checkCompanyname()"/><span id="companynameMsg"></span>
			          </div>
			       </div>
			       <br>
			        <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             <span style="color:red">&#42</span><span style="color:black">所在省份:</span>
			          </div>
			        </div>
			         <div class="user_right" >
			          	<div style="float:left">
			   	         <select id="localcity" name="user.localcity"
			   	         style="width:100px;font-size:20px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	        >
			   	        <option value="shanghai" selected="selected" > 上海</option>
			   	         	<option value="北京">北京</option>
			   	         	<option value="天津">天津</option>
			   	         	<option value="河北">河北</option>
			   	         	<option value="山西">山西</option>
			   	         	<option value="内蒙古">内蒙古</option>
			   	         	<option value="辽宁">辽宁</option>
			   	         	<option value="吉林">吉林</option>
			   	         	<option value="黑龙江">黑龙江</option>
			   	         	<option value="江苏">江苏</option>
			   	         	<option value="浙江">浙江</option>
			   	         	<option value="安徽">安徽</option>
			   	         	<option value="福建">福建</option>
			   	         	<option value="江西">江西</option>
			   	         	<option value="山东">山东</option>
			   	         	<option value="河南">河南</option>
			   	         	<option value="湖北">湖北</option>
			   	         	<option value="湖南">湖南</option>
			   	         	<option value="广东">广东</option>
			   	         	<option value="广西">广西</option>
			   	         	<option value="海南">海南</option>
			   	         	<option value="重庆">重庆</option>
			   	         	<option value="四川">四川</option>
			   	         	<option value="贵州">贵州</option>
			   	         	<option value="云南">云南</option>
			   	         	<option value="西藏">西藏</option>
			   	         	<option value="陕西">陕西</option>
			   	         	<option value="甘肃">甘肃</option>
			   	         	<option value="青海">青海</option>
			   	         	<option value="宁夏">宁夏</option>
			   	         	<option value="新疆">新疆</option>
			   	         	<option value="香港">香港</option>
			   	         	<option value="台湾">台湾</option>
			   	         	<option value="澳门">澳门</option>
			   	         </select>
			   	        	   	         
			            </div>
			            </div>
			         </div>
			         <br>
			         <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">地址:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="address" name="user.address"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         onblur="checkAddress()"/><span id="addressMsg"></span>
			          </div>
			       </div>
			       <br>
			      <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			             <span style="color:red">&#42</span><span style="color:black">主营业务:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" id="mainbussiness" name="user.mainbussiness"
			   	         class="user_in" style="width:350px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"
			   	         onblur="checkMainbussiness()"/><span id="mainbussinessMsg"></span>
			          </div>
			      </div>
			       <br>
			        <div class="user">
			       	<div class="left_right">
			          <div class="user_left" >
			            &nbsp;&nbsp;<span style="color:red">&#42</span><span style="color:black">验证码:</span>
			          </div>
			        </div>
			          <div class="user_right" >
			   	         <input type="text" class="user_in"  size="6"  name="code" style="width:80px;height:26px;line-height:26px;border:1px solid;border-color:#d7d7d7;border-width:1px;"/>
			   	         &nbsp;&nbsp;<img border=0 title="点击刷新" name="valimg" id="valimg" src="image.jsp" style="width:80px;height:50px;line-height:50px;vertical-align:middle">
					  <span id="codeMsg"></span>
			          </div>
			        </div>
			       <br>  
			       </div>
			       <br>
			       <br>
			       <br>
			       
			  
			   <div class="line">
			   	       <hr width="750px" size="2" style="border:1px solid #004a80"/>
			   </div>
			   <br>
			   <div class="read">
			   	       <input type="checkbox" name="read" value="read" checked="checked">我已阅读并接受
			   	       <a href="#" style="color:#0072bc;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color=''"><blink>&#8243逍遥游会员服务条款&#8243</blink></a>
			   </div>
			   <br>
			   <div class="bnt">
			   	      <input type="submit" value="注 &nbsp;&nbsp;册" style="width:150px;height:40px;font-size:25px;color:#ffffff;
			   	      background:#f7941d;border:none;border-radius:8px">
			   	       &nbsp;&nbsp;&nbsp;&nbsp;
			   	      <input type="reset" value="重 &nbsp;&nbsp;置" style="width:150px;height:40px;font-size:25px;color:#ffffff;
			   	      background:#f7941d;border:none;border-radius:8px"> 
			   </div>
	       	</form> 
  </body>
</html>