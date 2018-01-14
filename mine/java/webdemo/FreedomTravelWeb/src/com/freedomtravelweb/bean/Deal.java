package com.freedomtravelweb.bean;

import java.util.Date;


public class Deal {
	
	private int dealId;			//订单编号
	private String purchaserName;	//买方用户名	
	private String sellerName;		//卖方用户名
//	private Integer routeId;			//线路id用于指向线路
	private Route route;			//线路对象
	private int dealNum;			//成交数量
	private double totalMktPrice;	//总市场价格
	private double totalDsctPrice;	//总折扣价格
	private String message;			//买家的备注
	private int dealState;			//订单状态
	private int dealOver;			//交易是否完成状态
	private Date tradeTime;		//订单付钱完成时间

	
	public String getPurchaserName() {
		return purchaserName;
	}
	public void setPurchaserName(String purchaserName) {
		this.purchaserName = purchaserName;
	}
	public String getSellerName() {
		return sellerName;
	}
	public void setSellerName(String sellerName) {
		this.sellerName = sellerName;
	}
	public Route getRoute() {
		return route;
	}
	public void setRoute(Route route) {
		this.route = route;
	}
	public int getDealNum() {
		return dealNum;
	}
	public void setDealNum(int dealNum) {
		this.dealNum = dealNum;
	}
	public double getTotalMktPrice() {
		return totalMktPrice;
	}
	public void setTotalMktPrice(double totalMktPrice) {
		this.totalMktPrice = totalMktPrice;
	}
	public double getTotalDsctPrice() {
		return totalDsctPrice;
	}
	public void setTotalDsctPrice(double totalDsctPrice) {
		this.totalDsctPrice = totalDsctPrice;
	}
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	public int getDealState() {
		return dealState;
	}
	public void setDealState(int dealState) {
		this.dealState = dealState;
	}
	public Date getTradeTime() {
		return tradeTime;
	}
	public void setTradeTime(Date tradeTime) {
		this.tradeTime = tradeTime;
	}
	public int getDealId() {
		return dealId;
	}
	public void setDealId(int dealId) {
		this.dealId = dealId;
	}
	public int getDealOver() {
		return dealOver;
	}
	public void setDealOver(int dealOver) {
		this.dealOver = dealOver;
	}
	
}
