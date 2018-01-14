package com.freedomtravelweb.dao.impl;

import com.freedomtravelweb.bean.*;
import com.freedomtravelweb.dao.AdminDAO;
import com.freedomtravelweb.page.*;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

public class AdminDAOImpl extends HibernateDaoSupport implements AdminDAO{
	
	private  final Log log = LogFactory.getLog(UserDAOImpl.class);
	
	public Admin adminLoginCheck(Admin admin){
		Admin adm = null;
		List <Admin>list = null;
		try{
			String str = "from Admin admin where admin.adminName=? and admin.password=?";
			list = this.getHibernateTemplate().find(str,new String[]{admin.getAdminName(),admin.getPassword()});
			if(list.size() >= 1){
				adm = list.get(0);//获取查到的第一个对象
			}
			log.debug("find successful");
		}catch(RuntimeException re) {
			log.error("find failed",re);
			throw re;
		}
		return adm;
	}
	
	public void deleteSuggestion(Integer suggestionId){//ToTest
		
		try{
			Suggestion sugt = (Suggestion)this.getHibernateTemplate().get(Suggestion.class, suggestionId);
			this.getHibernateTemplate().delete(sugt);
			log.debug("delete  successful");
		}catch(RuntimeException re){
			
			log.error("delete failed",re);
			throw re;
		}
		
	}
	
	public void changeSuggestion(Integer suggestionId,String changeWords,String admin){//ToTest
		
		try{
			
			Suggestion sugt = (Suggestion)this.getHibernateTemplate().get(Suggestion.class, suggestionId);
			sugt.setVerifyState(changeWords);
			sugt.setVerifyAdminName(admin);
			this.getHibernateTemplate().update(sugt);
			log.debug("delete  suggestion successful");
		}catch(RuntimeException re){
			
			log.error("delete suggestion failed",re);
			throw re;
		}
	}
	public int findAllSuggestionCount(){
		
		List <Suggestion>list = null;
		try{
			String hql = "from Suggestion suggestion order by suggestion.suggestionId desc";
			list = this.getHibernateTemplate().find(hql);
			log.debug("find all suggestion  successful");
		}catch(RuntimeException re){
			
			log.error("find all suggestion failed",re);
			throw re;
		}
		return list.size();
		
	}
	public List<Suggestion> findAllSuggestion(Page page){
			
		Session session = getSession();		
		Query query = session.createQuery("from Suggestion suggestion order by suggestion.suggestionId desc");
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();
			
	}
	
	public void deleteUser(Integer userId){
		
		try{
			User user = (User)this.getHibernateTemplate().get(User.class, userId);
			this.getHibernateTemplate().delete(user);
			log.debug("delete  user successful");
		}catch(RuntimeException re){
			
			log.error("delete user failed",re);
			throw re;
		}
		
	}

	public int findAllUserCount(){
		
		List <User>list = null;
		try{
			String hql = "from User user order by user.userid desc";
			list = this.getHibernateTemplate().find(hql);
			log.debug("find all user  successful");
		}catch(RuntimeException re){
			
			log.error("find all user failed",re);
			throw re;
		}
		return list.size();
		
	}
	public List<User> findAllUser(Page page){
			
		Session session = getSession();		
		Query query = session.createQuery("from User user order by user.userid desc");
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();
			
	}
	
	public void deleteRoute(Integer routeId){
		
		try{
			Route route = (Route)this.getHibernateTemplate().get(Route.class, routeId);
			this.getHibernateTemplate().delete(route);
			log.debug("delete  route successful");
		}catch(RuntimeException re){
			
			log.error("delete route failed",re);
			throw re;
		}
		
	}

	public int findAllRouteCount(){
		
		List <Route>list = null;
		try{
			String hql = "from Route route order by route.routeId desc";
			list = this.getHibernateTemplate().find(hql);
			log.debug("find all route  successful");
		}catch(RuntimeException re){
			
			log.error("find all route failed",re);
			throw re;
		}
		return list.size();
		
	}
	
	public List<Route> findAllRoute(Page page){
			
		Session session = getSession();		
		Query query = session.createQuery("from Route route order by route.routeId desc");
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();
			
	}
	
	public void deleteDeal(Integer dealId){
		
		try{
			Deal deal = (Deal)this.getHibernateTemplate().get(Deal.class, dealId);
			this.getHibernateTemplate().delete(deal);
			log.debug("delete  deal successful");
		}catch(RuntimeException re){
			
			log.error("delete deal failed",re);
			throw re;
		}
		
	}

	public int findAllDealCount(){
		
		List list = null;
		try{
			String hql = "select route.routeId,route.routeName,route.username,deal.purchaserName,deal.dealNum,deal.totalMktPrice,deal.tradeTime,deal.dealId from Route route,Deal deal where  route.routeId = deal.route.routeId";
			list = this.getHibernateTemplate().find(hql);
			//System.out.println("测试list："+list.size());
			log.debug("find all deal  successful");
		}catch(RuntimeException re){
			
			log.error("find all deal failed",re);
			throw re;
		}
		return list.size();
		
	}
	
	public List findAllDeal(Page page){
			
		Session session = getSession();		
		Query query = session.createQuery("select route.routeId,route.routeName,route.username,deal.purchaserName,deal.dealNum,deal.totalMktPrice,deal.tradeTime,deal.dealId from Route route,Deal deal where  route.routeId = deal.route.routeId");
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();
			
	}
}
