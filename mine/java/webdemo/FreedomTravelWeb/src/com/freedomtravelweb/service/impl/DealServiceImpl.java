package com.freedomtravelweb.service.impl;

import java.util.List;

import com.freedomtravelweb.bean.Deal;
import com.freedomtravelweb.bean.DealTemp;
import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.dao.DealDAO;
import com.freedomtravelweb.service.DealService;

public class DealServiceImpl implements DealService {
	private DealDAO dealDAO;
	public void addToDeal(Deal deal) {
		// TODO Auto-generated method stub
		this.dealDAO.addToDeal(deal);
	}
	public DealDAO getDealDAO() {
		return dealDAO;
	}
	public void setDealDAO(DealDAO dealDAO) {
		this.dealDAO = dealDAO;
	}
	public List findMyDealByName(String purchaserName) {
		// TODO Auto-generated method stub
		return this.dealDAO.findDealByName(purchaserName);
	}
	public void updateDeal(Deal deal) {
		// TODO Auto-generated method stub
		this.dealDAO.update(deal);
	}
	public List findInfoByDealId(Integer dealId) {
		// TODO Auto-generated method stub
		
		return this.dealDAO.findDealInfoByDealId(dealId);
	}
	public void updateDealState(Deal deal) {
		// TODO Auto-generated method stub
		this.dealDAO.updateDealState(deal);
	}
	public List findAllDealByName(String name) {
		// TODO Auto-generated method stub
		return this.dealDAO.findAllDealByName(name);
	}
	public boolean isUserExist(DealTemp dealTemp) {
		// TODO Auto-generated method stub
		
		return this.dealDAO.isUserExist(dealTemp);
	}
	public void finishDeal(Deal deal) {
		// TODO Auto-generated method stub
		this.dealDAO.finishDeal(deal);
	}
	public void deleteDeal(Deal deal) {
		// TODO Auto-generated method stub
		this.dealDAO.deleteDeal(deal);
	}
}
