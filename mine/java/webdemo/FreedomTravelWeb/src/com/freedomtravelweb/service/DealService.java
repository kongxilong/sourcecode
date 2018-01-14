package com.freedomtravelweb.service;

import java.util.List;

import com.freedomtravelweb.bean.Deal;
import com.freedomtravelweb.bean.DealTemp;
import com.freedomtravelweb.bean.Route;

public interface DealService {
	public void addToDeal(Deal deal);
	public List findMyDealByName(String purchaserName);
	public void updateDeal(Deal deal);
	public void updateDealState(Deal deal);
	public List findInfoByDealId(Integer dealId);
	public List findAllDealByName(String name);
	public boolean isUserExist(DealTemp dealTemp);
	public void finishDeal(Deal deal);
	public void deleteDeal(Deal deal);
}
