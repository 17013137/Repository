#include "stdafx.h"
#include "MainGame.h"
#include "Monster.h"
#include "Boss.h"
#include "AbstractFactory.h"
#include "MyObjMgr.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"

CMainGame::CMainGame()
{
	time2 = 0;
	cnt = 0;
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(void)
{
	m_DC = GetDC(g_hWnd);
	SceneMgr::Get_Instance()->Scene_Change(START);

}
void CMainGame::Update(void)
{
	
	SceneMgr::Get_Instance()->Update();
	MyObjMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update(void)
{
	SceneMgr::Get_Instance()->Late_Update();
	MyObjMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render(void)
{
	Rectangle(m_DC, 0, 0, WINCX, WINCY);

	SceneMgr::Get_Instance()->Render(m_DC);
}

void CMainGame::Release(void)
{
	SceneMgr::Get_Instance()->Destroy_Instance();
	MyObjMgr::Get_Instance()->Release();
	ReleaseDC(g_hWnd, m_DC);
}
