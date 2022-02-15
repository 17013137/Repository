#include "stdafx.h"
#include "Bullet7.h"
#include "MyObjMgr.h"
#include "AbstractFactory.h"
#include "BulletX.h"


Bullet7::Bullet7()
{
}


Bullet7::~Bullet7()
{
}

void Bullet7::Initialize(void)
{

	// 0, 4, 8, 12 ÃÑ¾Ë³ª°¡´Â ÁÂÇ¥
	m_BulletPoint[0] = { m_tInfo.vPos.x, m_tInfo.vPos.y - 15.f, 0.f };

	m_BulletPoint[1] = { m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y - 15.f, 0.f };
	m_BulletPoint[2] = { m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y - 5.f, 0.f };
	m_BulletPoint[3] = { m_tInfo.vPos.x + 15.f, m_tInfo.vPos.y - 5.f, 0.f };

	m_BulletPoint[4] = { m_tInfo.vPos.x + 15.f, m_tInfo.vPos.y, 0.f };

	m_BulletPoint[5] = { m_tInfo.vPos.x + 15.f, m_tInfo.vPos.y + 5.f, 0.f };
	m_BulletPoint[6] = { m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y + 5.f, 0.f };
	m_BulletPoint[7] = { m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y + 15.f, 0.f };

	m_BulletPoint[8] = { m_tInfo.vPos.x, m_tInfo.vPos.y + 15.f, 0.f };

	m_BulletPoint[9] = { m_tInfo.vPos.x - 5.f, m_tInfo.vPos.y + 15.f, 0.f };
	m_BulletPoint[10] = { m_tInfo.vPos.x - 5.f, m_tInfo.vPos.y + 5.f, 0.f };
	m_BulletPoint[11] = { m_tInfo.vPos.x - 15.f, m_tInfo.vPos.y + 5.f, 0.f };

	m_BulletPoint[12] = { m_tInfo.vPos.x - 15.f, m_tInfo.vPos.y, 0.f };

	m_BulletPoint[13] = { m_tInfo.vPos.x - 15.f, m_tInfo.vPos.y - 5.f, 0.f };
	m_BulletPoint[14] = { m_tInfo.vPos.x - 5.f, m_tInfo.vPos.y - 5.f, 0.f };
	m_BulletPoint[15] = { m_tInfo.vPos.x - 5.f, m_tInfo.vPos.y - 15.f, 0.f };

	for (int i = 0; i < 16; ++i)
		m_OriginBulletPoint[i] = m_BulletPoint[i];

	m_fSpeed = 1.f;

}

int Bullet7::Update(void)
{


	m_tInfo.vPos += m_tInfo.vDir*m_fSpeed;

	Update_Rect();

	if (20 >= m_tInfo.vPos.x || WINCX - 20 <= m_tInfo.vPos.x || 20 >= m_tInfo.vPos.y || WINCY - 20 <= m_tInfo.vPos.y)
		m_bDead = true;


	if (true == m_bDead)
		return OBJ_DEAD;





	//if (dwTime + 300 < GetTickCount())
	//{
	//	BulletDir = m_BulletPoint[0] - m_tInfo.vPos;
	//	Create_Bullet1(BulletDir);
	//}
	//if (dwTime + 600 < GetTickCount())
	//{
	//	BulletDir = m_BulletPoint[4] - m_tInfo.vPos;
	//	Create_Bullet2(BulletDir);
	//}
	//if (dwTime + 900 < GetTickCount())
	//{
	//	BulletDir = m_BulletPoint[8] - m_tInfo.vPos;
	//	Create_Bullet3(BulletDir);
	//}
	//if (dwTime + 1200 < GetTickCount())
	//{
	//	BulletDir = m_BulletPoint[12] - m_tInfo.vPos;
	//	Create_Bullet4(BulletDir);
	//}

	if (delay == 10)
	{
		BulletX1 = true;
	}
	else if (delay == 20)
	{
		BulletX2 = true;
	}
	else if (delay == 30)
	{
		BulletX3 = true;
	}
	else if (delay == 40)
	{
		BulletX4 = true;
	}

	if (dwTime + 800 < GetTickCount())
	{
		delay++;
	}

	return OBJ_NOEVENT;
}

