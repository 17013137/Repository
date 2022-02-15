#pragma once

#include "Define.h"
#include "Player.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize(void);			// 데이터(멤버 변수)를 초기화하기 위한 함수
	void		Update(void);				// 매 프레임마다 데이터 갱신을 하기 위한 함수
	void		Late_Update(void);
	void		Render(void);				// 매 프레임마다 그리기를 수행하는 함수
	void		Release(void);				// 동적할당한 포인터를 해제하기 위한 함수


private:
	HDC			m_DC;			// 그림을 그리기 위해서는 dc가 필요하다. api에서는 dc를 얻어올 수 있는 함수가
								// 두개 있는데 하나는 beginpaint이고 하나는 GetDC;
								// 우리가 그리기를 하려는 dc는 윈도우 초기화 창에 해당하는 dc이며
								// 멤버 변수로 선언한 m_DC은 윈도우 초기화 창의 dc를 얻어와서 보관하기 위한 녀석이다.

	DWORD time2;
	int cnt;
};

