#include "stdafx.h"
#include "MonsterBullet2.h"


MonsterBullet2::MonsterBullet2()
{
	sizeInfo.fX = 10.f;
	sizeInfo.fY = 10.f;
}


MonsterBullet2::~MonsterBullet2()
{
	Release();
}

void MonsterBullet2::Initialize(void)
{
	m_fSpeed = 2.f;
}

int MonsterBullet2::Update(void)
{
	if (5 >= m_tInfo.vPos.x || WINCX - 5 <= m_tInfo.vPos.x || 5 >= m_tInfo.vPos.y || WINCY - 5 <= m_tInfo.vPos.y)
		m_bDead = true;

	if (true == m_bDead)
		return OBJ_DEAD;

	float fDiagonal = sqrtf(m_tInfo.vDir.x*m_tInfo.vDir.x + m_tInfo.vDir.y*m_tInfo.vDir.y);

	m_tInfo.vDir.x /= fDiagonal;
	m_tInfo.vDir.y /= fDiagonal;
	m_tInfo.vDir.z = 0.f;

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	Update_Size();

	return OBJ_NOEVENT;
}

void MonsterBullet2::Late_Update(void)
{
}

void MonsterBullet2::Render(HDC hDC)
{
	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, (int)m_tInfo.vPos.x - 5, (int)m_tInfo.vPos.y - 5, (int)m_tInfo.vPos.x + 5, (int)m_tInfo.vPos.y + 5);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void MonsterBullet2::Release(void)
{
}
