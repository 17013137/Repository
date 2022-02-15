#pragma once
#include "Obj.h"

class BossBullet2 : public CObj
{
public:
	BossBullet2();
	~BossBullet2();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;


};

