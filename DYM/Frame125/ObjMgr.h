#pragma once

#include "Include.h"

// 메디에이터(중재자) + 싱글톤

class CObj;
class ObjMgr
{
private:
	ObjMgr();
	~ObjMgr();

public:
	CObj*			Get_Player() { return m_ObjList[PLAYER].front(); }
	CObj*			Get_Target(OBJID eID, CObj* pObj);
	list<CObj*>*	Get_ObjList(OBJID eID);

public:
	void			Add_Object(OBJID eID, CObj* pObj);

	int				Update(void);
	int				Late_Update(void);
	void			Render(HDC hDC);
	void			Release(void);
	void			Delete_ID(OBJID eID);

public:
	static ObjMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new ObjMgr;

		return m_pInstance;
	}
	static void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	list<CObj*>				m_ObjList[OBJ_END];
	static		ObjMgr*	m_pInstance;

};

