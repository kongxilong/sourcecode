package com.freedomtravelweb.action;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts2.ServletActionContext;

import com.freedomtravelweb.bean.Deal;
import com.freedomtravelweb.bean.DealTemp;
import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.service.DealService;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

public class DealAction extends ActionSupport{
	private  final Log log = LogFactory.getLog(RouteAction.class);
	private Deal deal ;
	private Route route;
	private DealTemp dealTemp;
	private DealService service;
	
	private List<DealTemp> dealList;
	public Deal getDeal() {
		return deal;
	}
	public void setDeal(Deal deal) {
		this.deal = deal;
	}
	public Route getRoute() {
		return route;
	}
	public void setRoute(Route route) {
		this.route = route;
	}
	public DealService getService() {
		return service;
	}
	public void setService(DealService service) {
		this.service = service;
	}
	public String deleteDeal() {
		this.service.deleteDeal(this.getDeal());
		return SUCCESS;
	}
	public String  addToDeal(){
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		//this.getDeal().setRouteId(this.getDeal().getRoute().getRouteId());
		this.setDeal(new Deal());
		this.getDeal().setPurchaserName((String)session.getAttribute("nameonline"));
		this.getDeal().setRoute(this.getRoute());
//		log.debug(getDeal().getPurchaserName());
//		log.debug(getDeal().getRouteId());
		this.service.addToDeal(this.deal);
		return SUCCESS;
	}
	public String submitDeal(){
		log.debug("您传入的ID参数是："+this.deal.getDealId());
		if(deal.getDealId() == 0){
		   return ERROR;
		}
		log.debug("您传入的购买数量是："+this.deal.getDealNum());
		List listTemp = this.service.findInfoByDealId(this.deal.getDealId());
		Object[] object = (Object[])listTemp.get(0);
		String sellerName = (String)object[0];
		String price = (String)object[1];
		String routename = (String)object[2];
		double totalvalue = (Double.parseDouble(price))*(deal.getDealNum());
		log.debug("您传入的购买价格是："+totalvalue);
		this.deal.setSellerName(sellerName);
		this.deal.setTotalMktPrice(totalvalue);
		this.deal.setTradeTime(new Date());
		this.service.updateDeal(deal);
		DealTemp dealTemp = new DealTemp();
		dealTemp.setUsername(sellerName);
		dealTemp.setDealId(this.deal.getDealId());
		dealTemp.setTotalPrice(totalvalue);
		dealTemp.setRouteName(routename);
		this.setDealList(new LinkedList());
		this.getDealList().add(dealTemp);
		return SUCCESS;
	}
	public String payDeal(){
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);		
		this.getDealTemp().setUsername((String)request.getSession().getAttribute("nameonline"));
		if(this.service.isUserExist(dealTemp)) {
			this.setDeal(new Deal());
			this.getDeal().setDealId(this.getDealTemp().getDealId());
			this.getDeal().setDealState(1);
			this.service.updateDealState(this.getDeal());
			return SUCCESS;
		}
		return ERROR;
	}
	public String listMyDeal(){
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		List list = this.service.findMyDealByName((String)session.getAttribute("nameonline"));
		DealTemp dealTemp;
		this.setDealList(new  LinkedList<DealTemp>());
		for (int i = 0; i < list.size(); i++) {
		        dealTemp = new DealTemp();
		        Object[] object = (Object[])list.get(i);// 每行记录不在是一个对象 而是一个数组
		        dealTemp.setUsername((String)object[0]);
		        dealTemp.setImageDir((String)object[1]);
		        dealTemp.setRouteName((String)object[2]);
		        dealTemp.setTotalDays((String)object[3]);
		        dealTemp.setKeyWord((String)object[4]);
		        dealTemp.setMarketPrice((String)object[5]);
		        int Id = Integer.parseInt(String.valueOf(object[6]));
		        dealTemp.setDealId(Id);
		        this.dealList.add(dealTemp);
		}
		return SUCCESS;
	}
	public String showDeal() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		List list = this.service.findAllDealByName((String)session.getAttribute("nameonline"));
		DealTemp dealTemp;
		this.setDealList(new  LinkedList<DealTemp>());
		for (int i = 0; i < list.size(); i++) {
		        dealTemp = new DealTemp();
		        Object[] object = (Object[])list.get(i);// 每行记录不在是一个对象 而是一个数组
		        dealTemp.setUsername((String)object[0]);
		        dealTemp.setImageDir((String)object[1]);
		        dealTemp.setRouteName((String)object[2]);
		        dealTemp.setTotalDays((String)object[3]);
		        dealTemp.setKeyWord((String)object[4]);
		        dealTemp.setMarketPrice((String)object[5]);
		        dealTemp.setDealId(Integer.parseInt(String.valueOf(object[6])));
		        dealTemp.setRouteNum(String.valueOf(object[7]));
		        
		        dealTemp.setDealState(String.valueOf(object[8]));
		        dealTemp.setDealOver(String.valueOf(object[9]));
		        dealTemp.setRouteId(Integer.parseInt(String.valueOf(object[10])));
		        
		        double totalprice = Double.parseDouble(dealTemp.getMarketPrice())*(Double.parseDouble(dealTemp.getRouteNum()));
		        dealTemp.setTotalPrice(totalprice);
		        this.dealList.add(dealTemp);
		}
		return SUCCESS;
	}
	public String finishDeal() {
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);		
		
		this.getDealTemp().setUsername((String)request.getSession().getAttribute("nameonline"));
		if(this.service.isUserExist(dealTemp)) {
			this.setDeal(new Deal());
			this.getDeal().setDealId(this.getDealTemp().getDealId());
			this.getDeal().setDealOver(1);
			this.service.finishDeal(this.getDeal());
			return SUCCESS;
		}
		return ERROR;
	}
	public String dealConfirm() {
		this.setDealTemp(new DealTemp());
		this.getDealTemp().setDealId(this.getDeal().getDealId());
//		System.out.println("传入的ID是："+this.getDealTemp().getDealId());
		return SUCCESS;
	}
	@Override
	public String execute()throws Exception{
		return SUCCESS;
	}
	public List getDealList() {
		return dealList;
	}
	public void setDealList(List dealList) {
		this.dealList = dealList;
	}
	public DealTemp getDealTemp() {
		return dealTemp;
	}
	public void setDealTemp(DealTemp dealTemp) {
		this.dealTemp = dealTemp;
	}
	
}
