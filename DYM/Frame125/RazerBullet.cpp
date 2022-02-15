#include "stdafx.h"
#include "RazerBullet.h"
#include "SceneMgr.h"

RazerBullet::RazerBullet()
{
	sizeInfo.fX = 60.f;
	sizeInfo.fY = 60.f;
}


RazerBullet::~RazerBullet()
{
	Release();
}

void RazerBullet::Initialize(void)
{
	m_fSpeed = 20.f;
}

int RazerBullet::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX - 0 || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY - 0)
		return OBJ_DEAD;
	if (true == m_bDead)
		return OBJ_DEAD;

	// FSM(유한 상태 기계)
	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;
	Update_Size();

	return OBJ_NOEVENT;
}

void RazerBullet::Late_Update(void)
{
}

void RazerBullet::Render(HDC hDC)
{
	//Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
}

void RazerBullet::Release(void)
{
}
