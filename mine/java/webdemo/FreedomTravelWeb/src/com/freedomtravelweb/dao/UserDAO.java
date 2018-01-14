package com.freedomtravelweb.dao;
import com.freedomtravelweb.bean.User;

import java.util.List;
public interface UserDAO {
	public void saveUser(User user);
	public void removeUser(User user);
	public User findUserById(Integer id);
	public List<User> findUserByUserName(String username);
	public List<User> findAllUsers();
	public void updateUser(User user);
	public User findUser(User user);
	public void passwordEdit(User user);
	public void iconEdit(User user);
	public List showDataByName(String name);
	public User showZoneByName(String name);
	public void dataUpdate(User user);
}
