#include "stdafx.h"
#include "Bullet4.h"


Bullet4::Bullet4()
{
	sizeInfo.fX = 20.f;
	sizeInfo.fY = 20.f;
}


Bullet4::~Bullet4()
{
	Release();
}

void Bullet4::Initialize(void)
{
	id = BULLET;

	m_vBarrelPoint[0] = { 100.f,300.f, 0.f };
	//m_vBarrelPoint[1] = { 115.f,115.f, 0.f };
	//m_vBarrelPoint[2] = { +5.f,-50.f, 0.f };
	//m_vBarrelPoint[3] = { 0.f, -55.f, 0.f };
	//m_vBarrelPoint[4] = { -5.f, -50.f, 0.f };
	//m_vBarrelPoint[5] = { -5.f, -10.f, 0.f };

	for (int i = 0; i < 6; ++i)

	m_vOriginBarrelPoint[i] = m_vBarrelPoint[i];
	m_fSpeed = 5.f;
}

int Bullet4::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX - 0 || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY - 0)
		return OBJ_DEAD;


	if (true == m_bDead)
		return OBJ_DEAD;


	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;

	if (delay > 10)
	{
		isgo = true;
		m_fSpeed = 7;
	}

	if (dwTime + 20 < GetTickCount()) {
		delay++;
	}


	Update_Size();
	return OBJ_NOEVENT;
}

void Bullet4::Late_Update(void)
{
}

void Bullet4::Render(HDC hDC)
{

	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(44, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(0, 180, 233));//ÆÄ¶õ»ö
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);



	if (isgo) {


		DWORD dwTime = GetTickCount();
		//	m_tInfo.vPos.y += (cos(dwTime)*10);
		m_tInfo.vPos.x += (sin(dwTime) * 10);


		Ellipse(hDC, m_tInfo.vPos.x - 30, m_tInfo.vPos.y - 30, m_tInfo.vPos.x + 30, m_tInfo.vPos.y + 30);
		Ellipse(hDC, m_tInfo.vPos.x - 10, m_tInfo.vPos.y - 10, m_tInfo.vPos.x + 10, m_tInfo.vPos.y + 10);
		Ellipse(hDC, m_tInfo.vPos.x - 30, m_tInfo.vPos.y - 20, m_tInfo.vPos.x + -20, m_tInfo.vPos.y - 10);
		Ellipse(hDC, m_tInfo.vPos.x + 20, m_tInfo.vPos.y - 20, m_tInfo.vPos.x + 30, m_tInfo.vPos.y - 10);

	}
	else
		Ellipse(hDC, m_tInfo.vPos.x - 20, m_tInfo.vPos.y - 20, m_tInfo.vPos.x + 20, m_tInfo.vPos.y + 20);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void Bullet4::Release(void)
{
}
