#include "stdafx.h"
#include "SceneMgr.h"
#include"Start.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Ending.h"
SceneMgr*	SceneMgr::m_pInstance = nullptr;
SceneMgr::SceneMgr()
	: m_pScene(nullptr), m_eScene(SCENEID(STAGE1)), m_ePreScene(SCENEID(SCENE_END))
{
}


SceneMgr::~SceneMgr()
{
	Release();
}

void SceneMgr::Scene_Change(SCENEID eID)
{
	m_eScene = eID;

	if (m_ePreScene != m_eScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eScene)
		{

		case START:
			m_pScene = new Start;
			break;
		case STAGE1:
			m_pScene = new Stage3; 
			break;

		case STAGE2:
			m_pScene = new Stage1;
			break;
			
		case STAGE3:
			m_pScene = new Stage2;
			break;

		case ENDING:
			m_pScene = new Ending;
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eScene;
	}
}

void SceneMgr::Update(void)
{
	if (nullptr != m_pScene)
		m_pScene->Update();
}

void SceneMgr::Late_Update(void)
{
	if (m_pScene)
		m_pScene->Late_Update();
}

void SceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void SceneMgr::Release(void)
{
	Safe_Delete(m_pScene);
}
