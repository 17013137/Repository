#pragma once
#include "Obj.h"
class Bullet7 :
	public CObj
{
public:
	Bullet7();
	virtual ~Bullet7();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:
	CObj*		Create_Bullet1(D3DXVECTOR3 _Dir);
	CObj*		Create_Bullet2(D3DXVECTOR3 _Dir);
	CObj*		Create_Bullet3(D3DXVECTOR3 _Dir);
	CObj*		Create_Bullet4(D3DXVECTOR3 _Dir);
	void			Update_Rect(void);

public:
	D3DXVECTOR3		m_BulletPoint[16];
	D3DXVECTOR3		m_OriginBulletPoint[16];

	D3DXVECTOR3		BulletDir;

	DWORD dwTime = GetTickCount();

	bool BulletX1 = false;
	bool BulletX2 = false;
	bool BulletX3 = false;
	bool BulletX4 = false;
	int delay = 0;
	float BulletAngle;

	D3DXMATRIX BulletWorld;
	list<CObj*>*			m_pBullet;
};

