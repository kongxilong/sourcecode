package com.freedomtravelweb.dao.impl;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import com.freedomtravelweb.bean.Deal;
import com.freedomtravelweb.bean.DealTemp;
import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.dao.DealDAO;


public class DealDAOImpl extends HibernateDaoSupport implements DealDAO {
	private  final Log log = (Log) LogFactory.getLog(UserDAOImpl.class);
	public void addToDeal(Deal deal) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("saving deal instance");
		}
		try{
			this.getHibernateTemplate().save(deal);
			if(log.isDebugEnabled()) {
				log.debug("save deal successful");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()){
				log.error("save deal failed",re);
			}
			throw re;
		}	
	}
	public List findDealByName(String name) {
		// TODO Auto-generated method stub
		
		if(log.isDebugEnabled()) {
			log.debug("开始通过名字查找订单信息");
		}
		try {
				String hql = "select route.username,route.imageDir,route.routeName,route.totalDays,route.keyWord,route.marketPrice,deal.dealId from Route route,Deal deal where  route.routeId = deal.route.routeId and deal.purchaserName = ?";
				Session session = this.getSessionFactory().openSession();
				List list = (List)session.createQuery(hql).setString(0, name).list();
				return list;
		}catch(RuntimeException re) {
			log.debug("通过名字查找订单信息失败",re);
			throw re;
		}
	}
	public void update(Deal deal) {
		// TODO Auto-generated method stub
		log.debug("开始修改deal对象");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update Deal d set d.sellerName = ?, d.dealNum = ?, d.totalMktPrice = ?, d.tradeTime = ? where d.dealId= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, deal.getSellerName());
			query.setParameter(1, deal.getDealNum());
			query.setParameter(2, deal.getTotalMktPrice());
			query.setParameter(3, deal.getTradeTime());
			query.setParameter(4, deal.getDealId());
			query.executeUpdate();
			log.debug("修改deal对象成功");
		}catch(RuntimeException re){
			log.debug("修改deal对象失败", re);
			throw re;
		}
	}
	public List findDealInfoByDealId(Integer dealId) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("开始通过Id查找订单信息");
		}
		try {
				int Id;
				String hql = "select route.username,route.marketPrice,route.routeName from Route route,Deal deal where  route.routeId = deal.route.routeId and deal.dealId = ?";
				Session session = this.getSessionFactory().openSession();
				Query query   = session.createQuery(hql);
				query.setParameter(0, dealId);
				List list = query.list();
				log.debug("通过Id查询Info成功");
				return list;
		}catch(RuntimeException re) {
			log.debug("通过Id查找订单信息失败",re);
			throw re;
		}
		
	}
	public void updateDealState(Deal deal) {
		// TODO Auto-generated method stub
		log.debug("开始修改dealState字段");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update Deal d set d.dealState = ? where d.dealId= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, deal.getDealState());
			query.setParameter(1, deal.getDealId());
			query.executeUpdate();
			log.debug("修改dealState字段成功");
		}catch(RuntimeException re){
			log.debug("修改dealState字段失败", re);
			throw re;
		}
	}
	public List findAllDealByName(String name) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("开始通过名字查找我的订单信息");
		}
		try {
				String hql = "select route.username,route.imageDir,route.routeName,route.totalDays,route.keyWord,route.marketPrice,deal.dealId,deal.dealNum,deal.dealState,deal.dealOver,route.routeId from Route route,Deal deal where  concat(deal.purchaserName,deal.sellerName) like '%"+name+"%' and route.routeId = deal.route.routeId";
				Session session = this.getSessionFactory().openSession();
				List list = (List)session.createQuery(hql).list();
				return list;
		}catch(RuntimeException re) {
			log.debug("通过名字查找我的订单信息失败",re);
			throw re;
		}
	}
	public boolean isUserExist(DealTemp dealTemp) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("开始通过名字和密码查找用户名是否存在");
		}
		try {
				String hql = "from User user where  user.username = ? and user.password = ?";
				Session session = this.getSessionFactory().openSession();
				Query query = session.createQuery(hql);
				query.setParameter(0, dealTemp.getUsername());
				query.setParameter(1, dealTemp.getPassword());
				List list = query.list();
				return !list.isEmpty();
		}catch(RuntimeException re) {
			log.debug("通过名字查找我的订单信息失败",re);
			throw re;
		}
	}
	public void finishDeal(Deal deal) {
		// TODO Auto-generated method stub
		log.debug("开始修改dealOver字段");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update Deal d set d.dealOver = ? where d.dealId= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, deal.getDealOver());//将交易状态改为1
			query.setParameter(1, deal.getDealId());
			query.executeUpdate();
			log.debug("修改dealOver字段成功");
		}catch(RuntimeException re){
			log.debug("修改dealOver字段失败", re);
			throw re;
		}
	}
	public void deleteDeal(Deal deal) {
		// TODO Auto-generated method stub
		this.getHibernateTemplate().delete(deal);
	}
}
