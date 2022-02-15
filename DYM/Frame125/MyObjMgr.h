#pragma once
#include "Include.h"

class CObj;
class MyObjMgr
{
public:
	MyObjMgr();
	~MyObjMgr();

public:
	list<CObj*>*	Get_ObjList(OBJID eID);
	CObj*			Get_Player() { return m_ObjList[PLAYER].front(); }
	CObj*			Get_Monster4() { return m_ObjList[MONSTER4].front(); }
	CObj*			Get_Monster2() { return m_ObjList[MONSTER2].front(); }
	int				Get_Monster2num() { return m_ObjList[MONSTER2].size(); }
	int				Get_Monster3num() { return m_ObjList[MONSTER3].size(); }
	int				Get_Monster4num() { return m_ObjList[MONSTER4].size(); }
	void			Add_Object(OBJID eID, CObj* pObj);
	void			Delete_Obj(OBJID eID) { m_ObjList[eID].clear(); }

public:
	int				Update(void);
	int				Late_Update(void);
	void			Render(HDC hDC);
	void			Release(void);


public:
	static MyObjMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new MyObjMgr;

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
	static		MyObjMgr*	m_pInstance;
};

