package com.freedomtravelweb.action;

import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.struts2.ServletActionContext;
import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.service.CollectService;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

public class CollectAction extends ActionSupport{
	private Collect collect;
	private CollectService service;
	private Route route;
	private List<CollectTemp> listTemp;
	public Collect getCollect() {
		return collect;
	}
	public void setCollect(Collect collect) {
		this.collect = collect;
	}
	
	public CollectService getService() {
		return service;
	}
	public void setService(CollectService service) {
		this.service = service;
	}
	public Route getRoute() {
		return route;
	}
	public void setRoute(Route route) {
		this.route = route;
	}
	public List<CollectTemp> getListTemp() {
		return listTemp;
	}
	public void setListTemp(List<CollectTemp> listTemp) {
		this.listTemp = listTemp;
	}
	public String addToCollect() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		this.setCollect(new Collect());
		this.getCollect().setRoute(this.getRoute());
		this.getCollect().setUsername((String)session.getAttribute("nameonline"));
		this.service.saveCollect(collect);
		//this.listCollect();
		return SUCCESS;
	}
	public String listCollect() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		String username = (String)session.getAttribute("nameonline");
		List list = this.service.findCollectByName(username);
		CollectTemp collectTemp;
		this.setListTemp(new LinkedList<CollectTemp>());
		for (int i = 0; i < list.size(); i++) {
			collectTemp = new CollectTemp();
	        Object[] object = (Object[])list.get(i);// 每行记录不在是一个对象 而是一个数组
	        collectTemp.setCollectId(Integer.parseInt(String.valueOf(object[0])));
	        collectTemp.setRouteId(Integer.parseInt(String.valueOf( object[1])));
	        collectTemp.setRouteName((String)object[2]);
	        collectTemp.setUsername((String)object[3]);
	        collectTemp.setPrice((String)object[4]);
	        collectTemp.setTravelTime((String)object[5]);
	        collectTemp.setImageDir((String)object[6]);
	        this.listTemp.add(collectTemp);
		}
		return SUCCESS;
	}
}
