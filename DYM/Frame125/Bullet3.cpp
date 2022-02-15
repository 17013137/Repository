#include "stdafx.h"
#include "Bullet3.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

Bullet3::Bullet3()
{
	sizeInfo.fX = 20.f;
	sizeInfo.fY = 20.f;

	D3DXMatrixIdentity(&m_tInfo.matWorld);	// �׵� ����� ������ִ� �Լ�

}


Bullet3::~Bullet3()
{
	Release();
}

void Bullet3::Initialize(void)
{
	id = BULLET;
	m_fSpeed = 3.f;


	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f,0.f,0.f };

	Point[0] = { m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y - 20.f,0.f };
	Point[1] = { m_tInfo.vPos.x + 20.f, m_tInfo.vPos.y - 20.f,0.f };

	for (int i = 0; i < 2; ++i) {
		originPoint[i] = Point[i];
	}







}

int Bullet3::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX - 0 || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY - 0)
		return OBJ_DEAD;

	if (true == m_bDead)
		return OBJ_DEAD;

	if (MyObjMgr::Get_Instance()->Get_Monster2num() > 0) {
		m_tInfo.vDir = MyObjMgr::Get_Instance()->Get_Monster2()->Get_Info().vPos - this->Get_Info().vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}


	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;

	if (delay > 20)
	{
		isgo = true;

		m_fSpeed = 8;
	}

	if (dwTime + 300 < GetTickCount()) {
		delay++;
	}




	//m_fAngle += D3DXToRadian(2);

	D3DXMATRIX	matScale, matRotZ, matTrans;
	// ũ�� ��ȯ ����� ������ִ� �Լ�
	D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
	// ȸ�� ����� ������ִ� �Լ�

	//D3DXMatrixRotationX()
	//D3DXMatrixRotationY()
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	// �̵� ����� ������ִ� �Լ�
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	// ���Ϳ� ����� ������ �������ִ� �Լ�
	// 1. ��ġ���� : D3DXVec3TransformCoord(vec3* pOut, ��ġ����, ���)
	// 2. ���⺤�� : D3DXVec3TransformNormal(vec3* pOut, ���⺤��, ���);

	for (int i = 0; i < 2; ++i)
	{
		Point[i] = originPoint[i];

		Point[i] -= D3DXVECTOR3(100.f, 100.f, 0.f);	// ���� ���������� ��ǥ�̵�
		D3DXVec3TransformCoord(&Point[i], &Point[i], &m_tInfo.matWorld);
	}



	Update_Size();

	return OBJ_NOEVENT;
}

void Bullet3::Late_Update(void)
{
}

void Bullet3::Render(HDC hDC)
{ 

	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(44, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(0, 180, 233));//�Ķ���
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

		for (int i = 0; i < 2; ++i) {



			MoveToEx(hDC, (int)Point[i].x, Point[i].y, NULL);
			LineTo(hDC, (int)Point[i].x - 10, Point[i].y + 20);


			MoveToEx(hDC, (int)Point[i].x - 10, Point[i].y + 20, NULL);
			LineTo(hDC, (int)Point[i].x + 10, Point[i].y + 20);

			MoveToEx(hDC, (int)Point[i].x + 10, Point[i].y + 20, NULL);
			LineTo(hDC, (int)Point[i].x, Point[i].y);
		}



	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void Bullet3::Release(void)
{
}
