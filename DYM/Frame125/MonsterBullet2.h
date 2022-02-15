#pragma once
#include "Obj.h"
class MonsterBullet2 :
	public CObj
{
public:
	MonsterBullet2();
	virtual ~MonsterBullet2();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;
};

