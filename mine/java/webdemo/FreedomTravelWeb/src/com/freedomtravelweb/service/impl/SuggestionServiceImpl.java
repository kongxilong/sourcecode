package com.freedomtravelweb.service.impl;

import com.freedomtravelweb.service.SuggestionService;
import com.freedomtravelweb.bean.Suggestion;
import com.freedomtravelweb.dao.SuggestionDAO;
import java.util.List;

public class SuggestionServiceImpl implements SuggestionService{
	
	private SuggestionDAO suggestionDAO;
	
	public void addSuggestion(Suggestion suggestion){
		
		this.getSuggestionDAO().addSuggestion(suggestion);
	}
	                                                   
	public SuggestionDAO getSuggestionDAO() {
		return suggestionDAO;
	}

	public void setSuggestionDAO(SuggestionDAO suggestionDAO) {
		this.suggestionDAO = suggestionDAO;
	}
}
