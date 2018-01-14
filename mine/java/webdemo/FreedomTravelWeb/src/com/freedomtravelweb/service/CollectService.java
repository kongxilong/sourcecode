package com.freedomtravelweb.service;

import java.util.List;

import com.freedomtravelweb.bean.Collect;

public interface CollectService {
	public void saveCollect(Collect collect);
	public List findCollectByName(String username);
}
