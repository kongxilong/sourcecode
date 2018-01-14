package com.freedomtravelweb.bean;

public class User {
	private Integer userid;
	private String username;
	private String password;
	private String email;
	private String membershipstyle;
	private String realname;
	private String sex;
	private String postion;
	private String companyname;
	private String localcity;
	private String mainbussiness;
	private String fixedphonenum;
	private String address;
	private String iconDir;
	
	//这里设置为String 类型为了防止用户注册时将cellphone输成非int类型时  程序会报错甚至崩溃但是会检查收入的是否为int型的
	private String cellphone;
	



	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getMembershipstyle() {
		return membershipstyle;
	}

	public void setMembershipstyle(String membershipstyle) {
		this.membershipstyle = membershipstyle;
	}

	public String getRealname() {
		return realname;
	}

	public void setRealname(String realname) {
		this.realname = realname;
	}

	public String getSex() {
		return sex;
	}

	public void setSex(String sex) {
		this.sex = sex;
	}

	public String getPostion() {
		return postion;
	}

	public void setPostion(String postion) {
		this.postion = postion;
	}

	public String getCompanyname() {
		return companyname;
	}

	public void setCompanyname(String companyname) {
		this.companyname = companyname;
	}

	public String getLocalcity() {
		return localcity;
	}

	public void setLocalcity(String localcity) {
		this.localcity = localcity;
	}

	public String getMainbussiness() {
		return mainbussiness;
	}

	public void setMainbussiness(String mainbussiness) {
		this.mainbussiness = mainbussiness;
	}

	public String getFixedphonenum() {
		return fixedphonenum;
	}

	public void setFixedphonenum(String fixedphonenum) {
		this.fixedphonenum = fixedphonenum;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getCellphone() {
		return cellphone;
	}

	public void setCellphone(String cellphone) {
		this.cellphone = cellphone;
	}

	public Integer getUserid() {
		return userid;
	}

	public void setUserid(Integer userid) {
		this.userid = userid;
	}

	public String getIconDir() {
		return iconDir;
	}

	public void setIconDir(String iconDir) {
		this.iconDir = iconDir;
	}



}
