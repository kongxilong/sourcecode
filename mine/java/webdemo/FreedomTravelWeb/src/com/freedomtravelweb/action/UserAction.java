package com.freedomtravelweb.action;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts2.ServletActionContext;

import com.freedomtravelweb.bean.IPTimeStamp;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.bean.UserDTO;
import com.freedomtravelweb.service.UserService;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

public class UserAction extends ActionSupport { //该类主要用作注册添加用户
	private  final Log log = LogFactory.getLog(RouteAction.class);
	private static final int BUFFER_SIZE = 16 * 1024; //图片缓存区大小
	private User user ;
	private UserService service;
	private String code;  //用来接收来自表单的验证码
	private UserDTO userDTO;
	private String username; //用于接收从ajax传来的参数验证用户名是否已经注册
	private File upload;              //图片file类   
	private String uploadFileName;    // 上传文件名      
	private String uploadContentType;   // 上传文件类型         
	private String savePath;  // 保存文件的目录路径(通过依赖注入) 
	private List<UserDTO> list;
	public User getUser() {
		return user;
	}
	public void setUser(User user) {
		this.user = user;
	}
	public UserService getService() {
		return service;
	}
	public void setService(UserService service) {
		this.service = service;
	}
	public String getCode() {
		return code;
	}
	public void setCode(String code) {
		this.code = code;
	}  
	
