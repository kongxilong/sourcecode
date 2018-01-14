
var pnlYM;var pnlWeek;var pnlDay;var inputCtrl;var datePicker;function Ecalendar(year,month,day)
{this.year=year;this.month=month;this.day=day;this.minYear=Ecalendar_minYear;this.maxYear=Ecalendar_maxYear;this.displayYearRange=Ecalendar_displayYear_range;this.getDisplayYearRange=function(){return this.displayYearRange;}
this.setDisplayYearRange=setDisplayYearRange;this.getMinYear=getMinYear;this.setMinYear=setMinYear;this.getMaxYear=getMaxYear;this.setMaxYear=setMaxYear;this.selectedDate=selectedDate;this.today=today;this.currDate=currDate;this.showDate=showDate;this.displayYM=displayCalendar_ym;this.displayWeek=displayCalendar_week;this.displayDay=displayDay;this.setDay=setDay;this.setYear=setYear;this.setMonth=setMonth;this.getVisibility=getVisibility;this.close=closeCalendar;}
function checkInputDate(value)
{var arr=new Array(3);var pattern=/^(\d{4})[-]?(\d{1,2})[-]?(\d{1,2})$/;if(pattern.test(value))
{var chkYear=false;var year=parseInt(RegExp.$1,10);if(year>0)
chkYear=true;var chkMonth=false;var month=parseInt(RegExp.$2,10);if(month>0&&month<=12)
chkMonth=true;var chkDay=false;var totDay=calcDays(year,month-1);var day=parseInt(RegExp.$3,10);if(day>0&&day<=totDay)
{chkDay=true;}
if(chkYear&&chkMonth&&chkDay)
{arr[0]=year;arr[1]=month;arr[2]=day;return arr;}
else
arr=null;}
else
arr=null;return arr;}
function checkDate(year,month,day)
{if(month<0||month>11)
{return-10;}
if(datePicker.getMinYear()!=0&&year<datePicker.getMinYear())
{return-20;}
if(datePicker.getMaxYear()!=0&&year>datePicker.getMaxYear())
{return-21;}
var totalDay=calcDays(year,month);if(day>totalDay||day<1)
{return-30}
return 0;}
function displayOrCloseMonth()
{var obj=document.getElementById("calendar_m");if(obj.childNodes.length>1)
{obj.removeChild(obj.lastChild);}
else
{var newNode=document.createElement("ul");newNode.onmouseover=function(){this.style.left="auto";}
newNode.onMouseDown=function(){this.style.left="auto";}
newNode.onMouseUp=function(){this.style.left="auto";}
newNode.onmouseout=function(){this.style.left="-999em";}
for(var i=0;i<12;i++)
{var newLi=document.createElement("li");var liobjA=document.createElement("a");liobjA.href="#";liobjA.innerHTML=months[i];liobjA.setAttribute("onclick","changeMonth("+i+"); datePicker.displayDay();");newLi.appendChild(liobjA);newNode.appendChild(newLi);}
obj.appendChild(newNode);}}
function changeMonth(month)
{datePicker.setMonth(month);var obj=document.getElementById("calendar_m");obj.firstChild.innerHTML=months[month];}
function displayOrCloseYear()
{var obj=document.getElementById("calendar_y");if(obj.childNodes.length>1)
{obj.removeChild(obj.lastChild);}
else
{var newNode=document.createElement("ul");newNode.onmouseover=function(){this.style.left="auto";}
newNode.onMouseDown=function(){this.style.left="auto";}
newNode.onMouseUp=function(){this.style.left="auto";}
newNode.onmouseout=function(){this.style.left="-999em";}
var currYear=datePicker.year
var yearRange=datePicker.getDisplayYearRange()
for(var i=0;i<yearRange;i++)
{var temp;if(i<Math.floor(yearRange/2))
{temp=currYear-Math.floor(yearRange/2)+i;if(datePicker.getMinYear()!=0&&temp<datePicker.getMinYear())
{continue;}}
else if(i==Math.floor(yearRange/2))
{temp=temp+2;if(datePicker.getMaxYear()!=0&&temp>datePicker.getMaxYear())
{break;}}
else
{temp++;if(datePicker.getMaxYear()!=0&&temp>datePicker.getMaxYear())
{break;}}
var liobj=document.createElement("li");var liobjA=document.createElement("a");liobjA.href="#";liobjA.innerHTML=temp;liobjA.setAttribute("onclick","changeYear("+temp+"); datePicker.displayDay();");liobj.appendChild(liobjA);newNode.appendChild(liobj);}
obj.appendChild(newNode);}}
function changeYear(year)
{var checked=checkDate(year,datePicker.month,datePicker.day);if(checked==-20||checked==-21)
return false;datePicker.setYear(year);var obj=document.getElementById("calendar_y");obj.firstChild.innerHTML=year;return true;}
function showDate()
{if(checkDate(this.year,this.month,this.day)!=0)
return-1;var calObj=document.getElementById("calendar");if(calObj==null)
{calObj=document.createElement("div");calObj.id="calendar";calObj.style.left="auto";inputCtrl.parentNode.appendChild(calObj);}
calObj.innerHTML="";calObj.style.visibility="visible";this.displayYM();this.displayWeek();this.displayDay();return 0;}
function displayCalendar_ym()
{pnlYM=document.createElement("div");pnlYM.id="calendar_ym";var topA=document.createElement("a");topA.href="#";topA.id="calendar_y_pre";topA.innerHTML=preYearPic;topA.onclick=function(){var b=changeYear(datePicker.year-1);if(!b)return;datePicker.displayDay();}
pnlYM.appendChild(topA);var preA=document.createElement("a");preA.href="#";preA.id="calendar_m_pre";preA.innerHTML=preMonthPic;preA.onclick=function(){if(datePicker.month==0)
{var b=changeYear(datePicker.year-1);if(!b)return;datePicker.month=11;}
else
datePicker.month--;changeMonth(datePicker.month);datePicker.displayDay();}
pnlYM.appendChild(preA);var yobj=document.createElement("ul");yobj.id="calendar_y";var yTitle=document.createElement("li");yTitle.className="calendar_header";yTitle.innerHTML=this.year;yTitle.onclick=displayOrCloseYear;yobj.appendChild(yTitle);pnlYM.appendChild(yobj);var mobj=document.createElement("ul");mobj.id="calendar_m";var mobjLi=document.createElement("li");mobjLi.className="calendar_header";mobjLi.innerHTML=months[this.month];mobjLi.onclick=displayOrCloseMonth;mobj.appendChild(mobjLi);pnlYM.appendChild(mobj);var obj=document.getElementById("calendar");obj.appendChild(pnlYM);var lastA=document.createElement("a");lastA.href="#";lastA.id="calendar_y_next";lastA.innerHTML=nextYearPic;lastA.onclick=function(){var b=changeYear(datePicker.year+1);if(!b)return;datePicker.displayDay();}
pnlYM.appendChild(lastA);var nextA=document.createElement("a");nextA.href="#";nextA.id="calendar_m_next";nextA.innerHTML=nextMonthPic;nextA.onclick=function()
{if(datePicker.month==11)
{var b=changeYear(datePicker.year+1);if(!b)return;datePicker.month=0;}
else
datePicker.month++;changeMonth(datePicker.month);datePicker.displayDay();}
pnlYM.appendChild(nextA);}
function displayCalendar_week()
{pnlWeek=document.createElement("ul");pnlWeek.id="calendar_week";for(var i=0;i<7;i++)
{var wobj=document.createElement("li");wobj.innerHTML=weeks[i];pnlWeek.appendChild(wobj);}
var obj=document.getElementById("calendar");obj.appendChild(pnlWeek);}
function displayDay()
{if(checkDate(datePicker.year,datePicker.month,datePicker.day)==-30)
datePicker.setDay(1);if(pnlDay!=null)
{var objCal=document.getElementById("calendar");if(objCal.lastChild==pnlDay)
objCal.removeChild(pnlDay);}
pnlDay=document.createElement("ul");var day=new Date(this.year,this.month,1).getDay();for(var i=0;i<day;i++)
{var liDay=document.createElement("li");liDay.innerHTML="";pnlDay.appendChild(liDay);}
var maxDay=calcDays(this.year,this.month);for(var i=0;i<maxDay;i++)
{var liDay=document.createElement("li");if(this.day==i+1)
{liDay.className="current_selection";}
var aDay=document.createElement("a");aDay.innerHTML=i+1;aDay.href="#";aDay.setAttribute("onclick","changeDate("+this.year+","+this.month+","+(i+1)+");");liDay.appendChild(aDay);pnlDay.appendChild(liDay);}
var x=(day+maxDay);var remainSpan=(x%7==0)?0:(7-x%7);for(var i=0;i<remainSpan;i++)
{var liDay=document.createElement("li");liDay.innerHTML="";pnlDay.appendChild(liDay);}
var obj=document.getElementById("calendar");obj.appendChild(pnlDay);}
function changeDate(year,month,day)
{datePicker.setYear(year);datePicker.setMonth(month);datePicker.setDay(day);inputCtrl.value=datePicker.currDate();}
function setInputControl(id)
{var obj=document.getElementById(id);inputCtrl=obj;}
function closeCalendar()
{var obj=document.getElementById("calendar");obj.style.position="relative";inputCtrl.parentNode.removeChild(obj);}
function calcDays(year,month)
{var m=month+1;if(m==2)
{var isleap=(year%400==0)||(year%4==0&&year%100!=0);return isleap?29:28;}
else if(m<8)
{return m%2==0?30:31;}
else
{return m%2==0?31:30;}}
function setDisplayYearRange(year)
{this.displayYearRange=year;}
function getMinYear()
{return this.minYear;}
function setMinYear(year)
{this.minYear=year;}
function getMaxYear()
{return this.maxYear;}
function setMaxYear(year)
{this.maxYear=year;}
function selectedDate()
{return new Date(year,month,day);}
function getVisibility()
{var obj=document.getElementById("calendar");return obj.style.visibility;}
function currDate()
{var y=this.year+"-";var m=(this.month+1)+"-";var d=this.day;return y+m+d;}
function today()
{var curr=new Date();var y=curr.getFullYear()+"-";var m=(curr.getMonth()+1)+"-";var d=curr.getDate();return y+m+d;}
function setDay(day)
{this.day=day}
function setYear(year)
{this.year=year;}
function setMonth(month)
{this.month=month;}