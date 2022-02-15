#include "stdafx.h"
#include "Stage3.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "MyObjMgr.h"
#include "LU.h"
#include "LD.h"
#include "RU.h"
#include "RD.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"

Stage3::Stage3()
{
	cnt = 0;
	isgo = false;
	delay = 0;
}


Stage3::~Stage3()
{
	Release();
}

void Stage3::Initialize(void)
{
	RECT tmp;

	CObj* object = CAbstractFactory<CPlayer>::Create(400, 400);
	MyObjMgr::Get_Instance()->Add_Object(PLAYER, object);

	CObj * MonLD = CAbstractFactory<LD>::Create(30, 30);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonLD);

	MonLD = CAbstractFactory<LD>::Create(30, 400);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonLD);

	CObj* MonRD = CAbstractFactory<RD>::Create(770, 30);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonRD);

	MonRD = CAbstractFactory<RD>::Create(770, 400);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonRD);

	CObj* MonLU = CAbstractFactory<LU>::Create(30, 400);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonLU);

	MonLU = CAbstractFactory<LU>::Create(30, 770);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonLU);

	CObj* MonRU = CAbstractFactory<RU>::Create(770, 400);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonRU);

	MonRU = CAbstractFactory<RU>::Create(770, 770);
	MyObjMgr::Get_Instance()->Add_Object(YHMONSTER, MonRU);

}

void Stage3::Update(void)
{
	if (GetAsyncKeyState(VK_F1) || MyObjMgr::Get_Instance()->Get_ObjList(YHMONSTER)->size() == 0) {

		if(delay == 0){
			delay = GetTickCount();
		}
		if (delay + 1000 < GetTickCount()) {
			SceneMgr::Get_Instance()->Scene_Change(STAGE2);
		}
	}

	MyObjMgr::Get_Instance()->Update();
}

void Stage3::Late_Update(void)
{
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(YHMONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(RAZER), MyObjMgr::Get_Instance()->Get_ObjList(YHMONSTER));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(HACK), MyObjMgr::Get_Instance()->Get_ObjList(YHMONSTER));

	
	MyObjMgr::Get_Instance()->Late_Update();
}

void Stage3::Render(HDC _DC)
{

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(204, 204, 204));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(_DC, hbr);

	Rectangle(_DC, 60, 60, WINCX - 60, WINCY - 60);

	hbr = (HBRUSH)SelectObject(_DC, hbrold);
	::DeleteObject(hbr);


	if (isgo) {


		TCHAR szBuf[32] = L"";
		swprintf_s(szBuf, L"Tank cooking...");

		TextOut(_DC, 130, 580, szBuf, lstrlen(szBuf));



		//±è
		HBRUSH hbr2;
		hbr2 = CreateSolidBrush(RGB(82, 73, 74));
		HBRUSH hbrold2;
		hbrold2 = (HBRUSH)::SelectObject(_DC, hbr2);

		Rectangle(_DC, 100, 600, 250, 710);

		hbr2 = (HBRUSH)SelectObject(_DC, hbrold2);
		::DeleteObject(hbr2);
	}


	MyObjMgr::Get_Instance()->Render(_DC);
}

void Stage3::Release(void)
{
	MyObjMgr::Get_Instance()->Delete_Obj(PLAYER);
	MyObjMgr::Get_Instance()->Delete_Obj(YHMONSTER);
	MyObjMgr::Get_Instance()->Delete_Obj(MONSTERBULLET);
}
