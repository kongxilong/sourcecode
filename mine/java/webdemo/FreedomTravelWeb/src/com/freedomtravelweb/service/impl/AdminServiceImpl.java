package com.freedomtravelweb.service.impl;


import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.service.AdminService;
import com.freedomtravelweb.dao.AdminDAO;
import com.freedomtravelweb.page.*;

import java.util.List;

public class AdminServiceImpl implements AdminService{
	
	private AdminDAO adminDAO;
	
	public Admin adminLoginCheck(Admin admin){
		
		return this.getAdminDAO().adminLoginCheck(admin);
	}
	
	public void deleteSuggestion(Integer suggestionId){
		
		this.getAdminDAO().deleteSuggestion(suggestionId);
	}
	
	public void changeSuggestion(Integer suggestionId,String changeWords,String admin){
		
		this.getAdminDAO().changeSuggestion(suggestionId, changeWords, admin);
	}
	
	public Result findAllSuggestion(Page page){
		
		page = PageUtil.createPage(page, 
				this.getAdminDAO().findAllSuggestionCount());	
		List<Suggestion> list = this.getAdminDAO().findAllSuggestion(page);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
		//return this.getAdminDAO().findAllSuggestion();
	}
	
	public void deleteUser(Integer userId){
		
		this.getAdminDAO().deleteUser(userId);
	}

	public Result findAllUser(Page page){
		
		page = PageUtil.createPage(page, 
				this.getAdminDAO().findAllUserCount());	
		List<User> list = this.getAdminDAO().findAllUser(page);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
	}
	
	public void deleteRoute(Integer routeId){
		
		this.getAdminDAO().deleteRoute(routeId);
	}

	public Result findAllRoute(Page page){
		
		page = PageUtil.createPage(page, 
				this.getAdminDAO().findAllRouteCount());	
		List<Route> list = this.getAdminDAO().findAllRoute(page);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
	}
	
	public void deleteDeal(Integer dealId){
		
		this.getAdminDAO().deleteDeal(dealId);
	}

	public Result findAllDeal(Page page){
		
		page = PageUtil.createPage(page, 
				this.getAdminDAO().findAllDealCount());	
		List list = this.getAdminDAO().findAllDeal(page);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
	}

	public AdminDAO getAdminDAO() {
		return adminDAO;
	}

	public void setAdminDAO(AdminDAO adminDAO) {
		this.adminDAO = adminDAO;
	}
}
