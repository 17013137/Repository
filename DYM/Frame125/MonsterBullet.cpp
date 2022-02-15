#include "stdafx.h"
#include "MonsterBullet.h"


CMonsterBullet::CMonsterBullet()
{
}


CMonsterBullet::~CMonsterBullet()
{
}

void CMonsterBullet::Initialize(void)
{
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;

	float fDiagonal = sqrtf(m_tInfo.vDir.x*m_tInfo.vDir.x + m_tInfo.vDir.y*m_tInfo.vDir.y);

	m_tInfo.vDir.x /= fDiagonal;
	m_tInfo.vDir.y /= fDiagonal;
	m_tInfo.vDir.z = 0.f;

	m_fSpeed = 5.f;

	
}

int CMonsterBullet::Update(void)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	D3DXMATRIX matScale, matRotZ, matTrans;
	
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale*matRotZ*matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vDir, &m_tInfo.matWorld);
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		
	return OBJ_NOEVENT;
}

void CMonsterBullet::Late_Update(void)
{
}

void CMonsterBullet::Render(HDC hDC)
{
}

void CMonsterBullet::Release(void)
{
}
