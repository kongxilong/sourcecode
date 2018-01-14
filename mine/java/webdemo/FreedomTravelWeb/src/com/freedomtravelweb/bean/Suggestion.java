package com.freedomtravelweb.bean;

public class Suggestion {

	private Integer suggestionId;	//建议表单ID
	private String username;		//建议者的用户名
	private String suggestType;		//建议的类型学
	private String suggestContent;	//建议的具体内容
	private String verifyState;		//该建议被处理的状态
	private String verifyAdminName;	//处理建议的管理员用户名
	
	public Integer getSuggestionId() {
		return suggestionId;
	}
	public void setSuggestionId(Integer suggestionId) {
		this.suggestionId = suggestionId;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getSuggestContent() {
		return suggestContent;
	}
	public void setSuggestContent(String suggestContent) {
		this.suggestContent = suggestContent;
	}
	public String getVerifyAdminName() {
		return verifyAdminName;
	}
	public void setVerifyAdminName(String verifyAdminName) {
		this.verifyAdminName = verifyAdminName;
	}
	public String getSuggestType() {
		return suggestType;
	}
	public void setSuggestType(String suggestType) {
		this.suggestType = suggestType;
	}
	public String getVerifyState() {
		return verifyState;
	}
	public void setVerifyState(String verifyState) {
		this.verifyState = verifyState;
	}
}
