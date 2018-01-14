package com.freedomtravelweb.action;
import com.freedomtravelweb.page.*;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream; 
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts2.ServletActionContext;

import com.freedomtravelweb.bean.DealTemp;
import com.freedomtravelweb.bean.IPTimeStamp;
import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.RouteComments;
import com.freedomtravelweb.bean.RouteCommentsDTO;

import com.freedomtravelweb.page.Page;
import com.freedomtravelweb.page.Result;
import com.freedomtravelweb.service.RouteService;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

public class RouteAction extends ActionSupport {
	private  final Log log = LogFactory.getLog(RouteAction.class);
	private static final int BUFFER_SIZE = 16 * 1024; 
	private String searchWords ;
	private SimpleDateFormat sdf = null ;
	private List <Route> list;
	private List<RouteCommentsDTO> RCDTOList;
	private Page page = new Page();
	private Route route;
	private RouteComments routeComments;
	private RouteCommentsDTO routeCommentsDTO;
	private RouteService service;
	private String username;
	private File upload;              //图片file类   
	private String uploadFileName;    // 上传文件名      
	private String uploadContentType;   // 上传文件类型         
	private String savePath;  // 保存文件的目录路径(通过依赖注入) 
	private String pictureDir;  //保存图片文件存储的最终路径
	public Route getRoute() {
		return route;
	}
	public void setRoute(Route route) {
		this.route = route;
	}
	public RouteService getService() {
		return service;
	}
	public void setService(RouteService service) {
		this.service = service;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
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
	public String getSavePath() {
		return savePath;
	}
	public void setSavePath(String savePath) {
		this.savePath = savePath;
	}
	public static int getBUFFER_SIZE() {
		return BUFFER_SIZE;
	}
	public String getDate() {  //获得指定格式的时间
		this.sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS") ;
		return this.sdf.format(new Date());
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
	public void savePicture() {
		//根据服务器的文件保存地址和原文件名创建目录文件全路径  
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);//获取request
		IPTimeStamp randomData = new IPTimeStamp(request.getRemoteAddr()) ;
		this.setPictureDir(ServletActionContext.getServletContext().getRealPath(this.getSavePath()) +"\\"+randomData.getIPTimeRand()+"."+this.getUploadFileName().split("\\.")[1]);  //This is to make the picture's name different .              
		                 
		File dstFile = new File(this.getPictureDir());          
		copy(this.upload, dstFile);  
	}
	public String saveRoute(){  //保存路线
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);//获取request
		IPTimeStamp randomData = new IPTimeStamp(request.getRemoteAddr()) ;
		String StrRandName = randomData.getIPTimeRand();  //为图片获得随机名字
		String dstDir = ServletActionContext.getServletContext().getRealPath(this.getSavePath()) +"\\"+StrRandName+"."+this.getUploadFileName().split("\\.")[1];   //图片存入的绝对路径
		this.setPictureDir(this.getSavePath().substring(1, this.getSavePath().length()) +"/"+StrRandName+"."+this.getUploadFileName().split("\\.")[1]);  //将图片的相对路径写到数据库中，偏于读取                             
		File dstFile = new File(dstDir); 
		log.debug("上传文件");
		copy(this.upload, dstFile);  
		this.getRoute().setUsername((String)request.getSession().getAttribute("nameonline"));
		this.getRoute().setPublishTime(new Date());  //保存路线发布日期
		this.getRoute().setImageDir(this.getPictureDir());  //保存图片文件路径
		log.debug("保存路线");
		this.getService().saveRoute(this.getRoute());   //写数据库
		return SUCCESS;
	}
	@Override
	public String execute() {	
		return ERROR;
	}
	public File getUpload() {
		return upload;
	}
	public void setUpload(File upload) {
		this.upload = upload;
	}
	public String getPictureDir() {
		return pictureDir;
	}
	public void setPictureDir(String pictureDir) {
		this.pictureDir = pictureDir;
	}
	public String searchRouteAddByUsername1()throws Exception{
		return SUCCESS;
	}
	public String searchRouteAddByUsername()throws Exception{
		String str = "error";
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		String userNameAddRoute = (String)session.getAttribute("nameonline");
		if(userNameAddRoute == null) {
			 userNameAddRoute = (String)session.getAttribute("userNameAddRoute");
		}
		log.debug(userNameAddRoute);
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(8);				//设置每页显示
		Result result = this.service.findRouteAddByUsername(page,userNameAddRoute);
		page = result.getPage();		//获取分页信息
		list = result.getList();	//获取商品信息列表
		if(list.size()!=0){				
			str = "success";
		}
		if(userNameAddRoute == null ) {
			session.setAttribute("userNameAddRoute", userNameAddRoute);
		}
		return str;
	}
	public String searchRouteByKeywords()throws Exception{
		String str = "error";
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		if(searchWords == null){
			
			searchWords = (String)session.getAttribute("searchWords");
		}
			
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(8);				//设置每页显示
		Result result = this.service.findRouteByKeywords(page,searchWords);
		page = result.getPage();		//获取分页信息
		list = result.getList();	//获取商品信息列表
		if(list.size()!=0){				
			str = "success";
		}
		if(searchWords != null){
			session.setAttribute("searchWords", searchWords);
		}
		return str;
	}
	
