#pragma once
#include "Bullet6.h"
class Bullet1_2 :
	public Bullet6
{
public:
	Bullet1_2();
	virtual ~Bullet1_2();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

};

