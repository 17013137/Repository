#include "stdafx.h"
#include "BossBullet.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

BossBullet::BossBullet()
{
	sizeInfo.fX = 20.f;
	sizeInfo.fY = 20.f;
}


BossBullet::~BossBullet()
{
	Release();

}

void BossBullet::Initialize(void)
{
	m_fSpeed = 3.f;
	m_tInfo.vDir = { 0.f, 1.f, 0.f };
}

int BossBullet::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
		return OBJ_DEAD;
	//
	if (true == m_bDead)
		return OBJ_DEAD;

	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;
	Update_Size();

	return 0;
}

void BossBullet::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;

}

void BossBullet::Render(HDC hDC)
{
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
}

void BossBullet::Release(void)
{
}
