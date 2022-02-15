#include "stdafx.h"
#include "Ending.h"

Ending::Ending()
{
}


Ending::~Ending()
{
}

void Ending::Initialize(void)
{
	


		

	
}

void Ending::Update(void)
{
	
}

void Ending::Late_Update(void)
{

}

void Ending::Render(HDC _DC)
{




	TCHAR szBuf[32] = L"";
	TCHAR szBuf2[32] = L"";
	TCHAR szBuf3[32] = L"";


	if (delay > 20) {
		swprintf_s(szBuf, L"-------------");
		TextOut(_DC, 370, 300, szBuf, lstrlen(szBuf));

	}
	if (delay > 40) {

		swprintf_s(szBuf2, L" enjoy your cooking life! ");
		TextOut(_DC, 315, 320, szBuf2, lstrlen(szBuf2));

	}
	if (delay > 60) {
		swprintf_s(szBuf3, L"-------------");
		TextOut(_DC, 370, 340, szBuf3, lstrlen(szBuf3));
	}

	if (dwTime + 100 < GetTickCount())
	{
		delay++;
	}

}

void Ending::Release(void)
{
}
