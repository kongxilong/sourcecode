package com.freedomtravelweb.dao;
import java.util.List;

import com.freedomtravelweb.bean.*;
public interface DealDAO {
	public  void addToDeal(Deal deal);
	public List findDealByName(String name);
	public List findAllDealByName(String name);
	public void update(Deal deal);
	public List findDealInfoByDealId(Integer dealId);
	public void updateDealState(Deal deal);
	public boolean isUserExist(DealTemp dealTemp);
	public void finishDeal(Deal deal);
	public void deleteDeal(Deal deal);
}
