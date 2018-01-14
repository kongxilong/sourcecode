package com.freedomtravelweb.service;
import com.freedomtravelweb.bean.User;

import java.util.List;
public interface UserService {
	public List<User> findAll();
	public void save(User user);
	public void delete(User user);
	public User findUserById(Integer id);
	public List<User> findUserByUserName(String username);
	public void update(User user);
	public User loginCheck(User user);
	public void passwordEdit(User user);
	public void iconEdit(User user);
	public List showDataByName(String name);
	public User showZoneByName(String name);
	public void dataUpdate(User user);
	
}
