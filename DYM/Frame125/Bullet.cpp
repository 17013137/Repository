#include "stdafx.h"
#include "Bullet.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

CBullet::CBullet()
{
	sizeInfo.fX = 20.f;
	sizeInfo.fY = 20.f;
}


CBullet::~CBullet()
{
	
	Release();
}


void CBullet::Initialize(void)
{
	m_fSpeed = 3.f;
	
}

int CBullet::Update(void)
{
	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
		return OBJ_DEAD;

	if (true == m_bDead)
		return OBJ_DEAD;
	
	// FSM(유한 상태 기계)
	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;
	Update_Size();

	return OBJ_NOEVENT;
}

void CBullet::Late_Update(void)
{
	
}


void CBullet::Render(HDC hDC)
{

	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(44, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(0, 180, 233));//파란색
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void CBullet::Release(void)
{

}
