#include "stdafx.h"
#include "BossBullet2.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

BossBullet2::BossBullet2()
{
	sizeInfo.fX = 10.f;
	sizeInfo.fY = 10.f;
}


BossBullet2::~BossBullet2()
{
	Release();
}

void BossBullet2::Initialize(void)
{
	m_fSpeed = 1.f;
}

int BossBullet2::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
		return OBJ_DEAD;
	//
	if (true == m_bDead)
		return OBJ_DEAD;

	m_tInfo.vDir = MyObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;
	Update_Size();

	return 0;
}

void BossBullet2::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;
}

void BossBullet2::Render(HDC hDC)
{
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
}

void BossBullet2::Release(void)
{
}
