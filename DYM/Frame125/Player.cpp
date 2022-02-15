#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "Razer.h"
#include "MyObjMgr.h"
#include "Hack.h"
#include "Bullet3.h"
#include "Bullet4.h"
#include "Bullet5.h"
#include "Bullet6.h"
#include "Bullet7.h"
#include "RazerBullet.h"

CPlayer::CPlayer()
	:GunAngle(0.f)
{
	sizeInfo.fX = 40.f;
	sizeInfo.fY = 40.f;
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	select = 0;
	timer3 = 0;

}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize(void)
{
	m_tInfo.vPos = {400.f, 600.f, 0.f};
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };

	m_vPoint[0] = { -30.f,- 30.f,0.f };
	m_vPoint[1] = { + 30.f,- 30.f,0.f };
	m_vPoint[2] = { +30.f, 20.f,0.f };
	m_vPoint[3] = { - 30.f, 20.f,0.f };
					  
	for (int i = 0; i < 4; ++i)
		m_vOriginPoint[i] = m_vPoint[i];

	m_fSpeed = 3.f;

	m_vBarrelPoint[0] = { 15.f, -10.f, 0.f };
	m_vBarrelPoint[1] = { +15.f,-10.f, 0.f };
	m_vBarrelPoint[2] = { +15.f,-50.f, 0.f };
	m_vBarrelPoint[3] = { 15.f, -55.f, 0.f };
	m_vBarrelPoint[4] = { -15.f, -50.f, 0.f };
	m_vBarrelPoint[5] = { -15.f, -10.f, 0.f };

	for (int i = 0; i < 6; ++i)
		m_vOriginBarrelPoint[i] = m_vBarrelPoint[i];

	Update_Size();
}

int CPlayer::Update(void)
{
	//º®Ãæµ¹Ã³¸®
	if (m_tInfo.vPos.x < 130)
		m_tInfo.vPos.x = 130;

	if (m_tInfo.vPos.x > WINCX - 130)
		m_tInfo.vPos.x = WINCX - 130;

	if (m_tInfo.vPos.y < 60)
		m_tInfo.vPos.y = 60;

	if (m_tInfo.vPos.y > WINCY - 60)
		m_tInfo.vPos.y = WINCY-60;


	Key_Input();
	Update_Rect();

	return OBJ_NOEVENT;
} 

void CPlayer::Late_Update(void)
{

}

void CPlayer::Render(HDC hDC)
{
	///*Rectangle(hDC, (int)m_tInfo.vPos.x -50, (int)m_tInfo.vPos.y - 50, (int)m_tInfo.vPos.x + 50, (int)m_tInfo.vPos.y + 50);*/
	HPEN hpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(44, 126, 147));
	HPEN hpenOld;
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);

	HBRUSH hbr;
	
	hbr = CreateSolidBrush(RGB(0, 180, 233));//ÆÄ¶õ»ö
	HBRUSH hbrold;
	hbrold = (HBRUSH)::SelectObject(hDC, hbr);

	MoveToEx(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y, NULL);
	for (int i = 0; i < 4; ++i)
	{
		LineTo(hDC, (int)m_vPoint[i].x, (int)m_vPoint[i].y);
	}
	LineTo(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y);





	MoveToEx(hDC, (int)m_vBarrelPoint[0].x, (int)m_vBarrelPoint[0].y-10, NULL);

	for (int i = 0; i < 6; ++i)
	{
		LineTo(hDC, (int)m_vBarrelPoint[i].x, (int)m_vBarrelPoint[i].y-10);
	}
	LineTo(hDC, (int)m_vBarrelPoint[0].x, (int)m_vBarrelPoint[0].y-10);


	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
	hbr = (HBRUSH)SelectObject(hDC, hbrold);
	::DeleteObject(hbr);


}

void CPlayer::Release(void)
{
	
}

