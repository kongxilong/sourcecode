package com.freedomtravelweb.service.impl;

import java.util.List;

import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.RouteComments;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.dao.RouteDAO;
import com.freedomtravelweb.page.Page;
import com.freedomtravelweb.page.PageUtil;
import com.freedomtravelweb.page.Result;
import com.freedomtravelweb.service.RouteService;

public class RouteServiceImpl implements RouteService {
	private RouteDAO routeDAO;
	public List<Route> findAllRoute() {
		// TODO Auto-generated method stub
		return null;
	}

	public Route findRouteById(Integer routeId) {
		// TODO Auto-generated method stub
		return this.routeDAO.findRouteById(routeId);

	}

	public List<User> findUserByUserName(String username) {
		// TODO Auto-generated method stub
		return null;
	}

	public void removeRoute(Route route) {
		// TODO Auto-generated method stub

	}

	public void saveRoute(Route route) {
		// TODO Auto-generated method stub
		this.routeDAO.saveRoute(route);

	}

	public void updateRoute(Route route) {
		// TODO Auto-generated method stub

	}

	public RouteDAO getRouteDAO() {
		return routeDAO;
	}

	public void setRouteDAO(RouteDAO routeDAO) {
		this.routeDAO = routeDAO;
	}
	public Result findRouteAddByUsername(Page page,String userNameAddRoute) {
		page = PageUtil.createPage(page, 
				this.routeDAO.findRouteByUsernameCount(userNameAddRoute));	
		List<Route> list = this.routeDAO.findRouteAddByUsername(page,userNameAddRoute);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
	}
	public Result findRouteByKeywords(Page page, String searchWords) {
		// TODO Auto-generated method stub
		
		page = PageUtil.createPage(page, 
				this.routeDAO.findRouteByKeywordsCount(searchWords));	
		List<Route> list = this.routeDAO.findRouteByKeywords(page,searchWords);
		Result result = new Result();	
		result.setPage(page);			
		result.setList(list);
		return result;
	}

	public List<Route> findRouteByTime(int num) {
		// TODO Auto-generated method stub
//		System.out.println("开始service层调用");
		return this.routeDAO.findRouteByTime(num);
	}

	public Route findRouteInfoById(int Id) {
		// TODO Auto-generated method stub
		
		return this.routeDAO.findRouteInfoById(Id);
	}

	public void addRouteComments(RouteComments rc) {
		// TODO Auto-generated method stub
		this.routeDAO.addRouteComments(rc);
	}

	public List findRouteCommentsById(int Id) {
		// TODO Auto-generated method stub
		return this.routeDAO.findRouteCommentsById(Id);
	}
	
}
