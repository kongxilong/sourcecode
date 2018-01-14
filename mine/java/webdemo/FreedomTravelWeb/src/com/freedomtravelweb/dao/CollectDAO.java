package com.freedomtravelweb.dao;
import java.util.List;
import com.freedomtravelweb.bean.Collect;

public interface CollectDAO {
	public void saveCollect(Collect collect);
	public List findCollectByName(String username);
}