	public String openComments() {
		this.setRoute(this.service.findRouteInfoById(this.getRoute().getRouteId()));
		return SUCCESS;
	}
	public String addRouteComments() {
		
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		this.setRouteComments(new RouteComments());
		String username = (String)request.getSession().getAttribute("nameonline");
		this.getRouteComments().setCommentTime(this.getDate());
		this.getRouteComments().setComment(this.getRouteCommentsDTO().getComment());
		if("公开评价".equals(this.getRouteCommentsDTO().getRadio())) {
			this.getRouteComments().setBuyer(username);
		}else {
			this.getRouteComments().setBuyer(username.substring(0, 1)+"*******");
		}
		this.service.addRouteComments(this.getRouteComments());
		return SUCCESS;
	}
	public String findRouteByID() {
		route = this.service.findRouteById(this.getRoute().getRouteId());
		List list = this.service.findRouteCommentsById(this.getRoute().getRouteId());
		RouteCommentsDTO rc;
		this.setRCDTOList(new  LinkedList<RouteCommentsDTO>());
		for (int i = 0; i < list.size(); i++) {
			rc = new RouteCommentsDTO();
		    Object[] object = (Object[])list.get(i);// 每行记录不在是一个对象 而是一个数组
		    rc.setBuyer((String)object[0]);
		    rc.setImageDir((String)object[1]);
		    rc.setRouteName((String)object[2]);
		    rc.setCommentTime((String)object[3]);
		    this.RCDTOList.add(rc);
		}
		return "success";
	}
	public String getSearchWords() {
		return searchWords;
	}
	public void setSearchWords(String searchWords) {
		this.searchWords = searchWords;
	}
	public List<Route> getList() {
		return list;
	}
	public void setList(List<Route> list) {
		this.list = list;
	}
	public Page getPage() {
		return page;
	}
	public void setPage(Page page) {
		this.page = page;
	}
	public String findRouteByTime(){
		log.debug("开始第一层Action的调用");
		list = this.getService().findRouteByTime(8);
		return SUCCESS;
	}
	public RouteComments getRouteComments() {
		return routeComments;
	}
	public void setRouteComments(RouteComments routeComments) {
		this.routeComments = routeComments;
	}
	public RouteCommentsDTO getRouteCommentsDTO() {
		return routeCommentsDTO;
	}
	public void setRouteCommentsDTO(RouteCommentsDTO routeCommentsDTO) {
		this.routeCommentsDTO = routeCommentsDTO;
	}
	public List<RouteCommentsDTO> getRCDTOList() {
		return RCDTOList;
	}
	public void setRCDTOList(List<RouteCommentsDTO> list) {
		RCDTOList = list;
	}
}
