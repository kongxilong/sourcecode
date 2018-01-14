package com.freedomtravelweb.bean;

import java.util.Date;

public class Route {
	private Integer routeId;		//线路ID
	private String routeName;       //线路名称
	private String cityType;		//城市类型
	private String username;		//线路发布方
	private String keyWord;			//线路关键字
	private String stayType;			//住宿类型
	private String totalDays;			//线路旅游的总时间
	private String stayRadio;       //是否提供住宿radiao
	
	private Date publishTime;		//发布时间
	
	private String  routeStartTime;  //报团开始时间
	private String  routeEndTime;    //报团结束时间
	private String routeCycleTime;   //发团周期
	private String marketPrice;		//市场价格
	private String transType;			//交通类型
	private String discountInfo;	//优惠信息
	private String routeDetailInfo;		//线路详细描述
	private String needToknow;		//报名须知
	private String otherRoutes;     //其他线路
	private String validAmounts;		//线路有效数量
	
	private String imageDir;           //线路图片
	public Integer getRouteId() {
		return routeId;
	}
	public void setRouteId(Integer routeId) {
		this.routeId = routeId;
	}

	public String getRouteName() {
		return routeName;
	}

	public void setRouteName(String routeName) {
		this.routeName = routeName;
	}

	public String getCityType() {
		return cityType;
	}

	public void setCityType(String cityType) {
		this.cityType = cityType;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getKeyWord() {
		return keyWord;
	}
	public void setKeyWord(String keyWord) {
		this.keyWord = keyWord;
	}

	public String getStayType() {
		return stayType;
	}

	public void setStayType(String stayType) {
		this.stayType = stayType;
	}
	public String getStayRadio() {
		return stayRadio;
	}

	public void setStayRadio(String stayRadio) {
		this.stayRadio = stayRadio;
	}

	public Date getPublishTime() {
		return publishTime;
	}

	public void setPublishTime(Date publishTime) {
		this.publishTime = publishTime;
	}

	public String getRouteStartTime() {
		return routeStartTime;
	}

	public void setRouteStartTime(String routeStartTime) {
		this.routeStartTime = routeStartTime;
	}

	public String getRouteEndTime() {
		return routeEndTime;
	}

	public void setRouteEndTime(String routeEndTime) {
		this.routeEndTime = routeEndTime;
	}

	public String getRouteCycleTime() {
		return routeCycleTime;
	}

	public void setRouteCycleTime(String routeCycleTime) {
		this.routeCycleTime = routeCycleTime;
	}
	public String getRouteDetailInfo() {
		return routeDetailInfo;
	}

	public void setRouteDetailInfo(String routeDetailInfo) {
		this.routeDetailInfo = routeDetailInfo;
	}

	public String getNeedToknow() {
		return needToknow;
	}

	public void setNeedToknow(String needToknow) {
		this.needToknow = needToknow;
	}

	public String getOtherRoutes() {
		return otherRoutes;
	}

	public void setOtherRoutes(String otherRoutes) {
		this.otherRoutes = otherRoutes;
	}
	public String getDiscountInfo() {
		return discountInfo;
	}

	public void setDiscountInfo(String discountInfo) {
		this.discountInfo = discountInfo;
	}
	public String getTransType() {
		return transType;
	}

	public void setTransType(String transType) {
		this.transType = transType;
	}

	public String getImageDir() {
		return imageDir;
	}

	public void setImageDir(String imageDir) {
		this.imageDir = imageDir;
	}

	public String getTotalDays() {
		return totalDays;
	}

	public void setTotalDays(String totalDays) {
		this.totalDays = totalDays;
	}

	public String getMarketPrice() {
		return marketPrice;
	}

	public void setMarketPrice(String marketPrice) {
		this.marketPrice = marketPrice;
	}

	public String getValidAmounts() {
		return validAmounts;
	}

	public void setValidAmounts(String validAmounts) {
		this.validAmounts = validAmounts;
	}
}
