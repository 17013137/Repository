#pragma once
#include "Obj.h"

template<typename T>
class CAbstractFactory
{
public:

	CAbstractFactory()
	{
	}

	~CAbstractFactory()
	{
	}

public:
	static	CObj*		Create()
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static	CObj*		Create(int _iX, int _iY)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_iX, _iY);

		return pObj;
	}

	static	CObj*		Create(float _fX, float _fY, float _Angle)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_Angle);

		return pObj;
	}

	static	CObj*		Create(D3DXVECTOR3 _Pos)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_Pos);

		return pObj;
	}

	static	CObj*		Create(D3DXVECTOR3 _Pos, float _Angle)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_Pos);
		pObj->Set_Angle(_Angle);

		return pObj;
	}

	static	CObj*		Create(D3DXVECTOR3 _Pos, D3DXVECTOR3 _Dir)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_Pos);
		pObj->Set_Dir(_Dir);

		return pObj;
	}
};

