#pragma once
#include "Obj.h"
class Boss : public CObj
{
public:
	Boss();
	virtual ~Boss();
public:
	void Update_Size();
	void Pattern1();
	void Pattern2();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

	RECT rectinfo;
	AttackInfo sk1set;
	AttackInfo sk1;
	AttackInfo sk2;
	DWORD timer;
	bool flag;
};

