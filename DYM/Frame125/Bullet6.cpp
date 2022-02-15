#include "stdafx.h"
#include "Bullet6.h"
#include "AbstractFactory.h"
#include "Bullet1_1.h"
#include "Bullet1_2.h"
#include "MyObjMgr.h"

Bullet6::Bullet6()
{
}


Bullet6::~Bullet6()
{
}

void Bullet6::Initialize(void)
{
	m_fSpeed = 3.f;
}

int Bullet6::Update(void)
{
	if (20 >= m_tInfo.vPos.x || WINCX - 20 <= m_tInfo.vPos.x || 20 >= m_tInfo.vPos.y || WINCY - 20 <= m_tInfo.vPos.y)
		m_bDead = true;


	if (true == m_bDead)
		return OBJ_DEAD;

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

	if (delay > 10)
	{
		isgo = true;

	}

	if (dwTime + 300 < GetTickCount())
	{
		delay++;
	}


	return OBJ_NOEVENT;
}

void Bullet6::Late_Update(void)
{
	if (isgo)
	{
		Create_Bullet(m_tInfo.vDir);
		m_bDead = true;
	}
}

void Bullet6::Render(HDC hDC)
{
	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;

	hbr = CreateSolidBrush(RGB(255, 0, 0));//ÆÄ¶õ»ö
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, (int)m_tInfo.vPos.x - 20, (int)m_tInfo.vPos.y - 20, (int)m_tInfo.vPos.x + 20, (int)m_tInfo.vPos.y + 20);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);

}

void Bullet6::Release(void)
{
}

CObj * Bullet6::Create_Bullet(D3DXVECTOR3 _Dir)
{
	D3DXVECTOR3 Temp = _Dir;
	m_tInfo.vLook = { 1.f, 1.f, 1.f };
	m_fAngle = 0.8f;
	D3DXMATRIX MatScale, BulletRot, MatTrans;
	D3DXMatrixScaling(&MatScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&BulletRot, m_fAngle);
	D3DXMatrixTranslation(&MatTrans, Temp.x, Temp.y, Temp.z);
	BulletWorld = MatScale*BulletRot*MatTrans;

	D3DXVec3TransformCoord(&Temp, &Temp, &BulletWorld);

	CObj*	pBullet1 = CAbstractFactory<Bullet1_1>::Create(m_tInfo.vPos, Temp);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet1);

	Temp = _Dir;
	m_fAngle = -0.8f;
	D3DXMatrixScaling(&MatScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&BulletRot, m_fAngle);
	D3DXMatrixTranslation(&MatTrans, Temp.x, Temp.y, Temp.z);
	BulletWorld = MatScale*BulletRot*MatTrans;

	D3DXVec3TransformCoord(&Temp, &_Dir, &BulletWorld);

	CObj*	pBullet2 = CAbstractFactory<Bullet1_2>::Create(m_tInfo.vPos, Temp);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet2);

	return pBullet1, pBullet2;
}