	public String saveUser() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		if(session.getAttribute("imagecode").equals(this.getCode())) {
			this.getService().save(this.getUser());
			return SUCCESS;
		}
		return ERROR;
	}
	public String getUserOnlineName() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		return (String)session.getAttribute("nameonline");
	}
	public void checkUserExists() throws IOException {
		String responseContext;
		//HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);//获取request
		HttpServletResponse response = (HttpServletResponse) ActionContext.getContext().get(ServletActionContext.HTTP_RESPONSE);
		PrintWriter out = response.getWriter();
		if(!this.getService().findUserByUserName(this.getUsername()).isEmpty()) { //用户已存在
			responseContext = "true" ;
			out.print(responseContext);
			out.flush();
			out.close();
		}else { //用户不存在 可以注册
			responseContext = "false" ;
			out.print(responseContext);
			out.flush();
			out.close();
		}
	
	}
	public String loginUser() {
		String responseText = "fail";
		try {
			//logincheck(User user)根据传入的user，查询是否已经存在返回boolean;
				user = this.getService().loginCheck(user);
				if (user != null) {
					//获取request
					HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
					HttpSession session = request.getSession();
					session.setAttribute("nameonline", user.getUsername());
					responseText = "success";
				} else {
					System.out.println("fail");
					responseText = "fail";
				}
				
			} catch (Exception e) {
				e.printStackTrace();
			}
			return responseText;
		}
	private void copy(File src, File dst) {//实现从文件源copy到文件目的地     
		InputStream in = null;         
		OutputStream out = null;         
		try {              
			in = new BufferedInputStream(new FileInputStream(src),BUFFER_SIZE);  
			out = new BufferedOutputStream(new FileOutputStream(dst),BUFFER_SIZE);       
			byte[] buffer = new byte[BUFFER_SIZE];              
			int len = 0;             
			while ((len = in.read(buffer)) > 0) {         
				out.write(buffer, 0, len);            
				}         
			} catch (Exception e) {        
				e.printStackTrace();       
			} finally {       
				if (null != in) {   
					try {              
						in.close();      
						} catch (IOException e){            
							e.printStackTrace();           
							}             
						}
				if (null != out) {       
					try {                     
						out.close();          
						} catch (IOException e) {           
							e.printStackTrace();                 
							}             
						}          
				}      
			} 
	public String passwordEdit() {
		if(this.getUserDTO().getOriginalPassword().isEmpty()||!this.getUserDTO().getConfirmPassword().equals(this.getUserDTO().getNewPassword())) {
			return ERROR;
		}
		
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		this.setUser(new User());
		this.getUser().setUsername((String)session.getAttribute("nameonline"));
		this.getUser().setPassword(this.getUserDTO().getOriginalPassword());
		if(this.service.loginCheck(this.user)==null) {
			return ERROR;
		}
		this.getUser().setPassword(this.getUserDTO().getNewPassword());
		this.getUser().setEmail(this.getUserDTO().getEmail());
		this.service.passwordEdit(user);
		return SUCCESS;
	}
	public String listUser() throws Exception{
		Map request = (Map)ActionContext.getContext().get("request");		
		request.put("list", this.service.findAll());
		return SUCCESS;
	}
	public String imageUpdate() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);//获取request
		
		IPTimeStamp randomData = new IPTimeStamp(request.getRemoteAddr()) ;
		String StrRandName = randomData.getIPTimeRand();  //为图片获得随机名字
		String dstDir = ServletActionContext.getServletContext().getRealPath(this.getSavePath()) +"\\"+StrRandName+"."+this.getUploadFileName().split("\\.")[1];   //图片存入的绝对路径
		this.setUser(new User());
		this.getUser().setIconDir(this.getSavePath().substring(1, this.getSavePath().length()) +"/"+StrRandName+"."+this.getUploadFileName().split("\\.")[1]);  //将图片的相对路径写到数据库中，偏于读取
		File dstFile = new File(dstDir); 
		log.debug("开始上传头像文件");
		copy(this.upload, dstFile); 
		this.getUser().setUsername((String)request.getSession().getAttribute("nameonline"));
		this.service.iconEdit(user);
		return SUCCESS;
	}
	public String showMyZone() {
		 this.setUser(this.service.showZoneByName(this.getUserOnlineName()));
		 this.setUserDTO(new UserDTO());
		 this.getUserDTO().setIconDir(this.getUser().getIconDir());
		 this.getUserDTO().setUsername(this.getUserDTO().getUsername());
		 this.list = new LinkedList<UserDTO>();
		 this.list.add(userDTO);
		return SUCCESS;
	}
	public String showPersonalData() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);//获取request
		List listTemp = this.service.showDataByName((String)request.getSession().getAttribute("nameonline"));
		list = new LinkedList<UserDTO>();
		for(int i = 0; i < listTemp.size(); i++) {
			 Object[] object = (Object[])listTemp.get(i);// 每行记录不在是一个对象 而是一个数组
			 userDTO = new UserDTO();
			 userDTO.setIconDir((String)object[0]);
		     userDTO.setUsername((String)object[1]);
		     userDTO.setRealName((String)object[2]);
		     userDTO.setCompanyName((String)object[3]);
		     userDTO.setCompanyAddress((String)object[4]);
		     userDTO.setCellphone((String)object[5]);
		     userDTO.setEmail((String)object[6]);
		     this.list.add(userDTO);	
		}
		return SUCCESS;
	}
	public String dataUpdate() {
		this.getUser().setUsername(this.getUserOnlineName());
		this.service.dataUpdate(this.getUser());
		return SUCCESS;
	}
	@Override
	public String execute() throws Exception {

			return SUCCESS;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public UserDTO getUserDTO() {
		return userDTO;
	}
	public void setUserDTO(UserDTO userDTO) {
		this.userDTO = userDTO;
	}

	public String getSavePath() {
		return savePath;
	}
	public void setSavePath(String savePath) {
		this.savePath = savePath;
	}
	public List<UserDTO> getList() {
		return list;
	}
	public void setList(List<UserDTO> list) {
		this.list = list;
	}
	public File getUpload() {
		return upload;
	}
	public void setUpload(File upload) {
		this.upload = upload;
	}
	public String getUploadFileName() {
		return uploadFileName;
	}
	public void setUploadFileName(String uploadFileName) {
		this.uploadFileName = uploadFileName;
	}
	public String getUploadContentType() {
		return uploadContentType;
	}
	public void setUploadContentType(String uploadContentType) {
		this.uploadContentType = uploadContentType;
	}
	public static int getBUFFER_SIZE() {
		return BUFFER_SIZE;
	}
	
}
