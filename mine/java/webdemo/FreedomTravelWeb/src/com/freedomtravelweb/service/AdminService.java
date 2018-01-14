package com.freedomtravelweb.service;

import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.page.*;

import java.util.List;

public interface AdminService {

	public Admin adminLoginCheck(Admin admin);
	//for suggestion
	public void deleteSuggestion(Integer suggestionId);
	public void changeSuggestion(Integer suggestionId,String changeWords,String admin);
	public Result findAllSuggestion(Page page);
	//for user
	public void deleteUser(Integer userId);
	public Result findAllUser(Page page);
	//for route
	public void deleteRoute(Integer routeId);
	public Result findAllRoute(Page page);
	//for deal
	public void deleteDeal(Integer dealId);
	public Result findAllDeal(Page page);
}
