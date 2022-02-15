#include "stdafx.h"
#include "Monster.h"
#include "AbstractFactory.h"
#include "MonsterBullet2.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

CMonster::CMonster()
{
	sizeInfo.fX = 40.f;
	sizeInfo.fY = 40.f;

}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize(void)
{
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_fSpeed = 1.f;
	Update_Size();
}

int CMonster::Update(void)
{
	m_tInfo.vDir = MyObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos - this->Get_Info().vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	Update_Size();
	return OBJ_NOEVENT;
}

void CMonster::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;


}

void CMonster::Render(HDC hDC)
{
	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
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

void CMonster::Release(void)
{
}

//void CMonster::Set_Bullet(list<CObj*>* _pBullet)
//{
//	m_pBullet = _pBullet;
//}
//
//CObj * CMonster::Create_Bullet(void)
//{
//	CObj* pBullet = CAbstractFactory<CMonsterBullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
//
//	return pBullet;
//}
