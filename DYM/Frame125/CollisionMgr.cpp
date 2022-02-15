#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "MyObjMgr.h"
CollisionMgr::CollisionMgr()
{
}


CollisionMgr::~CollisionMgr()
{
}



int CollisionMgr::Collision_Rect(CObj* Dest, list<CObj*>& Src)
{
	RECT	rc{};
	float X;
	float Y;

	for (auto& SrcIter : Src)
	{
		if (Check_Rect(Dest, SrcIter, &X, &Y))
		{
			return 1;
			
		}
	}
	return 0;
}


bool CollisionMgr::Check_Sphere(CObj* pDest, CObj* pSrc)
{
	float		fWidth = fabs(pDest->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
	float		fHeight = fabs(pDest->Get_Info().vPos.y - pSrc->Get_Info().vPos.y);

	float		fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float		fRadius = (pDest->Get_Size().fX + pSrc->Get_Size().fX) * 0.5f;

	return fRadius > fDistance;		// 충돌 했다
}


bool CollisionMgr::Check_Rect(CObj* pDest, CObj* pSrc, float* _pX, float* _pY)
{
	float		fWidth = abs(pDest->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
	float		fHeight = abs(pDest->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);

	// float		fCX = pDest->Get_Info().fCX / 2.f + pSrc->Get_Info().fCX / 2.f;
	// 위의 식을 아래 식으로 변경했을 뿐
	float		fCX = (pDest->Get_Size().fX + pSrc->Get_Size().fX) * 0.5f;
	float		fCY = (pDest->Get_Size().fY + pSrc->Get_Size().fY) * 0.5f;

	if (fCX > fWidth && fCY > fHeight)
	{
		*_pX = fCX - fWidth;
		*_pY = fCY - fHeight;

		return true;
	}
	return false;
}


int CollisionMgr::Collision_Sphere(list<CObj*>* Dest, list<CObj*>* Src)
{
	for (auto& DestIter : *Dest)
	{
		for (auto& SrcIter : *Src)
		{
			if (Check_Sphere(DestIter, SrcIter))
			{
				DestIter->Set_Dead();
				SrcIter->Set_Dead();
				return 1;
			}
		}
	}

	return 0;
}

int CollisionMgr::Collision_Sphere(CObj* Dest, list<CObj*> Src)
{
		for (auto& SrcIter : Src)
		{
			if (Check_Sphere(Dest, SrcIter))
			{
				return 1;
			}
		}

	return 0;
}

int CollisionMgr::Collision_Sphere2(list<CObj*>* Dest, list<CObj*>* Src)
{
	for (auto& DestIter : *Dest)
	{
		for (auto& SrcIter : *Src)
		{
			if (Check_Sphere(DestIter, SrcIter))
			{
				if (DestIter->getID() == BULLET && SrcIter->getID() == MONSTER2)
				{
					SrcIter->Set_Dead();
					SrcIter->Collide(BULLET);
				}
				else {
					DestIter->Set_Dead();
					SrcIter->Collide(BULLET);
				}


				if (DestIter->getID() == BULLET && SrcIter->getID() == MONSTER2)
				{
					
					SrcIter->Collide(BULLET);
				}
				else {
					DestIter->Set_Dead();
					SrcIter->Collide(BULLET);
				}

			}
		}
	}

	return 0;
}

void CollisionMgr::Collision_Sphere3(list<CObj*>& dst, list<CObj*>& src)
{

}


