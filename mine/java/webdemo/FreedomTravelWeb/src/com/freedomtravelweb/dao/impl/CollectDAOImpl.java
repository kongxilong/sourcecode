package com.freedomtravelweb.dao.impl;

import java.util.List;

import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import com.freedomtravelweb.bean.Collect;
import com.freedomtravelweb.dao.CollectDAO;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Session;
public class CollectDAOImpl  extends HibernateDaoSupport implements CollectDAO {
	private  final Log log = (Log) LogFactory.getLog(CollectDAOImpl.class);
	public void saveCollect(Collect collect) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("保存collect实例开始");
		}
		try{
			this.getHibernateTemplate().save(collect);
			if(log.isDebugEnabled()) {
				log.debug("保存collect实例成功");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()){
				log.error("保存collect实例失败",re);
			}
			throw re;
		}	
	}
	public List findCollectByName(String username) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("开始通过名字查找收藏夹信息");
		}
		try {
				String hql = "select collect.collectId,route.routeId,route.routeName,route.username,route.marketPrice,route.totalDays,route.imageDir from Route route,Collect collect where  route.routeId = collect.route.routeId and collect.username = ?";
				Session session = this.getSessionFactory().openSession();
				List list = (List)session.createQuery(hql).setString(0, username).list();
				return list;
		}catch(RuntimeException re) {
			log.debug("通过名字查找收藏夹信息失败",re);
			throw re;
		}
	}

}
