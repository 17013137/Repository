#pragma once

#include "Include.h"

class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	INFO		Get_Info(void) { return m_tInfo; }
	void		Set_Pos(int _iX, int _iY);
	void		Set_Pos(D3DXVECTOR3 _Pos);
	void		Set_Dir(D3DXVECTOR3 _Dir);
	void		Set_Angle(float _Dir);
	void		Set_Dead() { m_bDead = true; }
	void		Set_Info(INFO _info) { m_tInfo = _info; }
	const RECT&		Get_Rect() { return rect; }
	const SizeInfo&		Get_Size() { return sizeInfo; }
	D3DXVECTOR3*	Get_Point() { return vPoint; }
	OBJID getID() { return id; };
	void Update_Size();
public:
	virtual void		Initialize(void) = 0 ;			
	virtual int			Update(void) = 0;
	virtual void		Late_Update(void) = 0;
	virtual void		Render(HDC hDC) = 0;
	virtual void		Release(void) = 0;
	virtual void Collide(OBJID objid);

protected:
	INFO		m_tInfo;
	STAT		stat;
	float		m_fSpeed;
	float		m_fAngle = 0.f;
	bool		m_bDead;
	OBJID id;

	CObj*		m_pTarget = nullptr;
	SizeInfo sizeInfo;
	RECT rect;

	D3DXVECTOR3 vPoint[4];
	D3DXVECTOR3 Origin_vPoint[4];
};

