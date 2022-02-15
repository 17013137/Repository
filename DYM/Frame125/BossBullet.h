#pragma once
#include "Obj.h"

class BossBullet : public CObj
{
public:
	BossBullet();
	~BossBullet();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;


};

