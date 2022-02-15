#pragma once
#include "Obj.h"
class CMonster :
	public CObj
{
public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;

private:
	int		iDir = 1;

	list<CObj*>*			m_pBullet;
};

