package com.freedomtravelweb.bean;

public class Admin {

	private Integer adminId;	//管理员ID
	private String adminName;	//管理员用户名
	private String password;	//管理员密码
	
	public Integer getAdminId() {
		return adminId;
	}
	public void setAdminId(Integer adminId) {
		this.adminId = adminId;
	}
	public String getAdminName() {
		return adminName;
	}
	public void setAdminName(String adminName) {
		this.adminName = adminName;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	
}
