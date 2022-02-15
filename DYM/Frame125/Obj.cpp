#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	: m_fSpeed(0.f), m_bDead(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CObj::~CObj()
{
}

void CObj::Set_Pos(int _iX, int _iY)
{
	m_tInfo.vPos.x = (float)_iX;
	m_tInfo.vPos.y = (float)_iY;
}

void CObj::Set_Pos(D3DXVECTOR3 _Pos)
{
	m_tInfo.vPos = _Pos;
}

void CObj::Set_Dir(D3DXVECTOR3 _Dir)
{
	m_tInfo.vDir = _Dir;
}

void CObj::Set_Angle(float _fAngle)
{
	m_fAngle = _fAngle;
}

void CObj::Update_Size()
{
	rect.left = m_tInfo.vPos.x - (sizeInfo.fX / 2);
	rect.top = m_tInfo.vPos.y - (sizeInfo.fY / 2);
	rect.right = m_tInfo.vPos.x + (sizeInfo.fX / 2);
	rect.bottom = m_tInfo.vPos.y + (sizeInfo.fY / 2);
}

void CObj::Collide(OBJID objid)
{
}

