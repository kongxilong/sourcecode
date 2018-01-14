<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xtml1-transitional.dtd">

<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>逍遥游，满意的选择</title>
		<meta name="Keywords" content="登录界面">
		<meta name="description" content="">
		<style type="text/css">
			*{margin:0px;padding:0px;}
		  .head{width:100%;background:#f7f7f7;height:30px;margin:0px auto;font-size:16px;font-family:"微软雅黑";}
		  .top{width:1205px;height:300px;margin:0px auto;font-size:16px;font-family:"微软雅黑";
		   	     border-bottom:5px solid #004a80;}
		  .left{width:400px;height:200px;margin:60px 0 0 50px;font-size:16px;font-family:"微软雅黑";float:left;}
			.top_left{width:170px;height:120px;float:left;border-right:2px solid #e8e8e8;}
			.top_right{width:189px;height:120px;float:left;padding:45px 0 0 0}	
		  .right{width:650px;height:290px;margin:10px 0 0px 70px ;font-size:16px;font-family:"微软雅黑";float:left;}
			.right h1, h2, h3, h4, h5, h6 { font-size:1em; }
			.right a { color:#0287CA; text-decoration:none; }
			.right a:hover { text-decoration:underline; }
			.right ul, li { list-style:none; }
			.right fieldset, img { border:none; }
			.right legend { display:none; }
			.right em, strong, cite, th { font-style:normal; font-weight:normal; }
			.right input, textarea, select, button { font:12px Helvetica, Arial, sans-serif; }
			.right table { border-collapse:collapse; }
			.right html { overflow:-moz-scrollbars-vertical; } 
     	#ibanner { position:relative; width:650px; height:250px; overflow:hidden; }
     	#ibanner_pic {}
     	#ibanner_pic a { position:absolute; top:0; display:block; width:650px; height:250px; overflow:hidden; }
     	#ibanner_btn { position:absolute; z-index:9999; right:5px; bottom:5px; font-weight:700; font-family:Arial; }
     	#ibanner_btn span { display:block; float:left; margin-left:4px; padding:0 5px; background:#000; cursor:pointer; }
     	#ibanner_btn .normal { height:20px; margin-top:8px; border:1px solid #999; color:#999; font-size:16px; line-height:20px; }
     	#ibanner_btn .current { height:28px; border:1px solid #FF5300; color:#FF5300; font-size:28px; line-height:28px; }
		  .lan{width:1130px;height:428px;margin:20px auto;font-size:16px;font-family:"微软雅黑";}
		  .cart-table-th{width:1130px;height:30px;background: #e2f5f5;padding-top: 4px;overflow: hidden;display: block;
										-webkit-border-radius: 3px;border-radius: 3px;margin-bottom: 20px;font-size: 12px;color: #696969;
										border-top:3px solid #a7cbff;}
		  .wp{width:1130px;height:30px;margin: 0 auto;line-height: 30px;}
		  .th-chk{width:60px;height:30px;float:left;text-align: left}
		  .select-all{width:59px;height:30px;float:left;cursor: pointer;margin-top: 2px}
		  .s-checkbox{display: inline-block;width: 15px;height: 15px;background-position: 0 -31px;line-height:15px;vertical-align: middle;cursor: pointer;}
		  .th-item{width:290px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-info{width:161px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-price{width:131px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-amount{width:111px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-sum{width:131px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-zhuangtai{width:110px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .th-op{width:110px;height:30px;float:left;border-left: 1px solid #fff;text-align: center;display: block;}
		  .shoplist{width:1130px;height:400px;}
		  .shopone{width:1130px;height:123px;}
		  .shop-info{width:980px;height:20px;font:12px;color: #696969;}
		  .icon{display: inline-block;vertical-align: middle;margin-right: 6px;}
		  .content{width:1130px;height:95px;margin:8px 0 0 0;background: #e2f5f5;list-style: none;font-size: 12px;color: #696969;}
		  .td-chk{width:59px;height:70px;text-align: center;float:left; }
		  .inner{width:59px;height:70px;padding-top: 18px; }
		  .td-item{width:290px;height:90px;text-align:center;float:left;}
		  .item-pic{width: 80px;height: 80px;float:left;overflow: hidden;padding:6px 6px 0 0 }
			.item-info{width:190px;height:65px;text-align:left;float:left;}
			.jsc{width:69px;height:25px;margin:67px 0 0 206px;}
			.td-info{width:160px;height:90px;text-align:left;float:left;}
			.item-props{width:155px;height:85px;padding:3px 0 0 5px;}
			.td-price{width:130px;height:90px;text-align:center;line-height:90px;float:left;}
		  .td-amount{width:110px;height:90px;text-align:center;line-height:90px;float:left;}
			  .amount {width:110px;height:25px;margin:0 0 20px 0;}
        .amount a { display: inline-block;width: 9px;height: 9px;line-height: 9px;background: white;
         text-decoration: none;text-align: center;color: #555;border: 1px solid #ccc;}
        .amount a:hover { border-color: #ffffff;}
				.disabled,
				.disabled:hover {color: #ccc;border-color: #ccc;}
				#note {position: relative;width: 68px; margin-top: 6px; padding: 4px; line-height: 16px; border: 1px solid #FEC3A8; -webkit-border-radius: 2px;-moz-border-radius: 2px; -ms-border-radius: 2px; -o-border-radius: 2px; border-radius: 2px;background: white;}
				#note b,
				#note i {position: absolute;left: 30px;width: 18px; height: 7px;font: 16px/21px Simsun;overflow: hidden;}
				#note b {top: -8px;color: #FEC3A8;}
				#note i {top: -7px;color: white;}
				#note em {font-size: 12px;font-style: normal;color: #F40;}
				.hide {display: none;}
				.show { display: block;}
			.td-total{width:135px;height:90px;text-align:center;line-height:90px;float:left;}
			.td-zhuangtai{width:130px;height:90px;text-align:center;line-height:20px;float:left;padding:20px 0 0 5px}
			.item-zhuangtai{width:110px;height:85px;padding:3px 0 0 0px;}
			.caozuo{width:110px;height:90px;text-align:center;line-height:70px;float:left;}
		
		  .xiaobutton{width:55px;height:25px;margin:0px 0 0px 0px;}
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




			 </head>	
  <body>
  	 <div class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 您好！欢迎来到 
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>逍遥游</span></blink></a>！
		     <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;登录&#93;</span></blink></a>			
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;注册&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的订单&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的购物车&#93;</span></blink></a>
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a>
		 </div>
		 <div class="top">
		 	 <div class="left">
		 	 	<div class="top_left">
				    <img src="xiaoyaoimg/logo.png" alt="逍遥游" width="170" height="120" />
			     </div>
			     <div class="top_right">
				      <img src="xiaoyaoimg/dingdan.png" alt="欢迎来到" width="189" height="47" />
				   </div>
		 	 	
		 	 </div>
		 	
		 	 <div class="right">
		 	 	<div id="ibanner">
					<div id="ibanner_pic">
						    <a href="/jscss/"><img src="xiaoyaoimg/one.jpg" alt="" width="670" height=280"/></a>
							<a href="/sort/index.shtml"><img src="xiaoyaoimg/two.jpg" alt="" width="670" height="280"/></a>
							<a href="/other/lastupdate.shtml"><img src="xiaoyaoimg/three.jpg" alt="" width="670" height="280"/></a>
							<a href="/other/top100.shtml"><img src="xiaoyaoimg/four.jpg" alt="" width="670" height="280" /></a>
					</div>
				</div>
				<div style="height:20px; background:#EEE;"></div>
		 	 	
		 	 </div>
		 </div>
		 <div class="lan">
		 	<div class="cart-table-th">
                        <div class="wp">
                            <div class="th-chk">
                            	<div id="J_SelectAll1" class="select-all">
                            		
                            		<span class="s-checkbox"><input type="checkbox" onClick="checkAll(this)"></span>&nbsp;全选
                            	</div>
                            </div>
                            <div class="th-item">商品</div>
                            <div class="th-info">商品信息</div>
                            <div class="th-price">单价</div>
                            <div class="th-amount">数量</div>
                            <div class="th-sum">小计</div>
                            <div class="th-zhuangtai">交易状态</div>
                            <div class="th-op">操作</div>
                        </div>           
       </div>
       
       
       <div class="shoplist">
       <s:iterator value="dealList" id="dealTemp">
     	<form name=checkboxform>  
       	<div class=shopone>
       		<div class="shop-info">
       			<span class="s-checkbox"></span><input type="checkbox" name=C1 />
       			&nbsp;&nbsp;<span class="icon"></span>
       			卖家：<a href="#" target="_blank" title="用户昵称" class="J_MakePoint" data-point="tbcart.8.15"
       				style="color:#696969;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#696969'">
       				<s:property value="#dealTemp.username"/></a>
       		</div>
       		   <ul class="content"> 
       		   	<li class="td-chk">
       		   		 <div class="inner">
       		   		 	<span class="s-checkbox "><input type="checkbox" name="C2" /></span>
       		   		 </div> 
       		   	</li>
       		   	
       		   	<li class="td-item"> 
       		   		  <div class="item-pic">
       		   		    <a href="#" ><img src=<s:property value="#dealTemp.imageDir"/> width="78" height="78" style="border: 1px solid #ccc;" ></a>
       		   			</div> 
       		   			<div class="item-info"> 
       		   				<a href="#" style="color:#696969;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#696969'">
       		   					<s:property value="#dealTemp.routeName"/></a>  
       		   		  </div>
       		   		  <div class="jsc">
       		   		  	<a href="addToCollect.action?route.routeId=<s:property value="#dealTemp.routeId"/>" onclick="addFavorite();" style="margin:0 2px 0 0px;" >
       		   		  		<img src="xiaoyaoimg/sc.png" alt="" /></a>
       		   		  </div>       		   		
       		   	</li>
       		   	<li class="td-info"> 
       		   		<div class="item-props">
       		   			<p class="sku-line">行程信息：<s:property value="#dealTemp.totalDays"/></p>
       		   			<p class="sku-line">途经景点：<s:property value="#dealTemp.keyWord"/></p>
       		   			<p class="sku-line">订单号：<s:property value="#dealTemp.dealId"/></p>
       		   		</div> 
       		   	</li>
       		   	<li class="td-price"> 
       		   		<div class="item-price"><s:property value="#dealTemp.marketPrice"/></div> 
       		   	</li>
       		   	<li class="td-amount"> 
       		   		 <div class="amount"> 
                            	  <p><s:property value="#dealTemp.routeNum"/></p>
								 </div>
       		   	</li>
       		   	<li class="td-total"> 
       		   		<div class="item-price"><s:property value="#dealTemp.totalPrice"/></div> 
       		   	</li>
       		   	<li class="td-zhuangtai"> 
       		   		<div class="item-zhuangtai">
       		   			<s:if test='#dealTemp.dealState.equals("0")'>
       		   			<a href="submitDeal.action?deal.dealId=<s:property value="#dealTemp.dealId"/>">等待付款</a>
       		   			</s:if>
       		   			<s:else>
       		   			<p class="line">已付款</p>
       		   			</s:else>
       		   			<p class="line"><a href="routeDetail.action?route.routeId=<s:property value="#dealTemp.routeId"/>">订单详情</a></p>
       		   		</div> 
       		   	</li>
       		   	<li class="caozuo"> 	
       		   			<div class="xiaobutton">
		   							<div class="xiaobnt" style="width:55px;height:25px;padding:0px 0 0 25px;">
		   							<s:if test='#dealTemp.dealOver.equals("0")'>
		   	 						<a href="dealConfirm.action?deal.dealId=<s:property value="#dealTemp.dealId"/>">正在交易</a>
			  						</s:if>
			  						<s:else>
			  						<p class="line"><a href="openComments.action?route.routeId=<s:property value="#dealTemp.routeId"/>">评价</a></p>
			  						</s:else>
			  						</div>
			  						<br>
		   						</div>
       		   			<p class="line" style="margin:5px 0 5px 0px;color:#696969;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='#696969'"><a href="deleteDeal.action?deal.dealId=<s:property value="#dealTemp.dealId"/>">
       		   					删除</a></p> 	
       		   	</li>
       	     </ul>
        </div>
        </form>
       </s:iterator>
       </div> 
       
        
		 	<script type="text/javascript">
   			function Id(o){
        return document.getElementById(o);
					}
				var $minus=Id('minus'),
        $plus=Id('plus'),
        $text=Id('text'),
        $note=Id('note'),
        maxNum=$text.getAttribute('data-max'),
        minNum=$text.getAttribute('data-min');
        
				//显示提示信息
				function showNote(note,$a){
        var emNode=$note.getElementsByTagName("em")[0];
        $note.className='show';         
        emNode.innerText=note;
        $a.className='disabled';
				}
				//隐藏提示信息
				function hideNote(){
        $note.className='hide';
        $minus.className=$plus.className='';
				}                
				//减少数量        
				$minus.onclick=function(){
        var val=$text.value*1;
        if(val>minNum){
                $text.value=val-1;
                hideNote();
        }
        if(val==minNum+1){
                showNote('不能少于'+minNum,this);
        }
				}
				//添加数量
				$plus.onclick=function(){
        var val=$text.value*1;
        if(val<maxNum){
                $text.value=val+1;
                hideNote();
        }
        if(val==maxNum-1){
                showNote('最大不能超过'+maxNum,this);
        }
				}
				//手动更改数量
				$text.onkeyup=function(){
        var _this=this, 
                val=this.value=_this.value.replace(/[^\d]/g,'');
        if(val<=minNum){
                $text.value=minNum;
                showNote('不能少于'+minNum,$minus);
                $plus.className='';
        }else if(val>=maxNum){
                $text.value=maxNum;
                showNote('最大不能超过'+maxNum,$plus);
                $minus.className='';
        }else{
                hideNote();
        }
				}
      </script>
		   
		
		  
		  

<script type="text/javascript">
function checkAll(vi) {
	if (vi.checked){
for (var j = 1; j <= 9; j++) {
box = eval("document.checkboxform.C" + j); 
if (box.checked == false) box.checked = true;
   }
 }
 else
 	{
 		for (var j = 1; j <= 9; j++) {
		box = eval("document.checkboxform.C" + j); 
		if (box.checked == true) box.checked = false;
   }
 	}
}
</script>
		 
	</body>
</html>
