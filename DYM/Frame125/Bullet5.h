#pragma once
#include "Obj.h"
class Bullet5 : public CObj
{
public:
	Bullet5();
	virtual	~Bullet5();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:
	DWORD dwTime = GetTickCount();
	int delay = 0;
	bool isgo = false;


private:
	D3DXVECTOR3		m_vBarrelPoint[12];
	D3DXVECTOR3		m_vOriginBarrelPoint[12];
};

