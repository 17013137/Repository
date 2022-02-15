#include "stdafx.h"
#include "BulletX.h"


BulletX::BulletX()
{
}


BulletX::~BulletX()
{
	Release();
}

void BulletX::Initialize(void)
{
	m_fSpeed = 2.f;
}

int BulletX::Update(void)
{
	if (20 >= m_tInfo.vPos.x || WINCX - 20 <= m_tInfo.vPos.x || 20 >= m_tInfo.vPos.y || WINCY - 20 <= m_tInfo.vPos.y)
		m_bDead = true;


	if (true == m_bDead)
		return OBJ_DEAD;


	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	return 0;
}

void BulletX::Late_Update(void)
{
}

void BulletX::Render(HDC hDC)
{
	Ellipse(hDC, (int)m_tInfo.vPos.x - 10, (int)m_tInfo.vPos.y - 10, (int)m_tInfo.vPos.x + 10, (int)m_tInfo.vPos.y + 10);
}

void BulletX::Release(void)
{
}
