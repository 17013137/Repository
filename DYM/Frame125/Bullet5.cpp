#include "stdafx.h"
#include "Bullet5.h"


Bullet5::Bullet5()
{
}


Bullet5::~Bullet5()
{
	Release();
}

void Bullet5::Initialize(void)
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

int Bullet5::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX - 0 || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY - 0)
		return OBJ_DEAD;


	if (true == m_bDead)
		return OBJ_DEAD;




	return 0;
}

void Bullet5::Late_Update(void)
{
}

void Bullet5::Render(HDC hDC)
{

	
}

void Bullet5::Release(void)
{
}
