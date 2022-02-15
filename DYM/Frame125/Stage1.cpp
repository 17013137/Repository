#include "stdafx.h"
#include "Stage1.h"
#include "Monster.h"
#include "Boss.h"
#include "AbstractFactory.h"
#include "MyObjMgr.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "SceneMgr.h"

Stage1::Stage1()
{
	time2 = 0;
	cnt = 0;
}


Stage1::~Stage1()
{
	Release();
}

void Stage1::Initialize(void)
{
	CObj* object = CAbstractFactory<CPlayer>::Create(400, 400);
	MyObjMgr::Get_Instance()->Add_Object(PLAYER, object);

}

void Stage1::Update(void)
{
	CObj* object;
	if (GetAsyncKeyState(VK_F2)) {
		SceneMgr::Get_Instance()->Scene_Change(STAGE3);
	}
	if (time2 == 0) {
		time2 = GetTickCount();
	}

	if (cnt < 20 && time2 + 1000 < GetTickCount()) {
		cnt++;
	
		rand();
		object = CAbstractFactory<CMonster>::Create(rand() % 600 + 100, 0);
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, object);
		object = CAbstractFactory<CMonster>::Create(0, rand() % 600 + 100);
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, object);
		object = CAbstractFactory<CMonster>::Create(800, rand() % 600 + 100);
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, object);
		object = CAbstractFactory<CMonster>::Create(rand() % 600 + 100, 800);
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, object);
		time2 = 0;
	}

	if (cnt == 20) {
		MyObjMgr::Get_Instance()->Get_ObjList(MONSTER)->clear();
		cnt++;
		object = CAbstractFactory<Boss>::Create(400.f, 100.f);
		MyObjMgr::Get_Instance()->Add_Object(BOSS, object);
	}

	MyObjMgr::Get_Instance()->Update();
}

void Stage1::Late_Update(void)
{
	srand(time(NULL));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(PLAYER), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(PLAYER), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER_BULLET));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(BOSS));
	MyObjMgr::Get_Instance()->Late_Update();
}

void Stage1::Render(HDC _DC)
{
	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(204, 204, 204));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(_DC, hbr);

	Rectangle(_DC, 30, 30, WINCX - 30, WINCY - 30);

	hbr = (HBRUSH)SelectObject(_DC, hbrold);
	::DeleteObject(hbr);






	//±è
	HBRUSH hbr2;
	hbr2 = CreateSolidBrush(RGB(82, 73, 74));
	HBRUSH hbrold2;
	hbrold2 = (HBRUSH)::SelectObject(_DC, hbr2);

	Rectangle(_DC, 130, 630, 280, 740);

	hbr2 = (HBRUSH)SelectObject(_DC, hbrold2);
	::DeleteObject(hbr2);




	TCHAR szBuf[32] = L"";
	swprintf_s(szBuf, L"Tank cooking...");

	TextOut(_DC, 160, 610, szBuf, lstrlen(szBuf));


	MyObjMgr::Get_Instance()->Render(_DC);
}

void Stage1::Release(void)
{
	MyObjMgr::Get_Instance()->Delete_Obj(PLAYER);
	MyObjMgr::Get_Instance()->Delete_Obj(MONSTER);
	MyObjMgr::Get_Instance()->Delete_Obj(BULLET);
	//MyObjMgr::Get_Instance()->Delete_Obj(BOSS);
	//MyObjMgr::Get_Instance()->Release();
}
