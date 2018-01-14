package com.freedomtravelweb.dao.impl;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.dao.UserDAO;


public class UserDAOImpl extends HibernateDaoSupport implements UserDAO {
	private  final Log log = LogFactory.getLog(UserDAOImpl.class);
	@SuppressWarnings("unchecked") 
	public List<User> findAllUsers() {  
		if (log.isDebugEnabled()) {
			log.debug("finding AllUser  instance");
		}
		try{
			String hql = "from User user order by user.id desc";
			if(log.isDebugEnabled()) {
				log.debug("finding AllUser  success");
			}
			return  (List<User>)this.getHibernateTemplate().find(hql);	
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()) {
				log.debug("finding AllUser  failed", re);
			}
			throw re;
		}
	}

	public User findUserById(Integer id) {
		if(log.isDebugEnabled()) {
			log.debug("finding User by id instance");
		}
		try{
			User user = (User)this.getHibernateTemplate().get(User.class, id);
			log.debug("finding User by id success");
			return user;
		}catch(RuntimeException re) {
			log.debug("finding User by id failed", re);
			throw re;
		}
	}
	public List<User> findUserByUserName(String username) {
		if(log.isDebugEnabled()) {
			log.debug("finding user by name instance");
		}
		try{
			String hql = "from User  where username = ? ";
			Session session = this.getSessionFactory().openSession();
			List<User> list = session.createQuery(hql).setString(0, username).list();
			//User user = (User)this.getHibernateTemplate().get(User.class, username);
			if(log.isDebugEnabled()) {
			log.debug("find user by name success");
			}
			return list;
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()) {
				log.debug("find user by name failed", re);
			}
			throw re;
		}
	}

	public void removeUser(User user) {
		if(log.isDebugEnabled()) {
			log.debug("deleting User instance");
		}
		try{
			this.getHibernateTemplate().delete(user);
			if(log.isDebugEnabled()) {
				log.debug("delete User successful");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()) {
				log.error("delete User failed",re);
			}
			throw re;
		}	
	}

	public void saveUser(User user) {
		if(log.isDebugEnabled()) {
			log.debug("saving User instance");
		}
		try{
			this.getHibernateTemplate().save(user);
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

	public void updateUser(User user) {
		if(log.isDebugEnabled()){
			log.debug("updating User instance");
		}
		try{
			this.getHibernateTemplate().update(user);
			if(log.isDebugEnabled()){
				log.debug("update successful");
			}
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()){
				log.error("update failed",re);
			}
			throw re;
		}	
	}
	public User findUser(User user){
		User user1 = null;
		List <User>list = null;
		log.debug("find User instance");
		try{
			String str = "from User user where user.username=? and user.password=?";
			list = this.getHibernateTemplate().find(str,new String[]{user.getUsername(),user.getPassword()});
			if(list.size() >= 1){
				user1 = list.get(0);//获取查到的第一个对象
			}
			log.debug("find successful");
		}catch(RuntimeException re) {
			log.error("find failed",re);
			throw re;
		}
		return user1;
	}

	public void passwordEdit(User user) {
		// TODO Auto-generated method stub
		log.debug("更改用户密码实例开始执行");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update User user set user.password = ? where user.username= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, user.getPassword());
			query.setParameter(1, user.getUsername());
			query.executeUpdate();
			log.debug("更改用户密码实例成功");
		}catch(RuntimeException re) {
			log.error("更改用户密码实例失败",re);
			throw re;
		}
	}

	public void iconEdit(User user) {
		// TODO Auto-generated method stub
		log.debug("更改用户头像实例开始执行");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update User user set user.iconDir = ? where user.username= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, user.getIconDir());
			query.setParameter(1, user.getUsername());
			query.executeUpdate();
			log.debug("更改用户头像实例成功");
		}catch(RuntimeException re) {
			log.error("更改用户头像实例失败",re);
			throw re;
		}
	}

	public List showDataByName(String name) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("finding user by name instance");
		}
		try{
			String hql = "select user.iconDir,user.username,user.realname, user.companyname,user.address,user.cellphone,user.email from User user where user.username = ? ";
			Session session = this.getSessionFactory().openSession();
			List list = session.createQuery(hql).setString(0, name).list();
			if(log.isDebugEnabled()) {
			log.debug("find user by name success");
			}
			return list;
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()) {
				log.debug("find user by name failed", re);
			}
			throw re;
		}
	}

	public void dataUpdate(User user) {
		// TODO Auto-generated method stub
		log.debug("更改用户资料实例开始执行");
		try{
			Session s = getSessionFactory().openSession();
			String hql = "update User user set user.realname = ?,user.companyname = ?,user.address = ?,user.cellphone = ?,user.email = ? where user.username= ?";
			Query query = s.createQuery(hql);
			query.setParameter(0, user.getRealname());
			query.setParameter(1, user.getCompanyname());
			query.setParameter(2, user.getAddress());
			query.setParameter(3, user.getCellphone());
			query.setParameter(4, user.getEmail());
			query.setParameter(5, user.getUsername());
			query.executeUpdate();
			log.debug("更改用户资料实例成功");
		}catch(RuntimeException re) {
			log.error("更改用户资料实例失败",re);
			throw re;
		}
	}

	public User showZoneByName(String name) {
		// TODO Auto-generated method stub
		if(log.isDebugEnabled()) {
			log.debug("finding iconDir by name instance");
		}
		try{
			String hql = "from User user where user.username = ? ";
			Session session = this.getSessionFactory().openSession();
			List list = session.createQuery(hql).setString(0, name).list();
			if(log.isDebugEnabled()) {
				log.debug("find iconDir by name success");
				}
			if(list.size()>0){
				User user = (User)list.get(0);
				return user;
			}
			return null;
		}catch(RuntimeException re) {
			if(log.isDebugEnabled()) {
				log.debug("find iconDir by name failed", re);
			}
			throw re;
		}
	}
}
