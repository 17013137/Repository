#include "stdafx.h"
#include "Hack.h"
#include "CollisionMgr.h"
#include "MyObjMgr.h"

Hack::Hack()
{
	sizeInfo.fX = 10.f;
	sizeInfo.fY = 10.f;
	time2 = 0;
	boom = false;
}


Hack::~Hack()
{
}

void Hack::Initialize(void)
{
	m_fSpeed = 3.f;
	Update_Size();
}

int Hack::Update(void)
{

	if (m_tInfo.vPos.x <= 0 || m_tInfo.vPos.x >= WINCX || m_tInfo.vPos.y <= 0 || m_tInfo.vPos.y >= WINCY)
		return OBJ_DEAD;


	if (CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(MONSTER)) ||
		CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(MONSTER2)) ||
		CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(MONSTER3)) ||
		CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(MONSTER4)) ||
		CollisionMgr::Collision_Sphere(this, *MyObjMgr::Get_Instance()->Get_ObjList(YHMONSTER)))
		boom = true;


	if (boom) {
		if (sizeInfo.fX > 500.f) {
			if (time2 == 0)
				time2 = GetTickCount();
			if (time2 + 500 < GetTickCount())
				return OBJ_DEAD;

			return 0;
		}

		sizeInfo.fX += 7.f;
		sizeInfo.fY += 7.f;
	}
	else
		m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;


	Update_Size();

	

	if (true == m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void Hack::Late_Update(void)
{
}

void Hack::Render(HDC hDC)
{
	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(255, 212, 0));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);
}

void Hack::Release(void)
{

}

void Hack::Update_Rect()
{

}
