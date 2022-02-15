#pragma once
#include "Obj.h"
class S2Monster1: public CObj
{
public:
	S2Monster1();
	virtual ~S2Monster1();


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

};

