#pragma once
#include "Obj.h"
class CPlayer final :	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	void			Set_Bullet(list<CObj*>* _pBullet);

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:
	void		Key_Input(void);
	void		Update_Rect();
	CObj*		Create_Bullet(D3DXVECTOR3 _Dir);

private:
	D3DXVECTOR3		m_vPoint[4];
	D3DXVECTOR3		m_vOriginPoint[4];

	D3DXVECTOR3		m_vBarrelPoint[6];
	D3DXVECTOR3		m_vOriginBarrelPoint[6];
	D3DXVECTOR3		GunDir;
	D3DXMATRIX		GunMatWorld;

	float GunAngle;
	int select;
	AttackInfo attack;
	int bulletKind = 0;
	DWORD timer3;

};

