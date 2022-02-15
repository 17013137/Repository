#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;

	D3DXMATRIX			matWorld;
}INFO;

typedef struct Attack {
	bool attacking;
	DWORD cooltime;

	Attack() {
		attacking = false;
		cooltime = 0;
	}

}AttackInfo;

typedef struct SizeInfo {
	float fX;
	float fY;
	SizeInfo() {
		fX = 0.f;
		fY = 0.f;
	}
	SizeInfo(float _fX, float _fY) {
		fX = _fX;
		fY = _fY;
	}
}SizeInfo;

typedef struct Stat {
	int Max_Hp;
	int Hp;
	int Attack;

	Stat() {
		Max_Hp = 0;
		Hp = 0;
		Attack = 0;
	}

	Stat(int _Max_Hp, int _Hp, int _Attack) {
		Max_Hp = _Max_Hp;
		Hp = _Hp;
		Attack = _Attack;
	}
}STAT;