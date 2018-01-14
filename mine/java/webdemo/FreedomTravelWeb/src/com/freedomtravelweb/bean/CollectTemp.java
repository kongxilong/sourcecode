package com.freedomtravelweb.bean;

public class CollectTemp {
	private int collectId;
	private int routeId;
	private String routeName;
	private String username;
	private String price;
	private String travelTime;
	private String imageDir;
	public String getImageDir() {
		return imageDir;
	}
	public void setImageDir(String imageDir) {
		this.imageDir = imageDir;
	}
	public int getCollectId() {
		return collectId;
	}
	public void setCollectId(int collectId) {
		this.collectId = collectId;
	}
	public int getRouteId() {
		return routeId;
	}
	public void setRouteId(int routeId) {
		this.routeId = routeId;
	}
	public String getRouteName() {
		return routeName;
	}
	public void setRouteName(String routeName) {
		this.routeName = routeName;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPrice() {
		return price;
	}
	public void setPrice(String price) {
		this.price = price;
	}
	public String getTravelTime() {
		return travelTime;
	}
	public void setTravelTime(String travelTime) {
		this.travelTime = travelTime;
	}
}
