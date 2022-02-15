#pragma once
#include "Bullet7.h"
class BulletX :
	public Bullet7
{
public:
	BulletX();
	virtual ~BulletX();

	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;
};

