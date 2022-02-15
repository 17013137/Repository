#include "stdafx.h"
#include "S2Monster1.h"
#include"MyObjMgr.h"
#include"CollisionMgr.h"

S2Monster1::S2Monster1()
{
	sizeInfo.fX = 30.f;
	sizeInfo.fY = 30.f;
}


S2Monster1::~S2Monster1()
{
	Release();
}

void S2Monster1::Initialize(void)
{
	id = MONSTER2;
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f,0.f,0.f };

	Point[0] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[1] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[2] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y + 20.f,0.f };
	Point[3] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 20.f,0.f };

	for (int i = 0; i < 4; ++i) {
		originPoint[i] = Point[i];
	}


	m_fSpeed = 1.f;
}

int S2Monster1::Update(void)
{

	if (m_bDead)
		return OBJ_DEAD;


	DWORD dwTime = GetTickCount();
	m_tInfo.vPos.y += (cos(1.0f * 3.14 * dwTime)*3.33f);
	m_tInfo.vPos.x += (sin(1.0f * 3.14 * dwTime)*3.33f);


	if (m_tInfo.vPos.y > WINCY - 100) {
		m_tInfo.vPos.y = WINCY - 100;
		return OBJ_DEAD;
	}

	if (m_tInfo.vPos.y < 100) {
		m_tInfo.vPos.y = 100;

	}


	//m_tInfo.vDir = MyObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos - this->Get_Info().vPos;
	////m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
	//D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

	if (m_tInfo.vPos.x <= 100) {

		DWORD dwTime = GetTickCount();
		m_tInfo.vPos.y += (cos(1.0f * 3.14 * dwTime)*1.33f);
		m_tInfo.vPos.x += (sin(1.0f * 3.14 * dwTime)*1.33f);
		m_fSpeed *= -1;
	}
	else if (m_tInfo.vPos.x >= WINCX - 140)
	{

		isgo = true;
		m_fSpeed = 1.f;
		m_fSpeed *= -1;
	}

	Update_Size();

	return 0;
}

void S2Monster1::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;
}

void S2Monster1::Render(HDC hDC)
{

	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(34, 120, 10));
	//hpen = CreatePen(PS_SOLID, 3, RGB(95, 106, 175));보라색
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(73, 152, 44));
	//hbr = CreateSolidBrush(RGB(113, 139, 255));보라색
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	if (isCollide)
	{

		HPEN hpen2;

		HPEN hpenOld2;


		HBRUSH hbr2 = nullptr;

		HBRUSH hbrold2;
		hbrold2 = (HBRUSH)::SelectObject(hDC, hbr2);
		hpen2 = CreatePen(PS_SOLID, 3, RGB(173, 59, 70));
		hpenOld2 = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen2);
		hbr2 = CreateSolidBrush(RGB(250, 74, 80));
		hbrold2 = (HBRUSH)::SelectObject(hDC, hbr2);

		MoveToEx(hDC, (int)m_tInfo.vPos.x, m_tInfo.vPos.y, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x - 10, m_tInfo.vPos.y - 20);


		MoveToEx(hDC, (int)m_tInfo.vPos.x - 10, m_tInfo.vPos.y - 20, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x + 10, m_tInfo.vPos.y - 20);

		MoveToEx(hDC, (int)m_tInfo.vPos.x + 10, m_tInfo.vPos.y - 20, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x, m_tInfo.vPos.y);

		hpen2 = (HPEN)::SelectObject(hDC, hpenOld2);
		::DeleteObject(hpen2);
		hbr2 = (HBRUSH)SelectObject(hDC, hbrold2);
		::DeleteObject(hbr2);

		if (delay > 10) {
			Set_Dead();
			delay = 0;

		}
		if (dwTime + 100 < GetTickCount()) {
			delay++;
		}


	}
	else {


		MoveToEx(hDC, (int)m_tInfo.vPos.x, m_tInfo.vPos.y, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x - 10, m_tInfo.vPos.y - 20);


		MoveToEx(hDC, (int)m_tInfo.vPos.x - 10, m_tInfo.vPos.y - 20, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x + 10, m_tInfo.vPos.y - 20);

		MoveToEx(hDC, (int)m_tInfo.vPos.x + 10, m_tInfo.vPos.y - 20, NULL);
		LineTo(hDC, (int)m_tInfo.vPos.x, m_tInfo.vPos.y);
	}


	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);

	


}

void S2Monster1::Release(void)
{
}

void S2Monster1::Collide(OBJID objid)
{
	if (objid == BULLET)
	{
		isCollide = true;
	}
}
