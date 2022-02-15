#pragma once
#include "Obj.h"
class RazerBullet :
	public CObj
{
public:
	RazerBullet();
	virtual ~RazerBullet();

public:

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

};
