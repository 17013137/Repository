#pragma once
#include "Obj.h"
class RD :
	public CObj
{
public:
	RD();
	virtual ~RD();

public:
	virtual void		Initialize(void) override;
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC) override;
	virtual void		Release(void) override;


	CObj*		Create_MonsterBullet(D3DXVECTOR3 _Dir);

private:
	int		iDir = 1;


	list<CObj*>*			m_pMonsterBullet;
};

