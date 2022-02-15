#pragma once
#include "Obj.h"
class Bullet3 : public CObj
{
public:
	Bullet3();
	virtual ~Bullet3();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:

private:
	D3DXVECTOR3 Point[2];
	D3DXVECTOR3	originPoint[2];
	DWORD dwTime = GetTickCount();
	int delay = 0;
	bool isgo = false;

};

