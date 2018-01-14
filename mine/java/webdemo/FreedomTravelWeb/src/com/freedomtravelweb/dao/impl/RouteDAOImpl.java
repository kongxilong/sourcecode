package com.freedomtravelweb.dao.impl;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import com.freedomtravelweb.bean.Route;
import com.freedomtravelweb.bean.RouteComments;
import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.dao.RouteDAO;
import com.freedomtravelweb.page.Page;

public class RouteDAOImpl extends HibernateDaoSupport implements RouteDAO {
	private  final Log log = LogFactory.getLog(UserDAOImpl.class);
	public List<Route> findAllRoute() {
		// TODO Auto-generated method stub
		return null;
	}
	public Route findRouteById(Integer routeId) {
		// TODO Auto-generated method stub
		Route findRoute = null;
		Object[] args = {routeId};
		try{															
			List<Route> list = this.getHibernateTemplate().find("from Route where routeId=?",args);		
			if (list.size() >= 1){
			   findRoute = (Route)list.get(0);
			}
			log.debug("find successful");
		}catch(RuntimeException re) {
			log.error("find failed",re);
			throw re;
		}
		return findRoute;	}
	public List<User> findUserByUserName(String username) {
		// TODO Auto-generated method stub
		
		return null;
	}
	public void removeRoute(Route route) {
		// TODO Auto-generated method stub

	}

	public void saveRoute(Route route) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("saving route instance");
		}
		try{
			this.getHibernateTemplate().save(route);
			if(log.isDebugEnabled()) {
				log.debug("save successful");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()){
				log.error("save failed",re);
			}
			throw re;
		}
	}

	public void updateRoute(Route route) {
		// TODO Auto-generated method stub

	}
	public int findRouteByKeywordsCount(String searchWords){
		
		List <Route> list = null;
		try{
			list = this.getHibernateTemplate().find("from Route  where concat(routeName,cityType,keyWord,username) like '%"+searchWords+"%'");		
			log.debug("find successful");
		}catch(RuntimeException re) {
			log.error("find failed",re);
			throw re;
		}
		return list.size();
	}
	public List<Route> findRouteByKeywords(Page page,String searchWords){
		
		Session session = getSession();		
		Query query = session.createQuery("from Route route where concat(routeName,cityType,keyWord,username) like '%"+searchWords+"%'");
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();		
	}

	public List<Route> findRouteAddByUsername(Page page, String userNameAddRoute) {
		// TODO Auto-generated method stub
		String hql = "from Route  where username = ? ";		
		Query query = getSession().createQuery(hql).setString(0, userNameAddRoute);
		query.setFirstResult(page.getBeginIndex());	
		query.setMaxResults(page.getEveryPage());	
		return query.list();	
	}
	public int findRouteByUsernameCount(String userNameAddRoute) {
		// TODO Auto-generated method stub
		List <Route> list = null;
		try{
			String hql = "from Route  where username = ? ";
			Session session = this.getSessionFactory().openSession();
			list =(List<Route>)session.createQuery(hql).setString(0, userNameAddRoute).list();	
			log.debug("find successful");
		}catch(RuntimeException re) {
			log.error("find failed",re);
			throw re;
		}
		return list.size();
	}
	public List<Route> findRouteByTime(int num) {
		// TODO Auto-generated method stub
		List <Route> list = null;
		String hql ="from Route order by publishTime desc";
		try{
			Session session = getSession();	
			Query query = session.createQuery(hql);	
			query.setFirstResult(0);	
			query.setMaxResults(num);
			list = query.list();
		}catch(RuntimeException re){
			log.debug("查找失败", re);
			throw re;
		}
		return list;
	}
	public Route findRouteInfoById(int id) {
		// TODO Auto-generated method stub
		List <Route> list = null;
		String hql ="from Route route where route.routeId = ?";
		try{
			Query query = getSession().createQuery(hql);	
			query.setParameter(0, id);
			list = query.list();
			if (!list.isEmpty()) {
				return list.get(0);
			}
		}catch(RuntimeException re){
			log.debug("查找失败", re);
			throw re;
		}
		return null;
	}
	public void addRouteComments(RouteComments rc) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("saving RouteComments instance");
		}
		try{
			this.getHibernateTemplate().save(rc);
			if(log.isDebugEnabled()) {
				log.debug("save RouteComments successful");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()){
				log.error("save RouteComments failed",re);
			}
			throw re;
		}
	}
	public List findRouteCommentsById(int Id) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("开始通过routeId查找路线评论信息");
		}
		try {
				String hql = "select rc.buyer,route.imageDir,route.routeName,rc.commentTime from Route route,RouteComments rc where route.routeId = rc.route.rId and route.routeId = ?";
				Session session = this.getSessionFactory().openSession();
				List list = (List)session.createQuery(hql).setParameter(0, Id).list();
				return list;
		}catch(RuntimeException re) {
			log.debug("通过routeId查找路线评论信息失败",re);
			throw re;
		}
	}
}
