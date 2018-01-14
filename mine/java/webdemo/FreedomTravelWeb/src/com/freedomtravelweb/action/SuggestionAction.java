package com.freedomtravelweb.action;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.struts2.ServletActionContext;

import com.freedomtravelweb.bean.Suggestion;
import com.freedomtravelweb.service.SuggestionService;

import java.util.List;

public class SuggestionAction extends ActionSupport{

	private Suggestion suggestion;
	private SuggestionService service;
	private String code;
	private List <Suggestion>list;
	
	//add fuction suggestion
	public String addFSuggestion(){
		
		String str = "error";
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		if(session.getAttribute("imagecode").equals(this.getCode())){//验证正确
			
			String username = (String)session.getAttribute("nameonline");//get logined username
			this.getSuggestion().setUsername(username);
			this.getSuggestion().setVerifyState("未处理");
			this.getService().addSuggestion(this.getSuggestion());
			//System.out.println("用户名:"+suggestion.getUsername());
			//System.out.println("问题类型:"+suggestion.getSuggestType());
			//System.out.println("问题内容:"+suggestion.getSuggestContent());
			str = "success";
		}
		
		return str;
	}
	
	//add complaint suggestion 
	public String addCSuggestion(){
		
		String str = "error";
		HttpServletRequest request = (HttpServletRequest)ActionContext.getContext().get(ServletActionContext.HTTP_REQUEST);
		HttpSession session = request.getSession();
		if(session.getAttribute("imagecode").equals(this.getCode())){//验证正确
			
			String username = (String)session.getAttribute("nameonline");//get logined username
			this.getSuggestion().setUsername(username);
			this.getSuggestion().setVerifyState("未处理");
			this.getService().addSuggestion(this.getSuggestion());
			//System.out.println("用户名:"+suggestion.getUsername());
			//System.out.println("问题类型:"+suggestion.getSuggestType());
			//System.out.println("问题内容:"+suggestion.getSuggestContent());
			str = "success";
		}
		return str;
	}
	
	public Suggestion getSuggestion() {
		return suggestion;
	}

	public void setSuggestion(Suggestion suggestion) {
		this.suggestion = suggestion;
	}

	public SuggestionService getService() {
		return service;
	}

	public void setService(SuggestionService service) {
		this.service = service;
	}

	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}

	public List<Suggestion> getList() {
		return list;
	}

	public void setList(List<Suggestion> list) {
		this.list = list;
	}
}
