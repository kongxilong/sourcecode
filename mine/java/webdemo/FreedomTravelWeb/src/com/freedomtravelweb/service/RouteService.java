package com.freedomtravelweb.service;

import java.util.List;

import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.RouteComments;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.page.Page;
import com.freedomtravelweb.page.Result;

public interface RouteService {
	public void saveRoute(Route route);
	public void removeRoute(Route route);
	public Route findRouteById(Integer routeId);
	public List<User> findUserByUserName(String username);
	public List<Route>  findAllRoute();
	public void updateRoute(Route route);
	public Result findRouteByKeywords(Page page,String searchWords);
	public Result findRouteAddByUsername(Page page,String userNameAddRoute);
	public List<Route> findRouteByTime(int num);
	public Route findRouteInfoById(int Id);
	public void addRouteComments(RouteComments rc);
	public List findRouteCommentsById(int Id);
}
