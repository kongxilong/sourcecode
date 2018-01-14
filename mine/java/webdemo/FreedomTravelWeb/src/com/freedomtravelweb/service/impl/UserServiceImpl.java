package com.freedomtravelweb.service.impl;

import java.util.List;

import com.freedomtravelweb.bean.User;
import com.freedomtravelweb.dao.UserDAO;
import com.freedomtravelweb.service.UserService;

public class UserServiceImpl implements UserService {

	private UserDAO userDAO;
	public UserDAO getUserDAO() {
		return userDAO;
	}

	public void setUserDAO(UserDAO userDAO) {
		this.userDAO = userDAO;
	}

	public void delete(User user) {
		
		this.userDAO.removeUser(user);
	}

	public List<User> findAll() {
		
		return this.userDAO.findAllUsers();
	}

	public User findUserById(Integer id) {
	
		return this.userDAO.findUserById(id);
	}
	public List<User> findUserByUserName(String username) {
		
		return this.userDAO.findUserByUserName(username);
	}
	public void save(User user) {
		
		this.userDAO.saveUser(user);
	}

	public void update(User user) {
	
		this.userDAO.updateUser(user);
	}

	public User loginCheck(User user){
		
		return this.userDAO.findUser(user);
	}

	public void passwordEdit(User user) {
		// TODO Auto-generated method stub
		 this.getUserDAO().passwordEdit(user);
	}

	public void iconEdit(User user) {
		// TODO Auto-generated method stub
		this.userDAO.iconEdit(user);
	}

	public List showDataByName(String name) {
		// TODO Auto-generated method stub
		return this.userDAO.showDataByName(name);
	}

	public void dataUpdate(User user) {
		// TODO Auto-generated method stub
		this.userDAO.dataUpdate(user);
	}

	public User showZoneByName(String name) {
		// TODO Auto-generated method stub
		return this.userDAO.showZoneByName(name);
	}

	
}