void CPlayer::Key_Input(void)
{
	if (GetAsyncKeyState('1'))
		select = 0;
	else if (GetAsyncKeyState('2'))
		select = 1;
	else if (GetAsyncKeyState('3'))
		select = 2;
	//¹ÎÈñ
	else if (GetAsyncKeyState('4'))
		select = 3;
	else if (GetAsyncKeyState('5'))
		select = 4;
	else if (GetAsyncKeyState('6'))
		select = 5;
	else if (GetAsyncKeyState('7'))
		select = 6;

	if (GetAsyncKeyState('E'))
	{
		if (timer3 == 0) {
			timer3 = GetTickCount();
			select++;
		}

		if (timer3 + 50 < GetTickCount()) {
			timer3 = 0;
		}
	}

	if (GetAsyncKeyState('A'))
	{
		m_fAngle -= D3DXToRadian(3.f);
	}

	if (GetAsyncKeyState('D'))
	{
		m_fAngle += D3DXToRadian(3.f);
	}

	if (GetAsyncKeyState('W'))
	{
		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}

	if (GetAsyncKeyState('S'))
	{
		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
		m_tInfo.vPos -= m_tInfo.vDir * m_fSpeed;
	}

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		m_vBarrelPoint[2] = { +15.f,-20.f, 0.f };
		m_vBarrelPoint[4] = { -15.f, -20.f, 0.f };

		m_vBarrelPoint[0] = { 0.f, 0.f, 0.f };
		m_vBarrelPoint[1] = { +15.f,0.f, 0.f };
		m_vBarrelPoint[5] = { -15.f, 0.f, 0.f };

		m_vOriginBarrelPoint[0] = m_vBarrelPoint[0];
		m_vOriginBarrelPoint[1] = m_vBarrelPoint[1];
		m_vOriginBarrelPoint[2] = m_vBarrelPoint[2];
		m_vOriginBarrelPoint[4] = m_vBarrelPoint[4];
		m_vOriginBarrelPoint[5] = m_vBarrelPoint[5];

		if (attack.cooltime + 200 < GetTickCount()) { //°ø°Ý ÄðÅ½
			attack.attacking = false;
			attack.cooltime = 0;
		}

		if (!attack.attacking) {
			attack.attacking = true;
			attack.cooltime = GetTickCount();
		
			Create_Bullet(GunDir);
		}
	}
	else
	{

		m_vBarrelPoint[0] = { 0.f, -10.f, 0.f };
		m_vBarrelPoint[1] = { +15.f,-10.f, 0.f };


		m_vOriginBarrelPoint[0] = m_vBarrelPoint[0];
		m_vOriginBarrelPoint[1] = m_vBarrelPoint[1];



		m_vBarrelPoint[3] = { 0.f, -35.f, 0.f };
		m_vBarrelPoint[5] = { -15.f, -10.f, 0.f };

		m_vBarrelPoint[2] = { +15.f,-30.f, 0.f };
		m_vBarrelPoint[4] = { -15.f, -30.f, 0.f };
		m_vOriginBarrelPoint[2] = m_vBarrelPoint[2];
		m_vOriginBarrelPoint[4] = m_vBarrelPoint[4];

		m_vOriginBarrelPoint[3] = m_vBarrelPoint[3];
		m_vOriginBarrelPoint[5] = m_vBarrelPoint[5];
	}
}

void CPlayer::Update_Rect()
{
	D3DXMATRIX matScale/*Å©±â*/, matRotZ/*È¸Àü*/, matTrans/*ÀÌµ¿*/, GunRot;
	D3DXMatrixScaling(&matScale, 0.8f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale*matRotZ*matTrans;
	for (int i = 0; i < 4; ++i)
	{
		m_vPoint[i] = m_vOriginPoint[i];

		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_tInfo.matWorld);
	}

	GunDir = Get_Mouse() - m_tInfo.vPos;
	D3DXVec3Normalize(&GunDir, &GunDir);
	D3DXVECTOR3 Temp = { 1.f, 0.f, 0.f };
	GunAngle = (D3DXVec3Dot(&GunDir, &Temp));
	GunAngle = asinf(GunAngle);

	if (m_tInfo.vPos.y < Get_Mouse().y)
		GunAngle = 1.f * D3DX_PI - GunAngle;
	D3DXMatrixRotationZ(&GunRot, GunAngle);

	GunMatWorld = matScale*GunRot*matTrans;
	for (int i = 0; i < 6; ++i)
	{
		m_vBarrelPoint[i] = m_vOriginBarrelPoint[i];
		D3DXVec3TransformCoord(&m_vBarrelPoint[i], &m_vBarrelPoint[i], &GunMatWorld);
	}

}


CObj* CPlayer::Create_Bullet(D3DXVECTOR3 _Dir)
{
	CObj* pBullet = nullptr;
	if (select == 0) {
		pBullet = CAbstractFactory<CBullet>::Create(m_vBarrelPoint[3], _Dir);
		MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
	}
	else if (select == 1) {
		pBullet = CAbstractFactory<Razer>::Create(m_vBarrelPoint[3], GunAngle);
		MyObjMgr::Get_Instance()->Add_Object(RAZER, pBullet);
		MyObjMgr::Get_Instance()->Add_Object(RAZER, CAbstractFactory<RazerBullet>::Create(m_vBarrelPoint[3], _Dir));
	}
	else if (select == 2) {
		pBullet = CAbstractFactory<Hack>::Create(m_vBarrelPoint[3], _Dir);
		MyObjMgr::Get_Instance()->Add_Object(HACK, pBullet);
	}
	//¹ÎÈñ 4
	else if (select == 3)
	{
		CObj*		pBullet = CAbstractFactory<Bullet3>::Create(m_vBarrelPoint[3],_Dir);
		MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
		return pBullet;
	}  //5
	else if (select == 4)
	{
		CObj*		pBullet = CAbstractFactory<Bullet4>::Create(m_vBarrelPoint[3], GunDir);
		MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
		return pBullet;

	}
	else if (select == 5) {

		CObj*		pBullet = CAbstractFactory<Bullet6>::Create(m_vBarrelPoint[3], _Dir);
		MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
		return pBullet;
	}

	else if (select == 6) {

		CObj*		pBullet = CAbstractFactory<Bullet7>::Create(m_vBarrelPoint[3], _Dir);
		MyObjMgr::Get_Instance()->Add_Object(BULLET, pBullet);
		return pBullet;
	}

	return pBullet;
}

