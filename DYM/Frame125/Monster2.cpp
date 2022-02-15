#include "stdafx.h"
#include "Monster2.h"
//���콺�� ���󰡴� ����

CMonster2::CMonster2()
{
}


CMonster2::~CMonster2()
{
}

void CMonster2::Initialize(void)
{
	m_tInfo.vPos = { 700.f, 100.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 8.f;
	
}

int CMonster2::Update(void)
{
#pragma region ���콺�� ����ٴϴ� �Լ��̻�� ����
	/*m_tInfo.vDir = ::Get_Mouse() - m_tInfo.vPos;

	float	fDiagonal = sqrtf(m_tInfo.vDir.x*m_tInfo.vDir.x + m_tInfo.vDir.y*m_tInfo.vDir.y);

	m_tInfo.vDir.x /= fDiagonal;
	m_tInfo.vDir.y /= fDiagonal;
	m_tInfo.vDir.z = 0.f;

	fDiagonal = sqrtf(m_tInfo.vLook.x*m_tInfo.vLook.x + m_tInfo.vLook.y*m_tInfo.vLook.y);

	m_tInfo.vLook.x /= fDiagonal;
	m_tInfo.vLook.y /= fDiagonal;
	m_tInfo.vLook.z = 0.f;

	float fDot = (m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y);
	
	float fAngle = acosf(fDot);

	if (m_tInfo.vPos.y < Get_Mouse().y)
		fAngle = 2.f*D3DX_PI - fAngle;

	m_tInfo.vPos.x += m_fSpeed * cosf(fAngle);
	m_tInfo.vPos.y -= m_fSpeed * sinf(fAngle);*/
#pragma endregion ���콺�� ����ٴϴ� �Լ��̻�� ����

#pragma region ���콺�� ����ٴϴ� �Լ���� ����
	m_tInfo.vDir = Get_Mouse() - m_tInfo.vPos;

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	float fDot = D3DXVec3Dot(&m_tInfo.vLook, &m_tInfo.vDir);

	float fAngle = acosf(fDot);

	if (m_tInfo.vPos.y < Get_Mouse().y)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_tInfo.vPos.x += m_fSpeed * cosf(fAngle);
	m_tInfo.vPos.y -= m_fSpeed * sinf(fAngle);
#pragma endregion ���콺�� ����ٴϴ� �Լ� ��� ����

	return 0;
}

void CMonster2::Late_Update(void)
{
}

void CMonster2::Render(HDC hDC)
{
	Ellipse(hDC, (int)m_tInfo.vPos.x-20, (int)m_tInfo.vPos.y - 20, (int)m_tInfo.vPos.x + 20, (int)m_tInfo.vPos.y + 20);
}

void CMonster2::Release(void)
{
}
