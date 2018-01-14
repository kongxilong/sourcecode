<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
<title>我的收藏夹</title>
<style>
.head{width:100%;background:#f7f7f7;height:30px;margin:0px auto;font-size:16px;font-family:"微软雅黑";}
body{font-size:14px; font-family:"\5B8B\4F53",san-serif;line-height:21px;text-align:left;}
body,div,ul,li,h1,h2,h3{padding:0;margin:0;}
ul, li{list-style:none;}
.clear{clear:both;}
.sidebar {background: #3B4053;border:1px solid #e6ebf1;height:50px;width:190px;padding:5px;margin:10px 10px;float:left}
.sidebar h2{color:#fff;font-size:14px;line-height:21px;text-align:center;padding:2px 0 7px;}
#menu {width:190px;background: #e6ebf1;padding:8px 0;}
#menu li{float:left;width:166px;display:block;padding-left:24px;background: #e6ebf1;position:relative;line-height:21px;border:none;}
#menu li:hover {background: #3B4053;}
#menu li a {font-family:"\5B8B\4F53",san-serif;font-size:14px;color:#3B4053;display:block;outline:0;text-decoration:none;padding:5px 0;}
#menu li:hover a {color:#fff;}
.dropdown_4columns{	margin:6px auto;float:left;position:absolute;left:-999em;text-align:left;border-left:6px solid #3B4053; border-top:1px solid #3B4053;border-bottom:1px solid #3B4053;border-right:1px solid #3B4053;width:510px;background: #e6ebf1;padding:5px 0 10px;}
#menu li:hover .dropdown_4columns{left:190px;top:-6px;}
.col_1 {width:120px;display:inline;float: left;position: relative;margin:0;}
.col_4 {width:510px;display:inline;float: left;position: relative;margin:0;}
#menu h3 {font-size:14px;font-weight:bold;color:#3B4053;margin:4px 0;padding-left:20px;font-family:"\5B8B\4F53",san-serif;line-height:21px;font-size:12px;text-align:center;}
#menu li:hover div a {font-size:12px;color:#003366;}
#menu li:hover div a:hover {color:#e47911;}
#menu li ul {list-style:none;padding:0;margin:0;}
#menu li ul li {font-size:12px;line-height:21px;position:relative;padding:0 0 0 6px;margin:0;text-align:left;width:120px; border-left:#ccc 1px solid;}
#menu li ul li a{padding:0 0 0 6px;margin:0;}
#menu li ul li:hover {background:none;padding:0 0 0 6px;margin:0;}
.top{width:1100px;height:150px;margin:0px auto;display: block;border:1px solid;border-color:#ffffff;border-width:1px;}
.tupian{width:800px;height:150px;margin:0px 0 0 30px;float: left;}
input{padding:0;margin:0;border:none;background:none;}
.box{border:solid 2px #F00;height:30px;width:220px;margin:155px 0 0 0px;line-height:30px;font-size:14px;}
.text{width:180px;height:100%;float:left;}
.btnSubmit{width:40px;height:100%;background:#F00;color:#fff;float:left;}
.xian{width:1205px;height:5px;margin:50px 0 0 78px; auto;border-top:5px solid #004a80;}
.fav{height: 30px;line-height: 30px;letter-spacing: normal;border: 0;padding-bottom: 10px;position: relative;
			width: 1200px;margin: 0 auto;}
.con-fixed-box{width:1200px;height:27px;}
.luxiantu{width:190px;height:210px;margin:0px 0px 0px 0px;}
.fav-list{width:1200px;height:1300px;margin:2px 0 0 78px;}
.list{width:1200px;height:1300px;margin:2px 0 0 78px;}
.onexian{width:195px;height:310px;padding:0px 0px 0 0px;border:5px solid #eee;margin:0 8px 12px 8px;float:left}
.one{width:195px;height:280px;background:#eee;margin:0 8px 12px 0px;}
.item-controller{width:190px;height:23px;text-align: center;line-height: 20px;color: #eee;}
.price-container {width:190px;text-align: center;display: block;margin-bottom: 10px;height: 18px;overflow: hidden;}
.g_price{width:180px;text-align: center;display: block;}
.item-controller a {display: inline-block;zoom: 1;letter-spacing: normal;word-spacing: normal;width: 16px;height:16px;
	margin-right: 10px;overflow: hidden;color:#ffffff;text-decoration: none;font-size: 14px;display: inline-block;
	-display: inline;margin: 0;padding: 0 5px;vertical-align: middle;}
</style>
</head>
<body>
	<div class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 您好！欢迎来到  
			   <a href="main.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！
		     <a href="login.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a>			
			   <a href="register.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a>
			   <a href="mydeal.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a>
			   <a href="gouwuche.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
			   <a href="routeCollect.jsp" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a>
		 </div>
    <div class="top">
		  <div class="sidebar">
			<h2>线路目录</h2>
			<ul id="menu">
				<li><a href="/">国内路线</a>
        <div class="dropdown_4columns">
            <div class="col_1">
                <h3>热门线路</h3>
                <ul>
                    <li><a href="/">海南两日游</a></li>
                    <li><a href="/">南京一日游</a></li>
                    <li><a href="/">苏州五日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>价格区间</h3>
                <ul>
                    <li><a href="/">100-300元</a></li>
                    <li><a href="/">300-500元</a></li>
                    <li><a href="/">500-1000元</a></li>
                    <li><a href="/">1000元以上</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>最新发布</h3>
                <ul>
                    <li><a href="/">海南岛岛两日游</a></li>
                    <li><a href="/">陕西</a></li>
                    <li><a href="/">青岛三日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>推荐旅行社</h3>
                <ul>
                    <li><a href="#">春风旅行社</a></li>
                    <li><a href="#">中华旅行社</a></li>
                    <li><a href="#">乐乐旅行社</a></li>
                    <li><a href="#">玩乐旅行社</a></li>
                </ul>
            </div>
        </div>
  			</li>
				<li><a href="/">国外路线</a>
         <div class="dropdown_4columns">
            <div class="col_1">
                <h3>热门线路</h3>
                <ul>
                   <li><a href="/">海南两日游</a></li>
                    <li><a href="/">南京一日游</a></li>
                    <li><a href="/">苏州五日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>价格区间</h3>
                <ul>
                    <li><a href="/">100-1000元</a></li>
                    <li><a href="/">1000-5000元</a></li>
                    <li><a href="/">5000-10000元</a></li>
                    <li><a href="/">10000元以上</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>最新发布</h3>
                <ul>
                     <li><a href="/">巴厘岛两日游</a></li>
                    <li><a href="/">夏威夷两日游</a></li>
                    <li><a href="/">纽约三日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>推荐旅行社</h3>
                <ul>
                    <li><a href="#">春风旅行社</a></li>
                    <li><a href="#">中华旅行社</a></li>
                    <li><a href="#">乐乐旅行社</a></li>
                    <li><a href="#">玩乐旅行社</a></li>
                </ul>
            </div>
        </div>
  			</li>
				<li><a href="#">周边路线</a>
         <div class="dropdown_4columns">
            <div class="col_1">
                <h3>热门线路</h3>
                <ul>
                   <li><a href="/">海南两日游</a></li>
                    <li><a href="/">南京一日游</a></li>
                    <li><a href="/">苏州五日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>价格区间</h3>
                <ul>
                    <li><a href="/">100-300元</a></li>
                    <li><a href="/">300-500元</a></li>
                    <li><a href="/">500-1000元</a></li>
                    <li><a href="/">1000元以上</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>最新发布</h3>
                <ul>
                    <li><a href="/">巴厘岛两日游</a></li>
                    <li><a href="/">夏威夷两日游</a></li>
                    <li><a href="/">纽约三日游</a></li>
                </ul>
            </div>
            <div class="col_1">
                <h3>推荐旅行社</h3>
                <ul>
                    <li><a href="#">春风旅行社</a></li>
                    <li><a href="#">中华旅行社</a></li>
                    <li><a href="#">乐乐旅行社</a></li>
                    <li><a href="#">玩乐旅行社</a></li>
                </ul>
            </div>
        </div>
  			</li>
			</ul>
	  	</div>
		  <div class="tupian">
			  <img class="tu" src="xiaoyaoimg/hai.png" width="800" height="183" style="padding:0px 0 0px 0px;">
		</div>
		 <div class="box">
	     <input type="text" class="text"/>
       <input type="submit" value="搜索"  class="btnSubmit"/>
    </div>
    </div>
    <div class="xian">
		 </div>
		<div class="fav">
		 <div class="con-fixed-box">
				<input type="checkbox" value="1" class="select-all" id="select-all" >
				<label class="quanxuan" >全选</label>
				&nbsp;&nbsp;&nbsp;
				<img class="laji" src="xiaoyaoimg/laji.png" width="16" height="16" style="margin:4px -6px 0px 0px;">
				<a class="delete" href="#" style="color:black;text-decoration:none" 
					onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">删除</a>
 	 			<a class="page-pre " href="#" style="float:right;color:black;text-decoration:none" 
 	 				onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">下一页</a>
 	 				&nbsp;&nbsp;&nbsp;
				<a class="page-next" href="#" style="float:right;color:black;text-decoration:none" 
					onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">上一页	&nbsp;&nbsp;</a>
				
			</div>     
		</div>
		
		<div id="fav-list">
        <ul class="list">  
						<li class="onexian" >
							    <div class="one">
                    <div class="img-controller-box-t">
              				 <a href="#" class="img" >
                			<img class="luxiantu" src="xiaoyaoimg/four.jpg"></a>
       						</div>
    							  <div class="img-item-title">
       								 	 <input class="itemselect" type="checkbox" />
                    					 <a href="#" style="font-size:14px;color:#333;text-decoration:none" 
					                 	  onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#333'"> 
					                        苏州两日游啦啦啦啦啦啦啦阿苏苏州两日游啦啦啦啦阿</a>
    				</div>
                    <div class="price-box">
                         	<div class="g_price">
                              	<span>￥</span>
           					  	<strong>69.00</strong> 
							</div>
        			</div>
        					  <div class="item-controller">
        									<a class="shanchu" href="#" title="删除"  >
        										<img class="luxiantu" src="xiaoyaoimg/laji.png" style="width:16px;height:16px;float:left"   ></a>|
       					  					<a class="dianpu" href="#" title="进入旅行社"  >
       					  						<img class="luxiantu" src="xiaoyaoimg/dian.png" style="width:16px;height:16px;float:left" ></a>
                |							<a class="addtocar"  title="加入购物车" href="#" >
                	       						<img class="luxiantu" src="xiaoyaoimg/che.png"  style="width:16px;height:16px;float:left" ></a> 
                    		  </div>
                  	</div>
						</li>
				<li class="onexian" >
				<div class="one">
                    <div class="img-controller-box-t">
              				 <a href="#" class="img" >
                				<img class="luxiantu" src="xiaoyaoimg/four.jpg" >
            					 </a>
       				</div>
    							  <div class="img-item-title">
       								  <input class="itemselect" type="checkbox" >
                    					<a href="#" style="font-size:14px;color:#333;text-decoration:none" 
					                 	onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#333'"> 
					                 	苏州两日游啦啦啦啦啦啦啦阿苏苏州两日游啦啦啦啦阿</a>
    							  </div>
                    		<div class="price-box">
                         			<div class="g_price">
                              			<span>￥</span>
           					  			<strong>69.00</strong> 
									</div>
        					</div>
        					<div class="item-controller">
        									<a class="shanchu" href="#" title="删除"  >
        										<img class="luxiantu" src="xiaoyaoimg/laji.png" style="width:16px;height:16px;float:left"   ></a>|
       					  					<a class="dianpu" href="#" title="进入旅行社"  >
       					  						<img class="luxiantu" src="xiaoyaoimg/dian.png" style="width:16px;height:16px;float:left" ></a>
                |							<a class="addtocar"  title="加入购物车" href="#" >
                	       						<img class="luxiantu" src="xiaoyaoimg/che.png"  style="width:16px;height:16px;float:left" ></a> 
                    		 </div>
                  	</div>
						</li>
						
						
						
				 <s:iterator value="listTemp" id="collectTemp">		
				<li class="onexian" >
				<div class="one">
                    <div class="img-controller-box-t">
              				 <a href="#" class="img" >
                				<img class="luxiantu" src= "<s:property value="#collectTemp.getImageDir()"/>" /></a>
       				</div>
    							  <div class="img-item-title">
       								  <input class="itemselect" type="checkbox"/>
                    					<a href="#" style="font-size:14px;color:#333;text-decoration:none" 
					                 	onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#333'"> 
					                 	<s:property value="#collectTemp.getRouteName()"/></a>
    							  </div>
                    		<div class="price-box">
                         			<div class="g_price">
                              			<span>￥</span>
           					  			<strong><s:property value="#collectTemp.getMarketPrice()"/></strong> 
									</div>
        					</div>
        					<div class="item-controller">
        									<a class="shanchu" href="#" title="删除"  >
        										<img class="luxiantu" src="xiaoyaoimg/laji.png" style="width:16px;height:16px;float:left"   ></a>|
       					  					<a class="dianpu" href="#" title="进入旅行社"  >
       					  						<img class="luxiantu" src="xiaoyaoimg/dian.png" style="width:16px;height:16px;float:left" ></a>
                |							<a class="addtocar"  title="加入购物车" href="#" >
                	       						<img class="luxiantu" src="xiaoyaoimg/che.png"  style="width:16px;height:16px;float:left" ></a> 
                    		 </div>
                  	</div>
					</li>
					</s:iterator>	
						
						
				</ul>	
    </div>
</body>
</html>

