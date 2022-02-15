#pragma once
#include "Include.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual void	Initialize(void)PURE;
	virtual void	Update(void)PURE;
	virtual void	Late_Update(void)PURE;
	virtual void	Render(HDC _DC)PURE;
	virtual void	Release(void)PURE;

};

