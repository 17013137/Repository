#include "stdafx.h"
#include "stage2Monster3.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

stage2Monster3::stage2Monster3()
{
	sizeInfo.fX = 60.f;
	sizeInfo.fY = 60.f;
	D3DXMatrixIdentity(&m_tInfo.matWorld);	// 항등 행렬을 만들어주는 함수
}


stage2Monster3::~stage2Monster3()
{
	Release();
}

void stage2Monster3::Initialize(void)
{
	id = MONSTER2;

	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f,0.f,0.f };

	Point[0] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[1] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[2] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y + 20.f,0.f };
	Point[3] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 20.f,0.f };
	//Point[3] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 20.f,0.f };
	//Point[3] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 20.f,0.f };
	for (int i = 0; i < 4; ++i) {
		originPoint[i] = Point[i];
	}


	m_fSpeed = 5.f;
}

int stage2Monster3::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;


	if (m_tInfo.vPos.y > WINCY - 100)
		m_tInfo.vPos.y = WINCY - 100;

	if (m_tInfo.vPos.y < 100)
		m_tInfo.vPos.y = 100;


	if (m_tInfo.vPos.x > WINCX - 130)
		m_tInfo.vPos.x = WINCX - 130;

	if (m_tInfo.vPos.x < 130)
		m_tInfo.vPos.x = 130;




	if (delay > 100) {
		//m_tInfo.vPos.x = m_tInfo.vPos.x * cos(30) - m_tInfo.vPos.y * sin(30);
		//m_tInfo.vPos.y = m_tInfo.vPos.x * sin(30) + m_tInfo.vPos.y * cos(30);

		m_fAngle -= D3DXToRadian(2);
		delay = 0;
	}

	if (dwTime + 100 < GetTickCount()) {
		delay++;
	}


	m_fAngle += D3DXToRadian(2);

	D3DXMATRIX	matScale, matRotZ, matTrans;
	// 크기 변환 행렬을 만들어주는 함수
	D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
	// 회전 행렬을 만들어주는 함수

	//D3DXMatrixRotationX()
	//D3DXMatrixRotationY()
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	// 이동 행렬을 만들어주는 함수
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	// 벡터와 행렬의 곱셈을 수행해주는 함수
	// 1. 위치벡터 : D3DXVec3TransformCoord(vec3* pOut, 위치벡터, 행렬)
	// 2. 방향벡터 : D3DXVec3TransformNormal(vec3* pOut, 방향벡터, 행렬);

	for (int i = 0; i < 4; ++i)
	{
		Point[i] = originPoint[i];

		Point[i] -= D3DXVECTOR3(100.f, 100.f, 0.f);	// 원점 기준으로의 좌표이동
		D3DXVec3TransformCoord(&Point[i], &Point[i], &m_tInfo.matWorld);
	}



	
	Update_Size();
}

void stage2Monster3::Late_Update(void)
{
	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(HACK)) || CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(RAZER)))
		m_bDead = true;
}

void stage2Monster3::Render(HDC hDC)
{
	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(212, 201, 176));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(225,232 , 97));
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


		for (int i = 0; i < 4; ++i) {
			Rectangle(hDC, Point[i].x + 15.f,
				Point[i].y + 15.f,
				Point[i].x - 15.f,
				Point[i].y - 15.f);
		}
		hpen2 = (HPEN)::SelectObject(hDC, hpenOld2);
		::DeleteObject(hpen2);
		hbr2 = (HBRUSH)SelectObject(hDC, hbrold2);
		::DeleteObject(hbr2);


		if (delay > 50) {
			m_fAngle -= D3DXToRadian(20);
			//m_tInfo.vPos.y -= 30;
		}
		if (delay > 100) {
			Set_Dead();
			delay = 0;

		}
		if (dwTime + 100 < GetTickCount()) {
			delay++;
		}


	}
	else {

		Rectangle(hDC, Point[0].x + 15.f,
			Point[0].y + 15.f,
			Point[0].x - 15.f,
			Point[0].y - 15.f);


	/*	for (int i = 0; i < 4; ++i) {
			Rectangle(hDC, Point[i].x + 15.f,
				Point[i].y + 15.f,
				Point[i].x - 15.f,
				Point[i].y - 15.f);
		}*/


	
	}


	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);
}

void stage2Monster3::Release(void)
{
}

void stage2Monster3::Collide(OBJID objid)
{
	if (objid == BULLET)
	{
		isCollide = true;
	}
}
