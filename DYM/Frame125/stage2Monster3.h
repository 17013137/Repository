#pragma once
#include "Obj.h"
#include<random>
class stage2Monster3 : public CObj
{
public:
	stage2Monster3();
	virtual~stage2Monster3();

public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
	virtual void		Collide(OBJID objid);

private:
	D3DXVECTOR3 Point[4];
	D3DXVECTOR3	originPoint[4];
	bool isgo = false;
	DWORD dwTime = GetTickCount();
	int delay = 0;

	bool isCollide = false;

private:
	uniform_int_distribution <> uid{ 3,10 };
	default_random_engine dre;
};

