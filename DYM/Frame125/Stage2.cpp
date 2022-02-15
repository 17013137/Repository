#include "stdafx.h"
#include "Stage2.h"
#include "AbstractFactory.h"
#include"Player.h"
#include "MyObjMgr.h"
#include "S2Monster1.h"
#include "CollisionMgr.h"
#include "S2Monster2.h"
#include "stage2Monster3.h"
#include "SceneMgr.h"
Stage2::Stage2()
{
}


Stage2::~Stage2()
{
	Release();
}

void Stage2::Initialize(void)
{

	RECT tmp;

	

	CObj* object= CAbstractFactory<CPlayer>::Create();
	MyObjMgr::Get_Instance()->Add_Object(PLAYER, object);

	

	//파란몬스터, 당근
	object = CAbstractFactory<S2Monster2>::Create(D3DXVECTOR3(200, 300, 0), D3DXVECTOR3(1, 0, 0));
	MyObjMgr::Get_Instance()->Add_Object(MONSTER3, object);
	object = CAbstractFactory<S2Monster2>::Create(D3DXVECTOR3(500, 200, 0), D3DXVECTOR3(1, 1, 0));
	MyObjMgr::Get_Instance()->Add_Object(MONSTER3, object);

	object = CAbstractFactory<S2Monster2>::Create(D3DXVECTOR3(400, 500, 0), D3DXVECTOR3(-1, 0.3, 0));
	MyObjMgr::Get_Instance()->Add_Object(MONSTER3, object);

	object = CAbstractFactory<S2Monster2>::Create(D3DXVECTOR3(100, 600, 0), D3DXVECTOR3(0.5, -0.7, 0));
	MyObjMgr::Get_Instance()->Add_Object(MONSTER3, object);
	object = CAbstractFactory<S2Monster2>::Create(D3DXVECTOR3(600, 500, 0), D3DXVECTOR3(-0.5, -0.5, 0));
	MyObjMgr::Get_Instance()->Add_Object(MONSTER3, object);

	
	//노란 몬스터, 단무지

	for (int i = 0; i < 14; ++i) {
		object = CAbstractFactory<stage2Monster3>::Create(uid(dre), uid(dre), uidangle(dre));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER2, object);

	}

	//보라몬스터, 시금치
	for (int i = 0; i < 8; ++i) {
		object = CAbstractFactory<S2Monster1>::Create(uid(dre), uid(dre));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER4, object);
		//dynamic_cast<S2Monster1*>(MyObjMgr::Get_Instance()->Get_Monster4())->Set_Target(MyObjMgr::Get_Instance()->Get_Player());
	}




}

void Stage2::Update(void)
{

	//단무지 수세기
	if (MyObjMgr::Get_Instance()->Get_Monster2num()<=0) {
		goyellow = true;
	}
	//당근 수세기
	if (MyObjMgr::Get_Instance()->Get_Monster3num() <= 0) {
		gocarrot = true;
	}
	//시금치 수세기
	if (MyObjMgr::Get_Instance()->Get_Monster4num() <= 0) {
		gogreen = true;
	}


	if (GetAsyncKeyState(VK_F3)) {
		SceneMgr::Get_Instance()->Scene_Change(ENDING);
	}
	MyObjMgr::Get_Instance()->Update();
}

void Stage2::Late_Update(void)
{
	CollisionMgr::Collision_Sphere2(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER2));
	CollisionMgr::Collision_Sphere(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	CollisionMgr::Collision_Sphere2(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER2));
	CollisionMgr::Collision_Sphere2(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER3));
	CollisionMgr::Collision_Sphere2(MyObjMgr::Get_Instance()->Get_ObjList(BULLET), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER4));
	//CollisionMgr::Collision_Sphere3(MyObjMgr::Get_Instance()->Get_ObjList(MONSTER2), MyObjMgr::Get_Instance()->Get_ObjList(MONSTER2));




	MyObjMgr::Get_Instance()->Late_Update();
}

void Stage2::Render(HDC _DC)
{



	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(204, 204, 204));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(_DC, hbr);


	Rectangle(_DC, 30, 30, WINCX - 30, WINCY - 30);

	hbr = (HBRUSH)SelectObject(_DC, hbrold);
	::DeleteObject(hbr);


	//김
	HBRUSH hbr2;
	hbr2 = CreateSolidBrush(RGB(82, 73, 74));
	HBRUSH hbrold2;
	hbrold2 = (HBRUSH)::SelectObject(_DC, hbr2);

	Rectangle(_DC, 130, 630, 280, 740);

	hbr2 = (HBRUSH)SelectObject(_DC, hbrold2);
	::DeleteObject(hbr2);


	//밥
	HBRUSH hbr3;
	hbr3 = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hbrold3;
	hbrold3 = (HBRUSH)::SelectObject(_DC, hbr3);

	Rectangle(_DC, 140, 640, 270, 730);

	hbr3 = (HBRUSH)SelectObject(_DC, hbrold3);
	::DeleteObject(hbr3);

	//단무지
	if (goyellow) {
		
		HBRUSH hbr4;
		hbr4 = CreateSolidBrush(RGB(225, 232, 97));
		HBRUSH hbrold4;
		hbrold4 = (HBRUSH)::SelectObject(_DC, hbr4);

		Rectangle(_DC, 150, 650, 260, 670);

		hbr4 = (HBRUSH)SelectObject(_DC, hbrold4);
		::DeleteObject(hbr4);

	}
	//당근
	if (gocarrot) {

		HBRUSH hbr5;
		hbr5 = CreateSolidBrush(RGB(248, 146, 63));
		HBRUSH hbrold5;
		hbrold5 = (HBRUSH)::SelectObject(_DC, hbr5);

		Rectangle(_DC, 150, 680, 260, 690);

		hbr5 = (HBRUSH)SelectObject(_DC, hbrold5);
		::DeleteObject(hbr5);
	}
	//시금치

	if (gogreen) {

		HBRUSH hbr6;
		hbr6 = CreateSolidBrush(RGB(73, 152, 44));
		HBRUSH hbrold6;
		hbrold6 = (HBRUSH)::SelectObject(_DC, hbr6);

		Rectangle(_DC, 150, 700, 260, 710);

		hbr6 = (HBRUSH)SelectObject(_DC, hbrold6);
		::DeleteObject(hbr6);

		if (delay > 200) {

			//씬 전환
			SceneMgr::Get_Instance()->Scene_Change(ENDING);


		}

		if (dwTime + 100 < GetTickCount()) {
			delay++;
		
		}

	}

	

	TCHAR szBuf[32] = L"";
	swprintf_s(szBuf, L"Tank cooking...");

	TextOut(_DC, 160, 610, szBuf, lstrlen(szBuf));


	MyObjMgr::Get_Instance()->Render(_DC);
}

void Stage2::Release(void)
{

//	MyObjMgr::Get_Instance()->Delete_Obj(PLAYER);
	MyObjMgr::Get_Instance()->Delete_Obj(MONSTER2);
	MyObjMgr::Get_Instance()->Delete_Obj(MONSTER3);
	MyObjMgr::Get_Instance()->Delete_Obj(MONSTER4);
}
