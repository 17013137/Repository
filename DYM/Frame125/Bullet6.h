#pragma once
#include "Obj.h"
class Bullet6 :
	public CObj
{
public:
	Bullet6();
	virtual ~Bullet6();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:
	CObj*		Create_Bullet(D3DXVECTOR3 _Dir);
public:
	DWORD dwTime = GetTickCount();
	int delay = 0;
	bool isgo = false;

	D3DXMATRIX BulletWorld;
	list<CObj*>*			m_pBullet;
};