void Bullet7::Late_Update(void)
{
	if (BulletX1)
	{
		BulletDir = m_BulletPoint[0] - m_tInfo.vPos;

		float fDiagonal = sqrtf(BulletDir.x*BulletDir.x + BulletDir.y*BulletDir.y);

		BulletDir.x /= fDiagonal;
		BulletDir.y /= fDiagonal;
		BulletDir.z = 0.f;

		Create_Bullet1(BulletDir);
		BulletX1 = false;
	}

	if (BulletX2)
	{
		BulletDir = m_BulletPoint[4] - m_tInfo.vPos;

		float fDiagonal = sqrtf(BulletDir.x*BulletDir.x + BulletDir.y*BulletDir.y);

		BulletDir.x /= fDiagonal;
		BulletDir.y /= fDiagonal;
		BulletDir.z = 0.f;

		Create_Bullet2(BulletDir);
		BulletX2 = false;
	}

	if (BulletX3)
	{
		BulletDir = m_BulletPoint[8] - m_tInfo.vPos;

		float fDiagonal = sqrtf(BulletDir.x*BulletDir.x + BulletDir.y*BulletDir.y);

		BulletDir.x /= fDiagonal;
		BulletDir.y /= fDiagonal;
		BulletDir.z = 0.f;

		Create_Bullet3(BulletDir);
		BulletX3 = false;
	}

	if (BulletX4)
	{
		BulletDir = m_BulletPoint[12] - m_tInfo.vPos;

		float fDiagonal = sqrtf(BulletDir.x*BulletDir.x + BulletDir.y*BulletDir.y);

		BulletDir.x /= fDiagonal;
		BulletDir.y /= fDiagonal;
		BulletDir.z = 0.f;

		Create_Bullet4(BulletDir);
		BulletX4 = false;
		delay = 0;
	}
}

void Bullet7::Render(HDC hDC)
{
	HPEN hpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	Ellipse(hDC, (int)m_tInfo.vPos.x - 10, (int)m_tInfo.vPos.y - 10, (int)m_tInfo.vPos.x + 10, (int)m_tInfo.vPos.y + 10);

	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);
	MoveToEx(hDC, (int)m_BulletPoint[0].x, (int)m_BulletPoint[0].y, NULL);
	for (int i = 0; i < 16; ++i)
	{
		LineTo(hDC, (int)m_BulletPoint[i].x, (int)m_BulletPoint[i].y);
	}
	LineTo(hDC, (int)m_BulletPoint[0].x, (int)m_BulletPoint[0].y);
}

void Bullet7::Release(void)
{
}

CObj * Bullet7::Create_Bullet1(D3DXVECTOR3 _Dir)
{
	CObj*	pBullet = CAbstractFactory<BulletX>::Create(m_BulletPoint[0], _Dir);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
	return pBullet;
}


CObj * Bullet7::Create_Bullet2(D3DXVECTOR3 _Dir)
{
	CObj*	pBullet = CAbstractFactory<BulletX>::Create(m_BulletPoint[4], _Dir);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
	return pBullet;
}

CObj * Bullet7::Create_Bullet3(D3DXVECTOR3 _Dir)
{
	CObj*	pBullet = CAbstractFactory<BulletX>::Create(m_BulletPoint[8], _Dir);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
	return pBullet;
}

CObj * Bullet7::Create_Bullet4(D3DXVECTOR3 _Dir)
{
	CObj*	pBullet = CAbstractFactory<BulletX>::Create(m_BulletPoint[12], _Dir);
	MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
	return pBullet;
}

void Bullet7::Update_Rect(void)
{

	D3DXMATRIX matScale, BulletRot, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	BulletAngle += D3DXToRadian(2.f);

	D3DXMatrixRotationZ(&BulletRot, BulletAngle);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale*BulletRot* matTrans;

	for (int i = 0; i < 16; ++i)
	{
		m_BulletPoint[i] = m_OriginBulletPoint[i];
		D3DXVec3TransformCoord(&m_BulletPoint[i], &m_BulletPoint[i], &m_tInfo.matWorld);
	}
}


