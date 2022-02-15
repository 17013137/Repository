#pragma once
#include "Scene.h"
class Stage3 :
	public Scene
{
public:
	Stage3();
	virtual ~Stage3();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC _DC) override;
	virtual void Release(void) override;

private:
	int cnt;
	bool isgo;
	DWORD delay;
};

