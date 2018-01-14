<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
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
			  .main{width: 1100px;height:1700px;margin: 0 auto;}
			   div{margin: 0;padding: 0;font-family: "Microsoft Yahei";color: #333;display: block;border:1px solid;border-color:#ffffff;border-width:1px;}
			  .line-d h2{width: 1100px;height:50px;line-height: 30px;padding: 13px 0 5px 20px;font-size: 18px;color: #12609C;font-weight: normal;}
			   .header {width:1100px;height:280px;margin:5px 0 15px;}
			  .l{width: 678px;height:280px;font-family: "Microsoft Yahei";color: #333;background: #E8F6FC;float:left;}
			  .l .ld {width:658px;height:147px;border-bottom:2px solid #ffffff;padding:0px 10px;}
			  .l .ld li span {display: inline-block;width: 64px;color: #159; }
			  .l .ld li {width: 198px;height: 30px;line-height: 30px;padding:0 0 0 5px;font-size: 14px;color: #333;float: left;
			  	          display: list-item;text-align: -webkit-match-parent;font-family: "Microsoft Yahei";list-style: none;}
			  .l .ld li.t {width: 664px;padding:0 0 5px 5px;width: auto;float: none;clear: both;}
			  .l .ld li .c{width: 658px;height:0px}
			  .sp{width:240px;height:128px;border-right:2px solid #ffffff;font-family: "Microsoft Yahei";color: #333;float: left;}
			  .submit4{width:24px;height:17px;}
	      .sp .la {width:220px;height:100px;font-size: 14px;color: #333;padding:19px 10px;}
			  .sp .la li.d {width: 225px;padding:5px 0 5px 5px;width: auto;float: none;clear: both;list-style: none;border:1px solid;border-color:#E8F6FC;border-width:1px;}
			  .sp .la li.b {width: 193px;height:31px;padding:2px 0 2px 10px;float: none;clear: both;list-style: none;border:1px solid;border-color:#e8e8e8;border-width:1px;}
			  .article{width:432px;height:128px;float: left;font-family: "Microsoft Yahei";color: #333;}
			  .buy{width:327px;height:100px;margin:10px 0 0 0;padding:15px 0 0 10px;float:right;font-family: "Microsoft Yahei";color: #E8F6FC;border:1px solid;border-color:#E8F6FC;border-width:1px;}
			  .amount {width:120px;height:25px;margin:0 0 20px 0；border:1px solid;border-color:#E8F6FC;border-width:1px;}
        .amount a { display: inline-block;width: 9px;height: 9px;line-height: 9px;background: white;
         text-decoration: none;text-align: center;color: #555;border: 1px solid #ccc;}
        .amount a:hover { border-color: red;}
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
			  
			  .j{width:160px;height:30px;margin:20px 0 0 0;padding:2px 0 0 2px;float:right;font-family: "Microsoft Yahei";color: #E8F6FC;border:1px solid;border-color:#E8F6FC;}
		    .liucheng{width:1090px;height:54px;font-family: "Microsoft Yahei";margin:290px 0 0 0;color: #333;}	
		    .date{width:416px;height:283px;font-family: "Microsoft Yahei";color: #333;float: left;border:1px solid;border-color:#ffffff;border-width:1px;background: #E8F6FC;}
		    .kuang{width:295px;height:28px;font-family: "Microsoft Yahei";border:1px solid;border-color:#ffffff;border-width:1px;color: #333;padding:0 0 0 10px;float: left;background: #ffffff;}
		    .tu{background: #ffffff;border:1px solid;border-color:#ffffff;border-width:1px;}
			 	.box{width:1095px;height:1042px;font-family: "Microsoft Yahei";color: #333;}
				#tablit {margin:30px;width:1041px; height:1200px;border:#64B8E4 1px solid;padding-top:10px;background:#E4F2FB;}
				#tablit dl{ float:left; width:1041px;}
				#tablit dl dt{float:left;border-bottom:#64B8E4 1px solid; width:15px; height:31px; line-height:30px;}
				#tablit dl dd{float:left;width:149px;height:30px; line-height:30px; text-align:center;}
				#tablit .on{border:#64B8E4 1px solid;border-bottom:#FFF 1px solid; color:#FF6600;background:#FFF;}
				#tablit .out{border:#64B8E4 1px solid; color:#000;background:#64B8E4;}
				.tabcon{width:1040px height:365px; background: #FFF; clear:both;}
				.dis{display:none;}
			  .one{display:none;}
			  .two{display:none;}
			  .three{display:none;}
			  .four{display:none;}
		</style>
		<link type="text/css" rel="Stylesheet" href="./datePicker.css" />
     <script type="text/javascript"  src="DatePicker_config.js"></script>
     <script type="text/javascript" src="DatePicker.js"></script>
     <script type="text/javascript" >
  function testDatePicker()
  {
        setInputControl("txtDate");
        if(datePicker !=null)
        {
            datePicker.close();
            datePicker =null;
           }
         else
         {
                if(inputCtrl.value!="")
                {
                    var arr=checkInputDate(inputCtrl.value);
                    if(arr==null)
                    {
                       alert("输入有误");
                        return ;
                    }
                    datePicker=new Ecalendar(arr[0],arr[1]-1,arr[2]);
                }
                else
                {
                    var d=new Date();
                    datePicker=new Ecalendar(d.getFullYear(),d.getMonth(),d.getDate());
                }
                datePicker.setDisplayYearRange(24);
                var b= datePicker.showDate();

                if(b==0)
                {
                  
                }
                else
                     alert("输入有误2");
                  
          }
       
  }
  function getToday()
  {
        setInputControl( "txtDate");
        var cal=new Ecalendar(2009,7,26);
        inputCtrl.value=cal.today();
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
			   <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'"><blink><span>&#91;我的收藏&#93;</span></blink></a>
		 </div>
		 
		
		 <div class="main">
      		
        		<h2 >旅行派致青春（2天帆船体验）</h2>
    					<div class="header">
        				<div class="l">
                	<ul class="ld" >
                    <li ><span>线路编号</span>151565</li>
                    <li ><span style="display: inline-block;width: 64px;color: #159;">出发地</span>上海</li>
                    <li ><span>行程天数</span>2天</li>
                    <li ><span>报团截止</span>发团前0天</li>
                    <li ><span>住宿标准</span>-</li>
                    <li><span>是否接送</span>无</li><i class="c"></i>
                    <li class="t"><span>交通方式&nbsp;&nbsp;</span>其它</li>
                    <li class="t"><span>途经景点&nbsp;&nbsp;</span>
                    <a href="#" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'" >三亚湾</a>&nbsp;</li>
                    <i class="c"></i>
                  </ul>
                  <div class="sp">
                  	<ul class="la">
                			<li class="d">海南金太阳国际旅行社有限公..</li>
                  		<li class="d">联系方式：请 
                  		<a href="#" class="thickbox" style="color:black;text-decoration:none" onMouseOver="this.style.color='red'" onMouseOut="this.style.color='black'">登录</a> 查看
                 		 	</li>
                 		 	<li class="b">
                  		<a class="submit4" href="#";>
                  		<img src="xiaoyaoimg/shangjia.png" alt="" /></a>
                  		</li>
                    </ul>
                  </div>
                  <div class="article">
                	     <img class="twocode" src="xiaoyaoimg/wei.png" width="78" height="78" style="padding:15px 0 2px 5px;">
                            <div class="buy">
                            	  <div class="amount"> 
                            	  	<span>数量：</span><a href="#" id="minus">-</a>
       													 <input type="text" id="text" value="1" data-max='5' data-min='0' 
       													   style="display: inline-block;width: 28px;padding: 1px 3px;text-align: right;color: #666;">
       														 <a href="#" id="plus">+</a>
        														<p id="note" class="hide"> <b>◆</b> <i>◆</i> <em></em> </p>
																</div>
																 <a href="#" title="请先登录后使用！" class="sub-buy"><img src="xiaoyaoimg/yuding.png" alt="" /></a>
													 				
													 			<div class="j" >
                        					<a href="#" title="请先登录后使用！" class="sub-mail thickbox no" style="margin:0 2px 0 0;" ><i></i><img src="xiaoyaoimg/fyj.png" alt="" /></a>
                       						<a href="#" onclick="addFavorite();" class="sub-fav" style="margin:0 2px 0 0px;"><i></i><img src="xiaoyaoimg/sc.png" alt="" /></a>
                       					</div>
																
                   					 </div>
                  </div>
                </div>
                <div class="date">
                	<div class="kuang">
                			<input type="text" name="inpCalendar" id="txtDate"  style="width:280px;height:25px;font-size:20px;border:2px solid;border-color:black;border-width:2px;"/>
                	</div>
                	<div class="tu">
                  		<a href="#" onclick="testDatePicker()" title="日历选择器" accesskey="C" style="margin:5px 5px 0 8px;padding:4px 0 0 0  "><img src="xiaoyaoimg/rili.png" alt="..." style="border-width:0px;width:30px;height:27px;"/></a>
                  		<a href="#" onclick="getToday()" title="今天" accesskey ="T" style="margin:5px 0 0 8px;padding:4px 0 0 0  "><img src="xiaoyaoimg/shizhong.png" alt="..." style="border-width:0px;width:30px;height:27px;"/></a>
                  </div>
                </div>
              </header>
              <div class="liucheng">
              	<img src="xiaoyaoimg/liucheng.png" alt=""  width="1090" height="53" />
              </div>
              
            <div class="box">
             <div id="tablit">
    					<dl>
        				 <dt></dt>
       				   <dd class="on">行程推荐</dd>
       					 <dt></dt>
        				 <dd class="out">线路特色</dd>
       					 <dt></dt>
                 <dt></dt>
       					 <dd class="on">费用包含</dd>
                 <dt></dt>
       					 <dd class="out">报名须知</dd>
       					 <dt></dt>
       					 <dt></dt>
       					 <dd class="on">温馨提示</dd>
       					 <dt></dt>
       					 <dd class="out">其他线路</dd>
       					 <dt></dt>
    					</dl>
 		 					<div class="tabcon">A</div>
   						 <div class="tabcon dis">B</div>
    						 <div class="tabcon dis one">C</div>
    							 <div class="tabcon dis one two">D</div>
    								 <div class="tabcon dis one two three">E</div>
     										<div class="tabcon dis one two three four">F</div>
						 </div>
						</div>
             
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
  	
  	
  	
  	
  	
</body>
  
  
<script type="text/javascript">
var mDD = document.getElementById("tablit").getElementsByTagName("dd");
var mDIV= document.getElementById("tablit").getElementsByTagName("div");
for (var i=0;i<mDD.length;i++){
 (function(index) {
  mDD[index].onmouseover = function() {
   if (mDD[index].className == 'out') {
    for (var j = 0; j < mDD.length; j++) {
     mDD[j].className = 'out';
     mDIV[j].style.display = 'none';
    }
    mDD[index].className = 'on';
    mDIV[index].style.display = 'block';
   }
  }

 })(i);
}
</script>
</html>