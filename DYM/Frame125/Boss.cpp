#include "stdafx.h"
#include "Boss.h"
#include "MyObjMgr.h"
#include "AbstractFactory.h"
#include "Include.h"
#include "BossBullet.h"
#include "BossBullet2.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"


Boss::Boss()
{
	sizeInfo.fX = 400.f;
	sizeInfo.fX = 200.f;
	timer = 0;
	flag = false;
	stat.Max_Hp = 100;
	stat.Hp = 100;
}


Boss::~Boss()
{
	Release();
}

void Boss::Initialize(void)
{
	m_tInfo.vPos = { 400.f, 100.f, 0.f };
	m_tInfo.vDir = { -1.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 1.f, 0.f };

	m_fSpeed = 1.f;
	Update_Size();
	timer = GetTickCount();
}

int Boss::Update(void)
{
	if (true == m_bDead) {
		stat.Hp -= 10;
		m_bDead = false;
	}

	if (stat.Hp <= 0) {
		SceneMgr::Get_Instance()->Scene_Change(STAGE3);

		return OBJ_DEAD;
	}
	if (rectinfo.left <= 30 || rectinfo.right >= WINCX - 30)
		m_tInfo.vDir *= -1;
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

	if (timer + 3000 < GetTickCount()) {
		flag = true;
		timer = 0;
	}

	if (flag) {
		Pattern1();
		Pattern2();
	}

	Update_Size();
	return 0;
}

void Boss::Late_Update(void)
{

}

void Boss::Pattern1()
{

	if (sk1.cooltime + 100 < GetTickCount()) { //°ø°Ý ÄðÅ½
		sk1.attacking = false;
		sk1.cooltime = 0;
	}

	if (!sk1.attacking) {
		sk1.attacking = true;
		sk1.cooltime = GetTickCount();
		D3DXVECTOR3 temp = m_tInfo.vPos;
		temp.y += 50.f;
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet>::Create(temp, D3DXVECTOR3{ 1.f, 1.f, 0.f }));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet>::Create(temp, D3DXVECTOR3{ 0.5f, 1.f, 0.f }));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet>::Create(temp, D3DXVECTOR3{ -0.5f, 1.f, 0.f }));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet>::Create(temp, D3DXVECTOR3{ 0.f, 1.f, 0.f }));
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet>::Create(temp, D3DXVECTOR3{ -1.f, 1.f, 0.f }));
	}

}

void Boss::Pattern2()
{
	if (sk2.cooltime + 2000 < GetTickCount()) { //°ø°Ý ÄðÅ½
		sk2.attacking = false;
		sk2.cooltime = 0;
	}

	if (!sk2.attacking) {
		sk2.attacking = true;
		sk2.cooltime = GetTickCount();
		D3DXVECTOR3 temp = m_tInfo.vPos;
		temp += {100.f, 50.f, 0.f};
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet2>::Create(temp, D3DXVECTOR3{ 1.f, 1.f, 0.f }));
		temp += {-200.f, 0.f, 0.f};
		MyObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<BossBullet2>::Create(temp, D3DXVECTOR3{ 0.5f, 1.f, 0.f }));

	}

}


void Boss::Render(HDC hDC)
{
	Ellipse(hDC, (int)m_tInfo.vPos.x - 200, (int)m_tInfo.vPos.y - 100, (int)m_tInfo.vPos.x + 200, (int)m_tInfo.vPos.y + 100);

	MoveToEx(hDC, (int)m_tInfo.vPos.x - 100, m_tInfo.vPos.y - 50, NULL);
	LineTo(hDC, (int)m_tInfo.vPos.x - 30, m_tInfo.vPos.y - 30);
	

	MoveToEx(hDC, (int)m_tInfo.vPos.x + 100, m_tInfo.vPos.y - 50, NULL);
	LineTo(hDC, (int)m_tInfo.vPos.x + 30, m_tInfo.vPos.y - 30);


	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(200, 92, 112));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);


	Ellipse(hDC, (int)m_tInfo.vPos.x - 70, (int)m_tInfo.vPos.y, (int)m_tInfo.vPos.x - 30, (int)m_tInfo.vPos.y + 20);
	Ellipse(hDC, (int)m_tInfo.vPos.x + 70, (int)m_tInfo.vPos.y, (int)m_tInfo.vPos.x + 30, (int)m_tInfo.vPos.y + 20);



	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);



	MoveToEx(hDC, (int)m_tInfo.vPos.x - 50, m_tInfo.vPos.y + 50, NULL);
	LineTo(hDC, (int)m_tInfo.vPos.x + 50, m_tInfo.vPos.y + 50);


}

void Boss::Release(void)
{
}

void Boss::Update_Size()
{
	rectinfo.left = m_tInfo.vPos.x - 200;
	rectinfo.top = m_tInfo.vPos.y - 100;
	rectinfo.right = m_tInfo.vPos.x + 200;
	rectinfo.bottom = m_tInfo.vPos.y + 100;

}


