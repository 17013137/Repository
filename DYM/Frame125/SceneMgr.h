#pragma once
#include "Include.h"
#include "Scene.h"

class SceneMgr
{
public:
	SceneMgr();
	~SceneMgr();

public:
	void		Set_Scene(SCENEID eScene) { m_eScene = eScene; }

public:
	void	Scene_Change(SCENEID eID);
	void	Update(void);
	void	Late_Update(void);
	void	Render(HDC _DC);
	void	Release(void);
public:
	static SceneMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new SceneMgr;

		return m_pInstance;
	}
	static void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static		SceneMgr*				m_pInstance;
	Scene*								m_pScene;
	SCENEID								m_eScene;
	SCENEID								m_ePreScene;
};

