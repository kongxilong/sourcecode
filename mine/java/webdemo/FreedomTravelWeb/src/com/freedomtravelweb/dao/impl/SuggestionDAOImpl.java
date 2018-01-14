package com.freedomtravelweb.dao.impl;

import com.freedomtravelweb.dao.SuggestionDAO;
import com.freedomtravelweb.bean.Suggestion;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;



public class SuggestionDAOImpl extends HibernateDaoSupport implements SuggestionDAO{
	
	private  final Log log = LogFactory.getLog(UserDAOImpl.class);
	
	public void addSuggestion(Suggestion suggestion){//OK
		try{
			
			this.getHibernateTemplate().save(suggestion);
			log.debug("find successful");
		}catch(RuntimeException re) {

			log.error("save failed",re);
			throw re;
		}
	}
	                                                               
}
