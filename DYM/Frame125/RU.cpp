#include "stdafx.h"
#include "RU.h"
#include "MyObjMgr.h"
#include "AbstractFactory.h"
#include "MonsterBullet2.h"
#include "CollisionMgr.h"

RU::RU()
{
	sizeInfo.fX = 60;
	sizeInfo.fY = 60;
}


RU::~RU()
{
}

void RU::Initialize(void)
{
}

int RU::Update(void)
{
	m_tInfo.vDir = MyObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;

	if (m_bDead)
		return OBJ_DEAD;

	switch (iDir)
	{
	case 1:
		m_tInfo.vPos.y -= 2.5f;
		if (m_tInfo.vPos.x == 770.f&&m_tInfo.vPos.y == 30.f)
		{
			Create_MonsterBullet(m_tInfo.vDir);
			iDir = 2;
		}
		break;
	case 2:
		m_tInfo.vPos.x -= 2.5f;
		if (m_tInfo.vPos.x == 30.f&&m_tInfo.vPos.y == 30.f)
		{
			Create_MonsterBullet(m_tInfo.vDir);
			iDir = 3;
		}
		break;
	case 3:
		m_tInfo.vPos.y += 2.5f;
		if (m_tInfo.vPos.x == 30.f&&m_tInfo.vPos.y == 770.f)
		{
			Create_MonsterBullet(m_tInfo.vDir);
			iDir = 4;
		}

		break;
	case 4:
		m_tInfo.vPos.x += 2.5f;
		if (m_tInfo.vPos.x == 770.f&&m_tInfo.vPos.y == 770.f)
		{
			Create_MonsterBullet(m_tInfo.vDir);
			iDir = 1;
		}

		break;
	default:
		break;
	}

	return 0;
}

void RU::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;

}

void RU::Render(HDC hDC)
{
	Ellipse(hDC, (int)m_tInfo.vPos.x - 30, (int)m_tInfo.vPos.y - 30, (int)m_tInfo.vPos.x + 30, (int)m_tInfo.vPos.y + 30);
	MoveToEx(hDC, (int)m_tInfo.vPos.x - 5, (int)m_tInfo.vPos.y - 5, nullptr);
	LineTo(hDC, (int)m_tInfo.vPos.x - 20, (int)m_tInfo.vPos.y - 15);
	MoveToEx(hDC, (int)m_tInfo.vPos.x + 5, (int)m_tInfo.vPos.y - 5, nullptr);
	LineTo(hDC, (int)m_tInfo.vPos.x + 20, (int)m_tInfo.vPos.y - 15);
	MoveToEx(hDC, (int)m_tInfo.vPos.x - 10, (int)m_tInfo.vPos.y + 15, nullptr);
	LineTo(hDC, (int)m_tInfo.vPos.x + 10, (int)m_tInfo.vPos.y + 15);


	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);



	Ellipse(hDC, (int)m_tInfo.vPos.x - 18, (int)m_tInfo.vPos.y - 5, (int)m_tInfo.vPos.x - 10, (int)m_tInfo.vPos.y + 5);
	Ellipse(hDC, (int)m_tInfo.vPos.x + 10, (int)m_tInfo.vPos.y - 5, (int)m_tInfo.vPos.x + 18, (int)m_tInfo.vPos.y + 5);

	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void RU::Release(void)
{
}

CObj * RU::Create_MonsterBullet(D3DXVECTOR3 _Dir)
{
	CObj* pMonsterBullet = nullptr;

	pMonsterBullet = CAbstractFactory<MonsterBullet2>::Create(m_tInfo.vPos, _Dir);
	MyObjMgr::Get_Instance()->Add_Object(MONSTERBULLET, pMonsterBullet);
	return pMonsterBullet;
}
