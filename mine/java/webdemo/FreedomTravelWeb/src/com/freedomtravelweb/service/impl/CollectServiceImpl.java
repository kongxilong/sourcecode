package com.freedomtravelweb.service.impl;

import java.util.List;

import com.freedomtravelweb.bean.Collect;
import com.freedomtravelweb.dao.CollectDAO;
import com.freedomtravelweb.service.CollectService;

public class CollectServiceImpl implements CollectService {
	private CollectDAO collectDAO;
	public void saveCollect(Collect collect) {
		// TODO Auto-generated method stub
		this.collectDAO.saveCollect(collect);
	}
	public CollectDAO getCollectDAO() {
		return collectDAO;
	}
	public void setCollectDAO(CollectDAO collectDAO) {
		this.collectDAO = collectDAO;
	}
	public List findCollectByName(String username) {
		// TODO Auto-generated method stub
		return this.collectDAO.findCollectByName(username);
	}
	
}
