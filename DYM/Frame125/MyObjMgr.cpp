#include "stdafx.h"
#include "MyObjMgr.h"
#include "Obj.h"
#include "SceneMgr.h"
MyObjMgr*	MyObjMgr::m_pInstance = nullptr;
MyObjMgr::MyObjMgr()
{
}


MyObjMgr::~MyObjMgr()
{
}

list<CObj*>* MyObjMgr::Get_ObjList(OBJID eID)
{
	return &m_ObjList[eID];
}

void MyObjMgr::Add_Object(OBJID eID, CObj * pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

int MyObjMgr::Update(void)
{

	int iEvent = 0;

	for (int i = 0; i < OBJ_END; ++i)
	{
		auto& iter = m_ObjList[i].begin();

		for (; iter != m_ObjList[i].end(); )
		{
			iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iEvent;
}

int MyObjMgr::Late_Update(void)
{


	//if (m_ObjList[MONSTER2].size() == 0)
	//{

	//	SceneMgr::Get_Instance()->Scene_Change(ENDING);
	//}



	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Late_Update();
	}

	return 0;
}

void MyObjMgr::Render(HDC hDC)
{

	TCHAR szBuf[32] = L"";
	swprintf_s(szBuf, L"Bullet: %d", m_ObjList[BULLET].size());
	TextOut(hDC, 120, 50, szBuf, lstrlen(szBuf));

	swprintf_s(szBuf, L"Monster: %d", m_ObjList[MONSTER].size());
	TextOut(hDC, 120, 70, szBuf, lstrlen(szBuf));



	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}
}

void MyObjMgr::Release(void)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}
