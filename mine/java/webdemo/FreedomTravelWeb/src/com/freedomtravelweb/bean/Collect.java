package com.freedomtravelweb.bean;

public class Collect {
	
	private Integer collectId; 		//收藏表Id
	private String username;		//收藏者用户名
	private Route route;			//线路对象
	private User user;				//用户对象
	
	public Integer getCollectId() {
		return collectId;
	}
	public void setCollectId(Integer collectId) {
		this.collectId = collectId;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public Route getRoute() {
		return route;
	}
	public void setRoute(Route route) {
		this.route = route;
	}
	public User getUser() {
		return user;
	}
	public void setUser(User user) {
		this.user = user;
	}
}
