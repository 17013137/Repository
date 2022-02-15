#include "stdafx.h"
#include "Start.h"
#include"SceneMgr.h"

Start::Start()
{
}


Start::~Start()
{
}

void Start::Initialize(void)
{
}

void Start::Update(void)
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		SceneMgr::Get_Instance()->Scene_Change(STAGE1);
	}

}

void Start::Late_Update(void)
{
}

void Start::Render(HDC _DC)
{

	TCHAR szBuf[32] = L"";
	TCHAR szBuf2[32] = L"";
	TCHAR szBuf3[32] = L"";


	if (delay > 30) {
		swprintf_s(szBuf, L"Cooking Life Start!");
		TextOut(_DC, 330, 300, szBuf, lstrlen(szBuf));
	
	}
	if (delay > 60) {

		swprintf_s(szBuf2, L" press your enter key...");
		TextOut(_DC, 315, 320, szBuf2, lstrlen(szBuf2));

	}
	if (delay > 90) {
		swprintf_s(szBuf2, L" - DYM - ");
		TextOut(_DC, 360, 340, szBuf2, lstrlen(szBuf2));
	}
	if (dwTime + 100 < GetTickCount())
	{
		delay++;
	}


}

void Start::Release(void)
{
}
