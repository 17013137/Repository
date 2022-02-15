#pragma once
#include "Scene.h"
class Start : public Scene
{
public:
	Start();
	virtual ~Start();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC _DC) override;
	virtual void Release(void) override;

private:
	DWORD dwTime = GetTickCount();
	int delay = 0;
};

