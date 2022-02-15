#pragma once
#include "Scene.h"
#include <random>
class Stage2 : public Scene
{
public:
	Stage2();
	virtual ~Stage2();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC _DC) override;
	virtual void Release(void) override;



private:
	uniform_int_distribution<>uidangle{ 30,80 };
	uniform_int_distribution<>uidcarrot{ 100,200 };
	uniform_int_distribution<>uid{ 150,600 };
	default_random_engine dre;

	bool goyellow = false;
	bool gocarrot = false;
	bool gogreen = false;
	DWORD dwTime = GetTickCount();
	int delay = 0;
	

};

