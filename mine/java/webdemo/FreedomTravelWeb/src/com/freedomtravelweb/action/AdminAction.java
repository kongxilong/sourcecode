package com.freedomtravelweb.action;

import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.service.AdminService;

import java.util.LinkedList;
import java.util.List;
import com.freedomtravelweb.page.*;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import org.apache.struts2.ServletActionContext;

public class AdminAction extends ActionSupport{
	
	private Admin admin;
	private User user;
	private Route route;
	private Deal deal;
	private Suggestion suggestion;
	private AdminService service;
	private List <User>userList;
	private List <Route>routeList;
	private List <DealAdminTemp> dealList ;
	private DealAdminTemp dealTemp;
	private List <Suggestion>suggestionList;
	private Page page = new Page();
	
	public String adminLogin(){//OK
		
		String str = "fail";
		try {
				System.out.println("管理员名："+admin.getAdminName());
				System.out.println("密码："+admin.getPassword());
				admin = this.getService().adminLoginCheck(admin);
				if (admin != null) {
					//获取request
					HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
					HttpSession session = request.getSession();
					session.setAttribute("adminOnline", admin.getAdminName());
					str = "success";
				} else {
					System.out.println("fail");
				}
				
			} catch (Exception e) {
				e.printStackTrace();
			}
			return str;
	}
	
	//delete and update suggestion
	public String deleteSuggestion(){//OK
		
		this.getService().deleteSuggestion(this.getSuggestion().getSuggestionId());
		String str = findAllSuggestions();
		return str;
	}
	//change some words of suggestion
	public String dealSuggestion(){//Ok
		
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		Integer suggestionId = suggestion.getSuggestionId();
		System.out.println("suggestionId:"+suggestionId);
		String changeWords = "已处理";
		String admin = (String)session.getAttribute("adminOnline");
		this.getService().changeSuggestion(suggestionId, changeWords, admin);
		System.out.println("currentPage:"+page.getCurrentPage());
		String str = findAllSuggestions();
		System.out.println("suggestListSize:"+suggestionList.size());
		return str;
	}
	
	public String findAllSuggestions(){//OK
		
		String str = "error";
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(5);				//设置每页显示
		Result result = this.getService().findAllSuggestion(page);
		page = result.getPage();		//获取分页信息
		suggestionList = result.getList();	//获取商品信息列表
		if(suggestionList != null){
			
			//System.out.println(suggestionList.size());
			str = "success";
		}
		return str;
	}
	
	//delete and update user
	public String deleteUser(){//OK
		
		this.getService().deleteUser(this.getUser().getUserid());
		String str = findAllUsers();
		return str;
	}
	
	public String findAllUsers(){
		
		String str = "error";
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(5);				//设置每页显示
		Result result = this.getService().findAllUser(page);
		page = result.getPage();		//获取分页信息
		userList = result.getList();	//获取商品信息列表
		if(userList != null){
			
			str = "success";
		}
		return str;
	}
	
	//delete and update route
	public String deleteRoute(){//OK
		
		this.getService().deleteRoute(this.getRoute().getRouteId());
		String str = findAllRoutes();
		return str;
	}
	
	public String findAllRoutes(){
		
		String str = "error";
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(5);				//设置每页显示
		Result result = this.getService().findAllRoute(page);
		page = result.getPage();		//获取分页信息
		routeList = result.getList();	//获取商品信息列表
		if(routeList != null){
			
			str = "success";
		}
		return str;
	}
	
	//delete and update deal
	public String deleteDeal(){//OK
		System.out.println("TestdealId:"+this.getDeal().getDealId());
		this.getService().deleteDeal(this.getDeal().getDealId());
		String str = findAllDeals();
		return str;
	}
	
	public String findAllDeals(){
		
		String str = "error";
		if(page.getCurrentPage()==0){
			page.setCurrentPage(1);	//设置当前页
		}
		page.setEveryPage(5);				//设置每页显示
		Result result = this.getService().findAllDeal(page);
		page = result.getPage();		//获取分页信息
		List list = result.getList();	//获取商品信息列表
		System.out.println("dealList:"+list.size());
		dealList = new LinkedList<DealAdminTemp>();
		this.setDealList(dealList);
		for (int i = 0; i < list.size(); i++) {
			dealTemp = new DealAdminTemp();
	        Object[] object = (Object[])list.get(i);// 每行记录不在是一个对象 而是一个数组
	        dealTemp.setRouteId(Integer.parseInt(String.valueOf(object[0])));
	        dealTemp.setRouteName((String)object[1]);
	        dealTemp.setSeller((String)object[2]);
	        dealTemp.setBuyer((String)object[3]);
	        dealTemp.setDealNum(Integer.parseInt(String.valueOf(object[4])));
	        dealTemp.setTotalPrice(Double.parseDouble(String.valueOf(object[5])));
	        dealTemp.setTradeTime((String)object[6]);
	        dealTemp.setDealId(Integer.parseInt(String.valueOf(object[7])));
	        this.dealList.add(dealTemp);
		}
		if(dealList != null){
			
			str = "success";
		}
		return str;
	}
	
	public Admin getAdmin() {
		return admin;
	}

	public void setAdmin(Admin admin) {
		this.admin = admin;
	}

	public List<User> getUserList() {
		return userList;
	}

	public void setUserList(List<User> userList) {
		this.userList = userList;
	}

	public List<Route> getRouteList() {
		return routeList;
	}

	public void setRouteList(List<Route> routeList) {
		this.routeList = routeList;
	}

	public List<Suggestion> getSuggestionList() {
		return suggestionList;
	}

	public void setSuggestionList(List<Suggestion> suggestionList) {
		this.suggestionList = suggestionList;
	}

	public AdminService getService() {
		return service;
	}

	public void setService(AdminService service) {
		this.service = service;
	}

	public User getUser() {
		return user;
	}

	public void setUser(User user) {
		this.user = user;
	}

	public Route getRoute() {
		return route;
	}

	public void setRoute(Route route) {
		this.route = route;
	}

	public Deal getDeal() {
		return deal;
	}

	public void setDeal(Deal deal) {
		this.deal = deal;
	}

	public Suggestion getSuggestion() {
		return suggestion;
	}

	public void setSuggestion(Suggestion suggestion) {
		this.suggestion = suggestion;
	}

	public Page getPage() {
		return page;
	}

	public void setPage(Page page) {
		this.page = page;
	}

	public List<DealAdminTemp> getDealList() {
		return dealList;
	}

	public void setDealList(List<DealAdminTemp> dealList) {
		this.dealList = dealList;
	}

	public DealAdminTemp getDealTemp() {
		return dealTemp;
	}

	public void setDealTemp(DealAdminTemp dealTemp) {
		this.dealTemp = dealTemp;
	}
}
