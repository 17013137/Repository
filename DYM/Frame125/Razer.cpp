#include "stdafx.h"
#include "Razer.h"
#include "MyObjMgr.h"
#include "CollisionMgr.h"


Razer::Razer()
{
	sizeInfo.fX = 5.f;
	sizeInfo.fY = 5.f;
	m_fSpeed = 20.f;
}


Razer::~Razer()
{
}

void Razer::Initialize(void)
{
	vPoint[0] = { -30.f, 0.f, 0.f };
	vPoint[1] = { -30.f, 1000.f, 0.f };
	vPoint[2] = { +30.f, 1000.f, 0.f };
	vPoint[3] = { +30.f, 0.f, 0.f };


	for (int i = 0; i < 4; i++) {
		Origin_vPoint[i] = vPoint[i];
	}
}

int Razer::Update(void)
{

	if (m_tInfo.vPos.x <= 110 || m_tInfo.vPos.x >= WINCX - 110 || m_tInfo.vPos.y <= 50 || m_tInfo.vPos.y >= WINCY - 50)
		return OBJ_DEAD;

	for (int i = 0; i < 2; i++) {
		Origin_vPoint[i].x += 0.5f;

	}
	for (int i = 2; i < 4; i++) {
		Origin_vPoint[i].x -= 0.5f;
	}



	if (Origin_vPoint[0].x > 0)
		m_bDead = true;

	if (true == m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	Update_Size();
	return OBJ_NOEVENT;
}

void Razer::Late_Update(void)
{
}

void Razer::Render(HDC hDC)
{
	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(255, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(255, 255, 255));//ÆÄ¶õ»ö
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	//Rectangle(hDC, bull.vPos.x - 20, bull.vPos.y - 20, bull.vPos.x + 20, bull.vPos.y + 20);

	MoveToEx(hDC, (int)vPoint[0].x, (int)vPoint[0].y, NULL);
	for (int i = 0; i < 4; i++)
		LineTo(hDC, vPoint[i].x, vPoint[i].y);
	LineTo(hDC, (int)vPoint[0].x, (int)vPoint[0].y);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void Razer::Release(void)
{
}

void Razer::Update_Rect()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 1.f, -1.f, 1.f);
	D3DXMatrixRotationZ(&R, m_fAngle);
	D3DXMatrixTranslation(&T, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = S*R*T;
	for (int i = 0; i < 4; i++)
	{
		vPoint[i] = Origin_vPoint[i];
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &m_tInfo.matWorld);

	}


}
