#pragma once

#include "Define.h"
#include "Player.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize(void);			// ������(��� ����)�� �ʱ�ȭ�ϱ� ���� �Լ�
	void		Update(void);				// �� �����Ӹ��� ������ ������ �ϱ� ���� �Լ�
	void		Late_Update(void);
	void		Render(void);				// �� �����Ӹ��� �׸��⸦ �����ϴ� �Լ�
	void		Release(void);				// �����Ҵ��� �����͸� �����ϱ� ���� �Լ�


private:
	HDC			m_DC;			// �׸��� �׸��� ���ؼ��� dc�� �ʿ��ϴ�. api������ dc�� ���� �� �ִ� �Լ���
								// �ΰ� �ִµ� �ϳ��� beginpaint�̰� �ϳ��� GetDC;
								// �츮�� �׸��⸦ �Ϸ��� dc�� ������ �ʱ�ȭ â�� �ش��ϴ� dc�̸�
								// ��� ������ ������ m_DC�� ������ �ʱ�ȭ â�� dc�� ���ͼ� �����ϱ� ���� �༮�̴�.

	DWORD time2;
	int cnt;
};

