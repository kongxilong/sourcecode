package com.freedomtravelweb.dao;

import java.util.List;

import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.page.*;

public interface AdminDAO {
	
	public Admin adminLoginCheck(Admin admin);
	//for suggestion
	public void deleteSuggestion(Integer suggestionId);
	public void changeSuggestion(Integer suggestionId,String changeWords,String admin);
	public List<Suggestion> findAllSuggestion(Page page);
	public int findAllSuggestionCount();
	//for user
	public void deleteUser(Integer userId);
	public List<User> findAllUser(Page page);
	public int findAllUserCount();
	//for route
	public void deleteRoute(Integer routeId);
	public List<Route> findAllRoute(Page page);
	public int findAllRouteCount();
	//for deal
	public void deleteDeal(Integer dealId);
	public List findAllDeal(Page page);
	public int findAllDealCount();
}
