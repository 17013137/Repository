#pragma once
#include "Obj.h"
class Hack : public CObj
{
public:
	Hack();
	virtual ~Hack();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;
	void				Update_Rect();

private:
	AttackInfo attack;
	DWORD time2;
	bool boom;
};

