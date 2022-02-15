#pragma once

class CObj;
class CollisionMgr
{
public:
	CollisionMgr();
	~CollisionMgr();

private:
	static bool			Check_Sphere(CObj* pDest, CObj* pSrc);
	static bool			Check_Rect(CObj* pDest, CObj* pSrc, float* _pX, float* _pY);


public:
	static int		Collision_Rect(CObj* Dest, list<CObj*>& Src);
	static int		Collision_Sphere(list<CObj*>* Dest, list<CObj*>* Src);
	static int		Collision_Sphere(CObj* Dest, list<CObj*> Src);


	static int		Collision_Sphere2(list<CObj*>* Dest, list<CObj*>* Src);
	static void		Collision_Sphere3(list<CObj*>& dst, list<CObj*>& src);

};

