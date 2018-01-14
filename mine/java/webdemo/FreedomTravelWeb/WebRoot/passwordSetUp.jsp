<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=gbk">
		<title>逍遥游，满意的选择</title>
		<meta name="Keywords" content="登录界面">
		<meta name="description" content="">
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
       
        .zone{width:1200px;height:540px;margin:10px auto;}  
			  .lan{width:190px;height:800px;float:left}
			  .person{width:190px;height:180px;}
			  .tree{width:190px;height:700px;margin:20px 0 0 0}
		   a{text-decoration: underline;}
       a:link {color: #595989;}
       a:visited {color: #595989;} 
       a:hover{color: #ff0000;} 
       a:active {color: #595989;} 
       .dt1, .dt2, .dt3, .dt4 ,.dt{padding: 0px; margin: 0px; border: 0px; padding-left: 25px;border-left: 5px solid #a7cbff; 
           border-top: 1px solid #ffffff; border-bottom: 1px solid #c0c0c0;width: auto;text-align: left;line-height: 26px;
           height: 26px;background: #E0E0E0;cursor:pointer!important;cursor:hand;display:block;}
       #aboutbox {/*左侧box*/padding: 0px; margin: 0px; border: 0px;width: 190px; 
           float: left;background: #eee;}
 			 #aboutbox dl {  /*dl、dt、dd*/margin: 0px; border: 0px;border: medium none;  /*不显示边框*/
						background:#eeeeee;background-image: url();/*背景图像，这里省略了*/background-repeat: repeat-y;
						background-position: left;clear: both;}
			 #aboutbox dd {padding: 0px; margin: 0px; border: 0px;background: #eee;
						border-top: 1px solid #fff;border-left: 0px solid #ffffff;padding-right:3px solid red; }
			 #aboutbox ul {  /*ul、li定义*/padding: 0px;margin: 0px; border: 0px;list-style-type: none; }
			 #aboutbox li {padding: 0px; margin: 0px; border: 0px;text-align: left;
						text-indent: 10px;list-style:none;}
			 #aboutbox li a {padding-left: 5px; margin: 0px; border: 0px;display: block;background:#fff;
						font-weight: normal;height: 22px;line-height: 22px;color: #000;border: 1px solid #eee;text-decoration: none;}
			 #aboutbox li a:link,#aboutbox li a:visited {height: 22px;line-height: 22px;}
			 #aboutbox li a:hover {padding-left: 5px;background-color:#e4e4e4;border: 1px solid #999999;
						color: #D90000;height: 22px;line-height: 22px;}
			 #aboutbox li a:active {color: #333333;height: 22px;line-height: 22px;background: #EEEEEE;}
      .center_tdbgall /* 中部表格背景颜色 */{background:#ffffff;}	
      .dt1{background-color: #a7cbff;background-repeat: no-repeat;background-position: right center;}
      .dt2{background-color: #a7cbff;background-repeat: no-repeat;background-position: right center;}
      .dt3{background-color: #a7cbff;background-repeat: no-repeat;background-position: right center;}
      .dt4{background-color: #a7cbff;background-repeat: no-repeat;background-position: right center;}
 
			 .content{width:700px;height:700px;margin:20px 0 0 100px;float:left;}
     
       .parts{width:640px;height:385px;border-bottom:1px solid #cecece;padding-top: 40px ;}
       #need {margin: 20px auto 0;width: 500px;}
       #need li {height: 46px;width: 500px;font: 16px/26px "微软雅黑"; sans-serif;background: #FFD;border-bottom: 1px dashed #E0E0E0;display: block;cursor: text;padding: 7px 0px 7px 10px!important;padding: 15px 0px 15px 10px;}
			 #need li:hover,#need li.hover {background: #FFE8E8;}
			 #need input {line-height: 20px;background: #FFF;height: 20px;width: 200px;border: 1px solid #E0E0E0;vertical-align: middle;padding: 6px;}
			 #need label {background: url(../images/icon_sprite.gif) no-repeat;padding-left: 30px;}
			 #need label.old_password {background-position: 0 -277px;}
			 #need label.new_password {background-position: 0 -1576px;}
			 #need label.rePassword {background-position: 0 -1638px;}
			 #need label.email {background-position: 0 -429px;}
			 #need dfn {display: none;}
			 #need li:hover dfn, #need li.hover dfn {display:inline;margin-left: 7px;color: #676767;}

      	.btn{width:120px;height:45px;margin:25px 0 0 100px;; }
			  .user_login{width:120px;height:32px;font-weight:bold;font-size:16px;color:#ffffff;background:url('xiaoyaoimg/ybtn.png');no-repeat 0 0 ;cursor:pointer;overflow:hidden;border:none;}
			  .user_login:hover{background:url('xiaoyaoimg/ybtn.png') no-repeat 0 -34px;} 
</style>


	
	
	<script>	
function showsubmenu(sid){
    whichEl = eval('submenu' + sid);
    if (whichEl.style.display == 'none'){
        eval("submenu" + sid + ".style.display='';");
        eval("dt" + sid + ".className='dt2';");
    }
    else{
        eval("submenu" + sid + ".style.display='none';");
        eval("dt" + sid + ".className='dt1';");
    }
}
function showbg(sid){
    whichEl = eval('submenu' + sid);
    if (whichEl.style.display == 'none'){
        eval("dt" + sid + ".className='dt4';");
    }
    else{
        eval("dt" + sid + ".className='dt3';");
    }
}
function showoutbg(sid){
    whichEl = eval('submenu' + sid);
    if (whichEl.style.display == 'none'){
        eval("dt" + sid + ".className='dt1';");
    }
    else{
        eval("dt" + sid + ".className='dt2';");
    }

</script>	

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
  	 <div class="zone">
  	 	<div class="lan">
  	 		<div class="person">
  	 				<img src="xiaoyaoimg/tou.png" alt="best" width="150" height="117" style="padding:15px 0 10px 19px" />
  	 				<br>
  	 				<a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">
  	 					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;编辑资料</a>
  	 		</div>
  	 		<div class="tree">
  	 			<table class=center_tdbgall width="191"  border="0" cellspacing="0" cellpadding="0">
 					<tr>
 					 <td width=191 rowspan="3" valign=top class=web_left_all>
						<div id=aboutbox>
							<dl>
        				<dt class=dt1 id=dt1 onmouseover=showbg(1) onclick=showsubmenu(1) onmouseout=showoutbg(1)>
        					<B>网站动态</B>
            			<dd id=submenu1>
                		<ul>
                			<LI><A href='/'>热门线路</A></LI>
                			<LI><A href='/'>最新发布</A></LI>
                		</ul>
           				</dd>
       					</dt>
     					</dl>
		
       			  <dl>
       			 		<dt class=dt2 id=dt2 onmouseover=showbg(2) onclick=showsubmenu(2) onmouseout=showoutbg(2)>
       			 			<B>个人信息</B>
            			<dd id=submenu2>
                		<ul>
                			<LI><A href='/'>我的订单</A></LI>
                			<LI><A href='/'>我的购物车</A></LI>
               			  <LI><A href='/'>我的收藏</A></LI>
               			  <LI><A href='/'>我的发布</A></LI>
               			  <LI><A href='/'>账户管理</A></LI>
                		</ul>
            			</dd>
        				</dt>
        			</dl>
							<dl>
       					 <dt class=dt3 id=dt3 onmouseover=showbg(3) onclick=showsubmenu(3) onmouseout=showoutbg(3)>
       					 	<B>旅行社中心</B>
            			<dd id=submenu3>
                		<ul>
                			<LI><A href='/'>发布路线</A></LI>
                			<LI><A href='/'>路线管理</A></LI>
               		 </ul>
            			</dd>
        				 <dt></dt>
       				 </dl>

 								
      			</div>
     			 </td>
					</tr>
					</table>	
  	 		</div>
  	 		
  	 		
  	 	</div>
  	 	<form action="passwordEdit.action" method="post">
  	 	<div class="content">
  	 		<div id="profile" class="sns-config">
						<div id="" class="parts">
					     <ol id="need">
                 <li><label class="old_password">原始密码：</label> <input name="userDTO.originalPassword" type="password" id="" /></li>
  							 <li><label class="new_password">新的密码：</label> <input name="userDTO.newPassword" type="password" id="" />
  							 	<br><dfn> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（密码长度为6~20字节。不想修改请留空）</dfn></li>
  							 <li><label class="rePassword">重复密码：</label> <input name="userDTO.confirmPassword" type="password" id="" /></li>
                 <li><label class="email">邮箱设置：</label> <input name="userDTO.email" type="text" id="" />
                 	<br><dfn> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（ 承诺绝不会给您发送任何垃圾邮件。）</dfn></li>
               </ol>  
			      </div>
								<div class="btn" >
									<input type="submit" value="保&nbsp;存" class="user_login"/>
								</div>
		    </div>
  	 		
  	 	</div>
  	 	</form>
  	 </div>	
 </body>
</html>

