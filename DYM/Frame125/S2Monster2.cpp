#include "stdafx.h"
#include "S2Monster2.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

S2Monster2::S2Monster2()
{
	sizeInfo.fX = 50.f;
	sizeInfo.fY = 50.f;

	//D3DXMatrixIdentity(&m_tInfo.matWorld);
}


S2Monster2::~S2Monster2()
{
	Release();
}

void S2Monster2::Initialize(void)
{
	id = MONSTER3;
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };

	Point[0] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[1] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[2] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y + 20.f,0.f };
	Point[3] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 20.f,0.f };

	for (int i = 0; i < 4; ++i) {
		originPoint[i] = Point[i];
	}


	m_fSpeed = 2.f;
	
}

int S2Monster2::Update(void)
{


	if (m_bDead)
		return OBJ_DEAD;

	if (m_tInfo.vPos.x < 50 || m_tInfo.vPos.x > WINCX- 50) {
		m_tInfo.vDir.x *= -1;
	}

	if (m_tInfo.vPos.y < 50 || m_tInfo.vPos.y > WINCY - 50) {
		m_tInfo.vDir.y *= -1;
	}

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	/*if (delay > 10) {
		
		delay = 0;
	}

	if (dwTime + 100 < GetTickCount()) {
		delay++;
	}
	*/


	//if (isgo) {
	//
	//
	//	m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
	//
	//
	//	// 단위벡터로 만들어주는 함수
	//	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	//	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);
	//
	//	// 두 방향벡터를 내적하여 스칼라 값을 구해주는 함수
	//	float		fDot = D3DXVec3Dot(&m_tInfo.vLook, &m_tInfo.vDir);
	//	// 내적의 결과 자체가 각도가 절대 아님.
	//
	//	float		fAngle = acosf(fDot);
	//
	//	if (m_tInfo.vPos.y < m_pTarget->Get_Info().vPos.y)
	//		fAngle = 2.f * D3DX_PI - fAngle;
	//
	//	m_tInfo.vPos.x += m_fSpeed * cosf(fAngle);
	//	m_tInfo.vPos.y -= m_fSpeed * sinf(fAngle);
	//
	//}


	//if (dwTime + 50 < GetTickCount())
	//	isgo = true;




	////if (m_tInfo.vPos.x <= 100)
	////	return 1;
	Update_Size();
	return 0;
}

void S2Monster2::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;
}

void S2Monster2::Render(HDC hDC)
{
	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(230, 96, 38));
	//hpen = CreatePen(PS_SOLID, 3, RGB(44, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(248, 146, 63));
	//hbr = CreateSolidBrush(RGB(0, 180, 233));//파란색
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	if (isCollide)
	{

		HPEN hpen2;
	
		HPEN hpenOld2;
	

		HBRUSH hbr2= nullptr;
	
		HBRUSH hbrold2;
		hbrold2 = (HBRUSH)::SelectObject(hDC, hbr2);
		hpen2 = CreatePen(PS_SOLID, 3, RGB(173, 59, 70));
		hpenOld2 = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen2);
		hbr2 = CreateSolidBrush(RGB(250, 74, 80));
		hbrold2 = (HBRUSH)::SelectObject(hDC, hbr2);

		Ellipse(hDC, m_tInfo.vPos.x + 25.f,
			m_tInfo.vPos.y + 25.f,
			m_tInfo.vPos.x - 25.f,
			m_tInfo.vPos.y - 25.f);

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


		Ellipse(hDC, m_tInfo.vPos.x + 25.f,
			m_tInfo.vPos.y + 25.f,
			m_tInfo.vPos.x - 25.f,
			m_tInfo.vPos.y - 25.f);
	}


	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);
}

void S2Monster2::Release(void)
{
}

void S2Monster2::Collide(OBJID objid)
{
	if (objid == BULLET)
	{
		isCollide = true;
	}
}
