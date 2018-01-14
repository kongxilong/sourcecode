package com.freedomtravelweb.dao;

import java.util.List;

import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.RouteComments;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.page.Page;
import com.freedomtravelweb.page.PageUtil;


public interface RouteDAO {
	public void saveRoute(Route route);
	public void removeRoute(Route route);
	public Route findRouteById(Integer routeId);
	public List<User> findUserByUserName(String username);
	public List<Route>  findAllRoute();
	public void updateRoute(Route route);
	public int findRouteByKeywordsCount(String searchWords);
	public List<Route> findRouteByKeywords(Page page,String searchWords);
	public int findRouteByUsernameCount(String userNameAddRoute);	
	public List<Route> findRouteAddByUsername(Page page,String userNameAddRoute);
	public List<Route> findRouteByTime(int num);
	public Route findRouteInfoById(int id);
	public void addRouteComments(RouteComments rc);
	public List findRouteCommentsById(int Id);
}
