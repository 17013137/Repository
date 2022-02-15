#pragma once
#include "Obj.h"
#include "Bullet.h"
class Razer : public CObj
{
public:
	Razer();
	virtual ~Razer();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;
	void				Update_Rect();
private:
	AttackInfo attack;
	INFO bull;
	bool flag;
	CObj* temp;
};

