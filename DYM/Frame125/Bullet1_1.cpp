#include "stdafx.h"
#include "Bullet1_1.h"


Bullet1_1::Bullet1_1()
{
}


Bullet1_1::~Bullet1_1()
{
}

void Bullet1_1::Initialize(void)
{
	m_fSpeed = 1.f;
}

int Bullet1_1::Update(void)
{
	if (20 >= m_tInfo.vPos.x || WINCX - 20 <= m_tInfo.vPos.x || 20 >= m_tInfo.vPos.y || WINCY - 20 <= m_tInfo.vPos.y)
		m_bDead = true;


	if (true == m_bDead)
		return OBJ_DEAD;


	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	return 0;
}

void Bullet1_1::Late_Update(void)
{
}

void Bullet1_1::Render(HDC hDC)
{
	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(255, 0, 0));//ÆÄ¶õ»ö
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, (int)m_tInfo.vPos.x - 10, (int)m_tInfo.vPos.y - 10, (int)m_tInfo.vPos.x + 10, (int)m_tInfo.vPos.y + 10);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void Bullet1_1::Release(void)
{
}
