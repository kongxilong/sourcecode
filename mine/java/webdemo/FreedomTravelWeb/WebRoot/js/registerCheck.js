 //定义一个变量用于存放XMLHttpRequest对象  
var xmlHttp ; 
var flag = true ;
//该函数用于创建一个XMLHttpRequest对象  
function createXMLHttpRequest() {
			if (window.ActiveXObject) {    
				xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");  
 			}else if(window.XMLHttpRequest) {   
 				xmlHttp = new XMLHttpRequest();  
 			} 
		}
function checkUsername() {
			var username = document.getElementById("username").value;
			if(username == "") {
				document.getElementById("usernameMsg").innerHTML = "*用户名必填" ;
				document.getElementById("usernameMsg").style.cssText="font-size:20px;color:red;";
				flag = false ;
				return;
			}
			if(username.length < 2 || username.length > 20) {
				document.getElementById("usernameMsg").innerHTML = "*用户名必须在2~20个字符之间" ;
				document.getElementById("usernameMsg").style.cssText="font-size:20px;color:red;";
				flag = false ;
				return;
			}
			var patrn=/^[a-zA-Z]{1}([a-zA-Z0-9]|[._]){4,19}$/; 
			if(!patrn.exec(username)) {
				document.getElementById("usernameMsg").innerHTML = "*只能输入以字母开头、可带数字、“_”、“.”的字串" ;
				document.getElementById("usernameMsg").style.cssText="font-size:20px;color:red;";
				flag = false ;
				return;
			}
			createXMLHttpRequest();
			xmlHttp.open("POST","checkUser?username="+username) ;
			xmlHttp.onreadystatechange = checkUsernameCallback;  //回调函数
			xmlHttp.send(null) ;
		}
		function checkUsernameCallback(){
			//定义一个变量用于存放从服务器返回的响应结果   
			var responseContext;   
			if(xmlHttp.readyState == 4) { //如果响应完成	    
				if(xmlHttp.status == 200) {  //如果返回成功       
					responseContext = xmlHttp.responseText;    //取出服务器的响应内容
					var text = xmlHttp.responseText ;        
					if ( text == "false" ) {   //如果注册名检查有效 即用户名不存在
							document.getElementById("usernameMsg").innerHTML = "√" ;
							document.getElementById("usernameMsg").style.cssText="font-size:20px;color:blue;";
							flag = true;
							return;
					}else {
						document.getElementById("usernameMsg").innerHTML = "用户名已存在" ;
						document.getElementById("usernameMsg").style.cssText="font-size:20px;color:red;";
						flag = false ;
						return;
					}
				}
			}
		}
		
function checkPassword() {
			var password = document.getElementById("password").value;
			if(password == null||password.length > 16 || password.length <6) {
				document.getElementById("passwordMsg").innerHTML = "密码必须为6~16位之间" ;
				document.getElementById("passwordMsg").style.cssText="font-size:20px;color:red;";
				flag = false ;
				
				return;
			}else {
				document.getElementById("passwordMsg").innerHTML = "√" ;
				document.getElementById("passwordMsg").style.cssText="font-size:20px;color:blue;";
				flag = true;
				return;
			}
		}
function checkConfirmpassword() {
			var confirmpassword = document.getElementById("confirmpassword").value;
			var password = document.getElementById("password").value;
			if(confirmpassword != password) {
				document.getElementById("confirmpasswordMsg").innerHTML = "密码不一致" ;
				document.getElementById("confirmpasswordMsg").style.cssText="font-size:20px;color:red;";
				flag = false ;		
				return;
			}else {
				document.getElementById("confirmpasswordMsg").innerHTML = "√" ;
				document.getElementById("confirmpasswordMsg").style.cssText="font-size:20px;color:blue;";		
				flag = true;
				return;
			}
		}
function checkEmail() {
          var temp = document.getElementById("email").value;
           //对电子邮件的验证
          var myreg = /^([a-zA-Z0-9]+[_|\_|\.]?)*[a-zA-Z0-9]+@([a-zA-Z0-9]+[_|\_|\.]?)*[a-zA-Z0-9]+\.[a-zA-Z]{2,3}$/;
            if(!myreg.test(temp))
            {
               	document.getElementById("emailMsg").innerHTML = "邮箱格式不正确" ;
				document.getElementById("emailMsg").style.cssText="font-size:20px;color:red;";		
                flag = false;
                return ;
           }else {
           		document.getElementById("emailMsg").innerHTML = "√" ;
				document.getElementById("emailMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
           }
        }
function checkRealname() {
			var temp = document.getElementById("realname").value;
			if(temp == null || temp.length < 2 || temp.length > 40) {
				document.getElementById("realnameMsg").innerHTML = "姓名必须在2~40个字符间" ;
				document.getElementById("realnameMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
			}else {
				document.getElementById("realnameMsg").innerHTML = "√" ;
				document.getElementById("realnameMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
			}
		}
		
function checkCellphone() {
			var temp = document.getElementById("cellphone").value;
			if(temp.length != 11) {
				document.getElementById("cellphoneMsg").innerHTML = "手机号应该是11位" ;
				document.getElementById("cellphoneMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
			}
			var reg = new RegExp("^[0-9]*$");
			if(!reg.test(temp)){
				document.getElementById("cellphoneMsg").innerHTML = "请输入数字" ;
				document.getElementById("cellphoneMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
			}else {
				document.getElementById("cellphoneMsg").innerHTML = "√" ;
				document.getElementById("cellphoneMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
			}
		}
function checkCompanyname() {
			var temp = document.getElementById("companyname").value;
			if(temp == null ||temp.length < 2 ||temp.length > 50) {
				document.getElementById("companynameMsg").innerHTML = "公式名称须在2~50个字符之间" ;
				document.getElementById("companynameMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
            }else {
            	document.getElementById("companynameMsg").innerHTML = "√" ;
				document.getElementById("companynameMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
            }
		}
function checkAddress()	{
			var temp = document.getElementById("address").value;
			if(temp == null || temp <4 || temp > 256) {
				document.getElementById("addressMsg").innerHTML = "地址须在 4~256个字符之间" ;
				document.getElementById("addressMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
			}else {
				document.getElementById("addressMsg").innerHTML = "√" ;
				document.getElementById("addressMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
			}
		}
function checkMainbussiness(){
			var temp = document.getElementById("mainbussiness").value;
			if(temp == null || temp <4 || temp > 256) {
				document.getElementById("mainbussinessMsg").innerHTML = "主营业务须在 4~256个字符之间" ;
				document.getElementById("mainbussinessMsg").style.cssText="font-size:20px;color:red;";		
                flag = false ;
                return ;
			}else {
				document.getElementById("mainbussinessMsg").innerHTML = "√" ;
				document.getElementById("mainbussinessMsg").style.cssText="font-size:20px;color:blue;";		
                flag = true;
                return ;
			}
		} 
function checkForm() {
			return flag ;
		}
