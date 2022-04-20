#include "stdafx.h"
#include "..\Public\PlayerCube.h"
#include "GameInstance.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "PlayerData.h"
#include "EventMgr.h"

CPlayerCube::CPlayerCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayerCube::CPlayerCube(const CPlayerCube & rhs)
	: CGameObject(rhs)

{
}

HRESULT CPlayerCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&TargetPos, sizeof(TargetPos));



	return S_OK;
}

HRESULT CPlayerCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.f, 1.f, 15.f));
	vAngle[0] = _float3(0.f, 0.f, 1.f);
	vAngle[1] = _float3(0.f, 0.f, -1.f);
	vAngle[2] = _float3(1.f, 0.f, 0.f);
	vAngle[3] = _float3(-1.f, 0.f, 0.f);
	vAngle[4] = _float3(0.5f, 0.f, 0.5f);
	vAngle[5] = _float3(-0.5f, 0.f, 0.5f);
	vAngle[6] = _float3(0.5f, 0.f, -0.5f);
	vAngle[7] = _float3(-0.5f, 0.f, -0.5f);

	CurrentHand = HAND_EMPTY;

	m_col_Range = 1.f;
	m_col_Size = { 1.f, 1.f };
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	return S_OK;
}

_int CPlayerCube::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	GunLook = m_vTF[3]->Get_StateWorld(CSubTransform::STATE_LOOK);
	GunPos = m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION);
	m_pGraphic_Device->LightEnable(1, FALSE);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse.r = 20.f;
	light.Diffuse.g = 20.f;
	light.Diffuse.b = 20.f;
	light.Diffuse.a = 0.f;
	light.Attenuation0 = 100.f;
	light.Range = 4.f;

	light.Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION)+ m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	light.Position.y =4.f;
	m_pGraphic_Device->SetLight(0, &light);

	

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;
	light.Diffuse.r = 30.f;
	light.Diffuse.g = 30.f;
	light.Diffuse.b = 30.f;
	light.Diffuse.a = 0.f;
	light.Attenuation0 = 30.f;
	//light.Attenuation1 = 5.f;
	//light.Attenuation2 = 5.f;
	light.Range = 30.f;
	light.Falloff = 1.f;
	light.Theta = D3DX_PI / 3.f;
	light.Phi = D3DX_PI / 2.f;
	light.Direction= m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	light.Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pGraphic_Device->SetLight(2, &light);
	
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (CheatMod)
			CheatMod = false;
		else
			CheatMod = true;
	}
	if (CheatMod)
	{
		m_pStatusCom->Heal(100.f);
		m_pStatusCom->Heal_Stamina(100.f);
	}

	//AngleState = ANGLE_STRAIGHE;
	//총알 정보
	/*
	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 1.f;
	StatusmDesc.fMaxStamina = 100.;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 10.f;
	StatusmDesc.fPower = 0.1f;
	StatusmDesc.frange = 1.f;

	StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;
	StatusmDesc.Transform_Com = m_pTransformCom;
	StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	StatusmDesc.fSpawnPos.y += 10.f;
	*/
	//지형 피킹
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_Terrain*		pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pVIBuffer_Terrain)
		return 0;

	CTransform*			pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTransform_Terrain)
		return 0;


	m_isChase = pVIBuffer_Terrain->Picking(pTransform_Terrain, &m_vTargetPos);
	_float3 LookAngle = m_vTargetPos;
	LookAngle.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	//CurrentAngle = ANGLE_STRAIGHERIGHT;

	



	if (State == STATE_WALK || State == STATE_STAY)
	{
		m_isChase = m_pTransformCom->LookAt(LookAngle);
	}
	else if (State == STATE_ROLL || State == STATE_THROW || State == STATE_ATTACK || State == STATE_FALLING || State == STATE_DOWN || State == STATE_UP)
	{
		//방향전환없음

	}

	else
	{
		m_pTransformCom->Look(vAngle[AngleState]);
	}


	key = false;

	if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
	{
		if (State != STATE_DOWN)
			Falling(fTimeDelta);
	}

	if (State== STATE_WALK|| State == STATE_STAY)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('G'))// 유탄
		{
			HandState = HAND_GRENADE;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('R'))
		{
			if (HandState == HAND_RIFLE)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::RELOAD);
				CSoundMgr::Get_Instance()->PlaySound(L"Rifle_Reload.wav", CSoundMgr::RELOAD, 1.f);
				m_pInventoryCom->Reload(CInventory::RIFLEAMMO);
			}
			else if (HandState == HAND_SHOTGUN)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::RELOAD);
				CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Reload.wav", CSoundMgr::RELOAD, 1.f);
				m_pInventoryCom->Reload(CInventory::SHOTGUNAMMO);
			}
			else if (HandState == HAND_PISTOL)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::RELOAD);
				CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Reload.wav", CSoundMgr::RELOAD, 1.f);
				m_pInventoryCom->Reload(CInventory::PISTOLAMMO);
			}
		}

		if(CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			if (HandState == HAND_GRENADE
				&& m_pInventoryCom->UseItem(CInventory::GRENADE))
			{
				if (m_pStatusCom->PlayAction(30.f))
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					CSoundMgr::Get_Instance()->PlaySound(L"Swing_0.wav", CSoundMgr::PLAYER_SHOT, 0.8f);
					Throw(fTimeDelta);
				}
				
			}

			else if (HandState == HAND_RIFLE&&Gundelay<=0)			
			{
				if (true == m_pInventoryCom->UseItem(CInventory::RIFLEAMMO))
				{
					m_pGraphic_Device->LightEnable(1, TRUE);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					GunPos.y = GunPos.y + 0.35f;
					GunPos = GunPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) / 3;
					CStatus::STATUSDESC		StatusmDesc;
					ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
					StatusmDesc.fMaxHp = 1.f;
					StatusmDesc.fMaxStamina = 0.f;
					StatusmDesc.fMaxStungauge = 50.f;
					StatusmDesc.fDamage = 100.f;
					StatusmDesc.fPower = 0.1f;
					StatusmDesc.frange = 2.f;
					StatusmDesc.fLook = GunLook * 150;
					StatusmDesc.fSpawnPos = GunPos;
					StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

					StatusmDesc.Transform_Com = m_pTransformCom;
					m_pTransformCom->Go_BackWard(0.01f);

					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;
					EFFECTTYPE m_Effect_Type;
					m_Effect_Type.Type = 0;
					m_Effect_Type.Pos = GunPos;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type)))
						return E_FAIL;


					ZeroMemory(&light, sizeof(light));
					light.Type = D3DLIGHT_POINT;
					light.Diffuse.r = 450.f;
					light.Diffuse.g = 135.f;
					light.Diffuse.b = 135.f;
					light.Diffuse.a = 0.f;
					light.Attenuation0 = 500.f;
					light.Attenuation1 = 10.f;
					light.Attenuation2 = 10.f;
					light.Range = 7.f;

					light.Position = GunPos + GunLook;
					light.Position.y += 0.1;
					m_pGraphic_Device->SetLight(1, &light);


					CSoundMgr::Get_Instance()->PlaySound(L"Rifle_Shot2.wav", CSoundMgr::PLAYER_SHOT, 0.6f);
					Gundelay = 0.1;
				}
				else if (false == m_pInventoryCom->UseItem(CInventory::RIFLEAMMO))
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Empty.wav", CSoundMgr::PLAYER_SHOT, 1.f);
				}
			}

			else if (HandState == HAND_SHOTGUN&&Gundelay <= 0)
			{
				if (true == m_pInventoryCom->UseItem(CInventory::SHOTGUNAMMO))
				{
					m_pGraphic_Device->LightEnable(1, TRUE);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					GunPos.y = GunPos.y + 0.35f;
					GunPos = GunPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) / 2;
					CStatus::STATUSDESC		StatusmDesc;
					ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
					StatusmDesc.fMaxHp = 1.f;
					StatusmDesc.fMaxStamina = 1.f;
					StatusmDesc.fMaxStungauge = 20.f;
					StatusmDesc.fDamage = 20.f;
					StatusmDesc.fPower = 0.2f;
					StatusmDesc.frange = 2.f;
					StatusmDesc.fLook = GunLook * 110;
					StatusmDesc.fSpawnPos = GunPos;
					StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;
					_float3 RandPos = StatusmDesc.fLook;
					StatusmDesc.Transform_Com = m_pTransformCom;
					m_pTransformCom->Go_BackWard(0.05f);

					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;
					StatusmDesc.fLook = _float3(RandPos.x + rand() % 10 - 5.f, RandPos.y, RandPos.z + rand() % 10 - 5.f);
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;
					StatusmDesc.fLook = _float3(RandPos.x + rand() % 10 - 5.f, RandPos.y, RandPos.z + rand() % 10 - 5.f);
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;
					StatusmDesc.fLook = _float3(RandPos.x + rand() % 10 - 5.f, RandPos.y, RandPos.z + rand() % 10 - 5.f);
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;

					EFFECTTYPE m_Effect_Type;
					m_Effect_Type.Type = 1;
					m_Effect_Type.Pos = GunPos;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type)))
						return E_FAIL;

					ZeroMemory(&light, sizeof(light));
					light.Type = D3DLIGHT_POINT;
					light.Diffuse.r = 600.f;
					light.Diffuse.g = 180.f;
					light.Diffuse.b = 180.f;
					light.Diffuse.a = 0.f;
					light.Attenuation0 = 500.f;
					light.Attenuation1 = 10.f;
					light.Attenuation2 = 10.f;
					light.Range = 10.f;

					light.Position = GunPos + GunLook;
					light.Position.y += 0.5;
					m_pGraphic_Device->SetLight(1, &light);

					CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.6f);

					Gundelay = 0.7;
				}
				else if (false == m_pInventoryCom->UseItem(CInventory::SHOTGUNAMMO))
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Empty.wav", CSoundMgr::PLAYER_SHOT, 1.f);
				}
			}

			else if (HandState == HAND_PISTOL&&Gundelay <= 0	)
			{
				if (true == m_pInventoryCom->UseItem(CInventory::PISTOLAMMO))
				{
					m_pGraphic_Device->LightEnable(1, TRUE);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					GunPos.y = GunPos.y + 0.45f;
					CStatus::STATUSDESC		StatusmDesc;
					ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
					StatusmDesc.fMaxHp = 1.f;
					StatusmDesc.fMaxStamina = 2.f;
					StatusmDesc.fMaxStungauge = 30.f;
					StatusmDesc.fDamage = 400.f;
					StatusmDesc.fPower = 0.3f;
					StatusmDesc.frange = 2.f;
					StatusmDesc.fLook = GunLook * 130;
					StatusmDesc.fSpawnPos = GunPos;
					StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

					StatusmDesc.Transform_Com = m_pTransformCom;
					m_pTransformCom->Go_BackWard(0.03f);

					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc)))
						return E_FAIL;

					EFFECTTYPE m_Effect_Type;
					m_Effect_Type.Type = 3;
					m_Effect_Type.Pos = GunPos;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type)))
						return E_FAIL;

					ZeroMemory(&light, sizeof(light));
					light.Type = D3DLIGHT_POINT;
					light.Diffuse.r = 300.f;
					light.Diffuse.g = 90.f;
					light.Diffuse.b = 90.f;
					light.Diffuse.a = 0.f;
					light.Attenuation0 = 500.f;
					light.Attenuation1 = 10.f;
					light.Attenuation2 = 10.f;
					light.Range = 5.f;

					light.Position = GunPos + GunLook;
					light.Position.y += 0.5;
					m_pGraphic_Device->SetLight(1, &light);

					CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.6f);

					Gundelay = 0.3;
				}
				else if (false == m_pInventoryCom->UseItem(CInventory::PISTOLAMMO))
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Empty.wav", CSoundMgr::PLAYER_SHOT, 1.f);
				}
			}

			else if (HandState == HAND_WEAPON)
			{
				if (m_pStatusCom->PlayAction(20.f))
				{
					Attack(fTimeDelta);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
					CSoundMgr::Get_Instance()->PlaySound(L"Hammer_1.mp3", CSoundMgr::PLAYER_SHOT, 1.f);
				}
				
			}
		}
	}
	if (State == STATE_THROW)
	{
		Throw(fTimeDelta);
	}
	if (State == STATE_ATTACK)
	{
		Attack(fTimeDelta);
	}
	if (State == STATE_DOWN)
	{
		Down(fTimeDelta);
	}
	if (State == STATE_UP)
	{
		Up(fTimeDelta);
	}
	if (State == STATE_ROLL)
	{
		Rolling(fTimeDelta);
	}

	// 구르기
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)&& key == false)
	{
		if (m_pStatusCom->PlayAction(20.f))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Swing_0.wav", CSoundMgr::ROLLING, 0.5f);
			Rolling(fTimeDelta);
		}
		
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('W') && key == false)
	{
		AngleState = ANGLE_STRAIGHE;
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))// 뛰기
		{
			if(m_pStatusCom->PlayAction(17.f*fTimeDelta))
			{
			Run(fTimeDelta);
			if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			{
				AngleState = ANGLE_STRAIGHERIGHT;
				m_pTransformCom->Go_X(fTimeDelta / 1.7f);
				m_pTransformCom->Go_Z(fTimeDelta / 1.7f);
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
			{
				AngleState = ANGLE_STRAIGHELEFT;
				m_pTransformCom->Go_X(-fTimeDelta / 1.7f);
				m_pTransformCom->Go_Z(fTimeDelta / 1.7f);
			}
			else
			{

				m_pTransformCom->Go_Z(fTimeDelta);
			}
		}
		}
		else //걷기
		{
			Walk(fTimeDelta);
			if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			{
				AngleState = ANGLE_STRAIGHERIGHT;
				m_pTransformCom->Go_X(fTimeDelta / 3.4f);
				m_pTransformCom->Go_Z(fTimeDelta / 3.4f);
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('A') )
			{
				AngleState = ANGLE_STRAIGHELEFT;
				m_pTransformCom->Go_X(-fTimeDelta / 3.4f);
				m_pTransformCom->Go_Z(fTimeDelta / 3.4f);
			}
			else
			{
				m_pTransformCom->Go_Z(fTimeDelta / 2);
			}

		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('S')&& key == false)
	{
		AngleState = ANGLE_BACK;
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))// 뛰기
		{
			if (m_pStatusCom->PlayAction(17.f*fTimeDelta))
			{
				Run(fTimeDelta);
				if (CKeyMgr::Get_Instance()->Key_Pressing('D') )
				{
					AngleState = ANGLE_BACKRIGHT;
					m_pTransformCom->Go_Z(-fTimeDelta / 1.7f);
					m_pTransformCom->Go_X(fTimeDelta / 1.7f);
				}
				else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
				{
					AngleState = ANGLE_BACKLEFT;
					m_pTransformCom->Go_Z(-fTimeDelta / 1.7f);
					m_pTransformCom->Go_X(-fTimeDelta / 1.7f);
				}
				else
				{

					m_pTransformCom->Go_Z(-fTimeDelta);
				}
			}
		}
		else //걷기
		{
			Walk(fTimeDelta);
			if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			{
				AngleState = ANGLE_BACKRIGHT;
				m_pTransformCom->Go_Z(-fTimeDelta / 3.4f);
				m_pTransformCom->Go_X(fTimeDelta / 3.4f);
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
			{
				AngleState = ANGLE_BACKLEFT;
				m_pTransformCom->Go_Z(-fTimeDelta / 3.4f);
				m_pTransformCom->Go_X(-fTimeDelta / 3.4f);
			}
			else
			{

				m_pTransformCom->Go_Z(-fTimeDelta / 2);
			}

		}
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing('D')&& key == false)
	{
		AngleState = ANGLE_RIGHT;
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))
		{
			if (m_pStatusCom->PlayAction(17.f*fTimeDelta))
			{
				Run(fTimeDelta);
				m_pTransformCom->Go_X(fTimeDelta);
			}
		}
		else
		{
			Walk(fTimeDelta);
			m_pTransformCom->Go_X(fTimeDelta / 2);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('A') && key == false)
	{
		AngleState = ANGLE_LEFT;
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))
		{
			if (m_pStatusCom->PlayAction(17.f*fTimeDelta))
			{
				Run(fTimeDelta);
				m_pTransformCom->Go_X(-fTimeDelta);
			}
		}
		else
		{
			Walk(fTimeDelta);
			m_pTransformCom->Go_X(-fTimeDelta / 2);
		}
	}

	//테스트용
	if (CKeyMgr::Get_Instance()->Key_Down('5') && HandState != HAND_EMPTY)
	{
		HandState = HAND_EMPTY;
		Set_Stay();
	}
	if (CKeyMgr::Get_Instance()->Key_Down('1') && HandState != HAND_RIFLE)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"Change.wav", CSoundMgr::WEAPON_CHANGE, 1.f);
		HandState = HAND_RIFLE;
		Set_Stay();
	}
	if (CKeyMgr::Get_Instance()->Key_Down('2') && HandState != HAND_SHOTGUN)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Reload.wav", CSoundMgr::WEAPON_CHANGE, 1.f);

		HandState = HAND_SHOTGUN;
		Set_Stay();
	}
	if (CKeyMgr::Get_Instance()->Key_Down('3') && HandState != HAND_PISTOL)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Reload.wav", CSoundMgr::WEAPON_CHANGE, 1.f);

		HandState = HAND_PISTOL;
		Set_Stay();
	}
	if (CKeyMgr::Get_Instance()->Key_Down('4') && HandState != HAND_WEAPON)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"Swing_0.wav", CSoundMgr::WEAPON_CHANGE, 0.7f);

		HandState = HAND_WEAPON;
		Set_Stay();
	}
	//if (CKeyMgr::Get_Instance()->Key_Down('6') || State == STATE_FALLING)
	//{
	//	//if (key == false)
	//		//Falling(fTimeDelta);

	//}


	if (key == false)
	{
		Stay(fTimeDelta);
	}

	//CurrentAngle = ANGLE_LEFT;
	//마우스 각도와 이동각도 사이각 구하기
	if (State == STATE_WALK)
	{
		_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float		WalkAngle = acos(D3DXVec3Dot(&vLook, &vAngle[AngleState])) * (180 / 3.14f);
		_float		fLook = vLook.z / vLook.x;
		_bool		Right;
		if (fLook * vAngle[AngleState].x > vAngle[AngleState].z)
		{
			if (vLook.x > 0)
			{
				Right = true;
			}
			else
			{
				Right = false;
			}
		}
		else
		{
			if (vLook.x > 0)
			{
				Right = false;
			}
			else
			{
				Right = true;
			}
		}

		if (WalkAngle < 30)
		{
			if (Right)
			{
				WalkState = WALK_STRAIGHERIGHT;
			}
			else
			{
				WalkState = WALK_STRAIGHELEFT;
			}

		}
		else if (WalkAngle >= 30 && WalkAngle < 150)
		{
			if (Right)
			{
				WalkState = WALK_RIGHT;
			}
			else
			{
				WalkState = WALK_LEFT;
			}
		}
		else
		{
			if (Right)
			{
				WalkState = WALK_BACKRIGHT;
			}
			else
			{
				WalkState = WALK_BACKLEFT;
			}
		}
	}
	//SetUp_OnTerrain();
		
	if (CKeyMgr::Get_Instance()->Key_Down('F') &&m_pInventoryCom->UseItem(CInventory::MEDICKIT))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DRINK);
		CSoundMgr::Get_Instance()->PlaySound(L"PainKiller.wav", CSoundMgr::DRINK, 0.8f);
		m_pStatusCom->Heal(50.f);
		m_pStatusCom->Heal_Stamina(50.f);
	}
	
	

	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECTTYPE m_Effect_Type;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BEATEN);
		CSoundMgr::Get_Instance()->PlaySound(L"Beaten.wav", CSoundMgr::PLAYER_BEATEN, 0.6f);
		m_Effect_Type.Type = (_int)(fTimeDelta * 1000) % 10;
		m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_Effect_Type.Target = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Blood_Effect"), &m_Effect_Type)))
			return E_FAIL;
	}
	Gundelay -= fTimeDelta;
	Safe_Release(pGameInstance);

	return 0;
}

void CPlayerCube::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	CPlayerData::Get_Instance()->Set_Inven(*m_pInventoryCom->Get_Inventory());

	//파츠별 위치 맞추기-------------------------------------------------------------
	_float3 Right;
	_float3 Up;
	_float3 Look;
	_float3 Pos;

	//어깨 위치 몸통에 맞추기
	Right = *D3DXVec3Normalize(&Right, &m_vTF[1]->Get_State(CSubTransform::STATE_RIGHT));
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));

	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[6]->Set_State(CSubTransform::STATE_POSITION, Pos + (Right * 0.4f) + (Up * 0.25f));
	m_vTF[7]->Set_State(CSubTransform::STATE_POSITION, Pos + -(Right * 0.4f) + (Up * 0.25f));

	//머리 위치 몸통에 맞추기
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[0]->Set_State(CSubTransform::STATE_POSITION, Pos + Up / 1.25f);



	m_vRD[1]->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPlayerCube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (CEventMgr::Get_Instance()->Get_OnAir() == true)
		return S_OK;


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(2)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;



	for (int i = 0; i < 10; ++i)
	{
		m_vTF[i]->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}
	m_vTFPistol->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	m_vTFTimber->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	m_vTFRifle->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	m_vTFShotGun->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	//손발 파츠 상하체에 고정
	Set_Part(2, 1, m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part(3, 1, m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part2(m_vTF[4], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(m_vTF[5], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(m_vTFGrenade, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[2]->Get_StateWorld(CSubTransform::STATE_UP) / 5.f);
	Set_Part(9, 1, m_vTF[1]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_LOOK) / 1.8);

	if (HandState == HAND_RIFLE)
	{
		Set_Part2(m_vTFRifle, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFRifle->LookAt(m_vTF[3]->Get_State(CSubTransform::STATE_POSITION));
		Set_Part2(m_vTF[3], m_vTFRifle->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTFRifle->Get_StateWorld(CSubTransform::STATE_LOOK) / 5.5f);

	}
	else if (HandState == HAND_SHOTGUN)
	{
		Set_Part2(m_vTFShotGun, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFShotGun->LookAt(m_vTF[3]->Get_State(CSubTransform::STATE_POSITION));
		Set_Part2(m_vTF[3], m_vTFShotGun->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTFShotGun->Get_StateWorld(CSubTransform::STATE_LOOK) / 7.5f);

	}
	else if (HandState == HAND_PISTOL)
	{
		m_vTFPistol->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));

		m_vTFPistol->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
		Set_Part2(m_vTFPistol, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFPistol->LookAt(m_vTF[3]->Get_State(CSubTransform::STATE_POSITION));
		Set_Part2(m_vTF[3], m_vTFPistol->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTFPistol->Get_StateWorld(CSubTransform::STATE_LOOK) / 3);
	}

	else if (HandState == HAND_WEAPON)
	{
		m_vTFTimber->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));

		m_vTFTimber->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
	}
	//어깨 각도 손으로 맞추기
	m_vTF[6]->LookAtWorld(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[7]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//골반 각도 발에 맞추기
	_float3 RF = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[4]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	_float3 LF = m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[5]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	if (State != STATE_ROLL)
	{
		m_vTF[8]->LookRight(RF - LF);
	}

	

	for (int i = 0; i < 10; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}

	if (HandState == HAND_GRENADE)
	{
		if (FAILED(m_vTTGrenade->Bind_OnGraphicDevice()))
			return E_FAIL;

		if (FAILED(m_vTFGrenade->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIGrenade->Render();
	}
	else if (HandState == HAND_RIFLE)
	{

		if (FAILED(m_vTTRifle->Bind_OnGraphicDevice(1)))
			return E_FAIL;

		if (FAILED(m_vTFRifle->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIRifle->Render();
	}
	else if (HandState == HAND_SHOTGUN)
	{

		if (FAILED(m_vTTShotGun->Bind_OnGraphicDevice(2)))
			return E_FAIL;

		if (FAILED(m_vTFShotGun->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIShotGun->Render();
	}
	else if (HandState == HAND_PISTOL)
	{

		if (FAILED(m_vTTPistol->Bind_OnGraphicDevice(3)))
			return E_FAIL;

		if (FAILED(m_vTFPistol->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIPistol->Render();
	}
	else if (HandState == HAND_WEAPON)
	{

		if (FAILED(m_vTTTimber->Bind_OnGraphicDevice(0)))
			return E_FAIL;

		if (FAILED(m_vTFTimber->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVITimber->Render();
	}



	//Compute_CamDistance(m_pTransformCom);
	//m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	
	return S_OK;
}

HRESULT CPlayerCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_PlayerCube"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 150.f;
	StatusmDesc.fMaxStamina = 100.f;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 100.f;
	StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;
	StatusmDesc.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &StatusmDesc)))
		return E_FAIL;
	InventorymDesc = CPlayerData::Get_Instance()->Get_Inven();


	if (FAILED(__super::SetUp_Components(TEXT("Com_Inventory"), LEVEL_STATIC, TEXT("Prototype_Component_Inventory"), (CComponent**)&m_pInventoryCom, InventorymDesc)))
		return E_FAIL;
	//그림자


	//무기(Rifle)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Rifle"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFRifle, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Rifle"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDRifle)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Rifle"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Rifle"), (CComponent**)&m_vVIRifle)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Rifle"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTRifle)))
		return E_FAIL;

	//무기(ShotGun)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_ShotGun"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFShotGun, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_ShotGun"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDShotGun)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_ShotGun"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_ShotGun"), (CComponent**)&m_vVIShotGun)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_ShotGun"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTShotGun)))
		return E_FAIL;

	//무기(Pistol)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFPistol, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Pistol"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Pistol"), (CComponent**)&m_vVIPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Pistol"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTPistol)))
		return E_FAIL;

	//무기(Timber)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Timber"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFTimber, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Timber"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDTimber)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Timber"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Knife"), (CComponent**)&m_vVITimber)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Timber"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTTimber)))
		return E_FAIL;

	//무기(GRENADE)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Grenade"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFGrenade, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Grenade"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDGrenade)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Grenade"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_vVIGrenade)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Grenade"), m_Current_Level, TEXT("Prototype_Component_Texture_Grenade"), (CComponent**)&m_vTTGrenade)))
		return E_FAIL;

	//각 부위별 컴퍼넌트
	CVIBuffer_Cube*		m_CubeVI = nullptr;
	CSubTransform*		m_CubeTF = nullptr;
	CTexture*			m_CubeTT = nullptr;
	CRenderer*			m_CubeRD = nullptr;

	_tchar		szPath1[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath2[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath3[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath4[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };

	for (int i = 0; i < 10; ++i)
	{
		wsprintf(szPath1[i], TEXT("Com_Transform_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath1[i], LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_CubeTF)))
			return E_FAIL;
		m_vTF.push_back(m_CubeTF);

		wsprintf(szPath2[i], TEXT("Com_Renderer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath2[i], LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_CubeRD)))
			return E_FAIL;
		m_vRD.push_back(m_CubeRD);

		wsprintf(szPath3[i], TEXT("Com_VIBuffer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath3[i], LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_CubeVI)))
			return E_FAIL;
		m_vVI.push_back(m_CubeVI);

		wsprintf(szPath4[i], TEXT("Com_CTexture_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_PlayerCube"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}
	Set_Stay();







	return S_OK;
}



void CPlayerCube::SetUp_OnTerrain()
{
	CGameInstance*			pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float RF = m_vTF[4]->Get_State(CSubTransform::STATE_POSITION).y;
	_float LF = m_vTF[5]->Get_State(CSubTransform::STATE_POSITION).y;

	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	CVIBuffer_Terrain*		pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pTerrainBuffer)
		return ;

	CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)
		return ;
	if (State == STATE_ROLL)
		vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y + 1.f;
	else
	{
		if (RF < LF)
		{
			vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - RF + 0.5f;
		}

		else if (RF >= LF)
		{
			vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - LF + 0.5f;
		}

	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);



	Safe_Release(pGameInstance);
	
}

void CPlayerCube::Set_Part(int Part, int Local, _float3 Pos)
{
	_float4x4	MX;
	D3DXMatrixIdentity(&MX);
	_float3  Ri = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_RIGHT);
	_float3  Up = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_UP);
	_float3  Lo = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_LOOK);
	D3DXVec3Normalize(&Ri, &Ri);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Normalize(&Lo, &Lo);
	memcpy(&MX.m[0][0], &Ri, sizeof(_float3));
	memcpy(&MX.m[1][0], &Up, sizeof(_float3));
	memcpy(&MX.m[2][0], &Lo, sizeof(_float3));
	memcpy(&MX.m[3][0], &Pos, sizeof(_float3));
	m_vTF[Part]->Set_WorldMatrix(MX);

}

void CPlayerCube::Set_Part2(CSubTransform* Part, _float3 Pos)
{
	_float4x4	MX;
	D3DXMatrixIdentity(&MX);
	_float3  Ri = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	_float3  Up = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_float3  Lo = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&Ri, &Ri);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Normalize(&Lo, &Lo);
	memcpy(&MX.m[0][0], &Ri, sizeof(_float3));
	memcpy(&MX.m[1][0], &Up, sizeof(_float3));
	memcpy(&MX.m[2][0], &Lo, sizeof(_float3));
	memcpy(&MX.m[3][0], &Pos, sizeof(_float3));
	Part->Set_WorldMatrix(MX);

}

void CPlayerCube::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);
	/*
	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 Look = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 Scal = m_pTransformCom->Get_Scale();
	_float3 Pos1 = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	_float3 Look1 = m_vTF[1]->Get_State(CSubTransform::STATE_LOOK);
	_float3 Scal1 = m_vTF[1]->Get_Scale();
	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	_float3 Scal = m_pTransformCom->Get_Scale();
*/
	//머리
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(0.5f, 0.5f, 0.5f));


	if (HandState == HAND_EMPTY)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_RIFLE)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(50.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_SHOTGUN)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(50.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_PISTOL)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(20.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.7f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_WEAPON)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		//m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//m_vTF[2]->Turn(_float3(0.f, 1.f, 0.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_GRENADE)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);

		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}

	//오른발
	m_vTF[4]->Set_LocalMatrix(i);
	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[4]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[4]->Turn(_float3(0.f, 1.f, 0.f), 0.15f);
	//왼발
	m_vTF[5]->Set_LocalMatrix(i);
	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[5]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[5]->Turn(_float3(0.f, 1.f, 0.f), -0.15f);

	//어꺠
	m_vTF[6]->Set_LocalMatrix(i);
	m_vTF[6]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	m_vTF[7]->Set_LocalMatrix(i);
	m_vTF[7]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	//골반
	m_vTF[8]->Set_LocalMatrix(i);
	m_vTF[8]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	m_vTF[8]->Scaled(_float3(0.65f, 0.18f, 0.3f));

	m_vTF[9]->Scaled(_float3(0.5f, 0.6f, 0.5f));

	m_vTFGrenade->Scaled(_float3(0.6f, 0.6f, 0.6f));
	m_vTFShotGun->Scaled(_float3(2.f, 2.f, 3.f));
	m_vTFShotGun->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	m_vTFRifle->Scaled(_float3(1.f, 1.f, 2.f));
	m_vTFRifle->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));


	m_vTFPistol->Scaled(_float3(1.5f, 1.5f, 2.f));
	m_vTFPistol->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	m_vTFTimber->Scaled(_float3(1.f, 1.f, 5.f));
	//m_vTFTimber->Scaled(_float3(0.8f, 0.8f, 3.5f));
	//m_vTFTimber->Turn(_float3(0.f, 1.f, 0.f), -0.5f);

}

void CPlayerCube::Run(_float fTimeDelta)
{
	//움직이는 속도
	_float3 RH = _float3(-0.004f, 0.03f, 0.03f);
	_float3 LH = _float3(0.004f, 0.03f, 0.03f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.13f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.03f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.03f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);

	//준비
	if (State != STATE_RUN || CurrentHand != HandState || CurrentAngle != AngleState|| AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_RUN;
		CurrentHand = HandState;
		CurrentAngle = AngleState;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;


		if (HandState == HAND_EMPTY)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);
			m_vTF[1]->Go_Straight(3.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);
			//손
			m_vTF[2]->Set_Speed(0.1f);
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

			m_vTF[3]->Set_Speed(0.1f);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
		}
		else if (HandState == HAND_RIFLE)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
			m_vTF[1]->Go_Straight(1.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);


		}
		else if (HandState == HAND_SHOTGUN)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
			m_vTF[1]->Go_Straight(1.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);


		}
		else if (HandState == HAND_PISTOL)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
			m_vTF[1]->Go_Straight(1.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);
		}

		else if (HandState == HAND_WEAPON)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);
			m_vTF[1]->Go_Straight(3.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.9f);
			m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.5f);
			m_vTF[3]->Set_Speed(0.1f);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
		}
		else if (HandState == HAND_GRENADE)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);
			m_vTF[1]->Go_Straight(3.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.9f);
			//m_vTF[2]->Turn(_float3(0.f, 1.f, 0.f), 2.5f);
			m_vTF[3]->Set_Speed(0.1f);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
		}

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.9f);

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);

	}

	_float Time = fTimeDelta* 60.f;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	OnF = OnF  *Time;
	UpF = UpF  *Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation*Time;

	//반복
	if (AnFrame < 15)
	{
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		}
		else if (CurrentHand == HAND_RIFLE)
		{

		}
		else if (CurrentHand == HAND_SHOTGUN)
		{

		}
		else if (CurrentHand == HAND_GRENADE)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 0.5);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		}
		else if (CurrentHand == HAND_WEAPON)
		{

			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		}
		//발
		if (AnFrame < 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.15f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
		}
		if (AnFrame >= 12)
		{
			if (12.f<AnFrame&&12.1f>AnFrame)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
				CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
			}
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
	}

	if (AnFrame >= 15)
	{
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		}
		else if (CurrentHand == HAND_RIFLE)
		{

		}
		else if (CurrentHand == HAND_SHOTGUN)
		{

		}
		else if (CurrentHand == HAND_GRENADE)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 0.5);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		}
		else if (CurrentHand == HAND_WEAPON)
		{

			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		}
		//발
		if (AnFrame < 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.15f*Time);
		}
		if (AnFrame >= 27)
		{
			if (27.f<AnFrame&&27.1f>AnFrame)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
				CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
			}
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f*Time);
	}




	AnFrame+= Time;

	
	key = true;
}

void CPlayerCube::Walk(_float fTimeDelta)
{
	//움직이는 속도
	_float3 RH = _float3(-0.0015f, 0.01f, 0.01f);
	_float3 LH = _float3(0.0015f, 0.01f, 0.01f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.05f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.02f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.02f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);

	//준비
	if (State != STATE_WALK || CurrentHand != HandState || CurrentWalk != WalkState|| AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_WALK;
		CurrentHand = HandState;
		CurrentWalk = WalkState;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;



		if (WalkState == WALK_STRAIGHERIGHT || WalkState == WALK_STRAIGHELEFT)
		{

			if (CurrentHand == HAND_EMPTY)
			{
				//손
				m_vTF[2]->Set_Speed(0.1f);
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

				m_vTF[3]->Set_Speed(0.1f);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

				m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.1f);
			}

			else if (CurrentHand == HAND_RIFLE)
			{

			}
			else if (CurrentHand == HAND_SHOTGUN)
			{

			}
			else if (CurrentHand == HAND_PISTOL)
			{

			}
			else if (CurrentHand == HAND_WEAPON)
			{
				m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.1f);
			}
			else if (CurrentHand == HAND_GRENADE)
			{
				m_vTF[3]->Set_Speed(0.1f);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
			}
			//몸통
			m_vTF[1]->Set_Speed(0.1f);

			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);

			//발
			m_vTF[4]->Set_Speed(0.1f);
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));

			m_vTF[5]->Set_Speed(0.1f);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
		}
		else if (WalkState == WALK_BACKRIGHT || WalkState == WALK_BACKLEFT)
		{
			if (CurrentHand == HAND_EMPTY)
			{
				//손
				m_vTF[2]->Set_Speed(0.1f);
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

				m_vTF[3]->Set_Speed(0.1f);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

			}
			else if (CurrentHand == HAND_RIFLE)
			{
				m_vTF[1]->Go_BackWard(-1.f);
				m_vTF[1]->Go_Right(1.f);
			}
			else if (CurrentHand == HAND_SHOTGUN)
			{
				m_vTF[1]->Go_BackWard(-1.f);
				m_vTF[1]->Go_Right(1.f);
			}
			else if (CurrentHand == HAND_PISTOL)
			{

			}
			else if (CurrentHand == HAND_WEAPON)
			{

			}
			else if (CurrentHand == HAND_GRENADE)
			{
				m_vTF[3]->Set_Speed(0.1f);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
			}

			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Go_BackWard(2.f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.3f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);

			//발
			m_vTF[4]->Set_Speed(0.1f);
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 10) + (UpF * 3));
			//m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);

			m_vTF[5]->Set_Speed(0.1f);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 2) + (DownF * 3) + DownFCompensation);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.3f);
		}
		else if (WalkState == WALK_RIGHT)
		{

			_float3 OnF = _float3(0.05f, 0.0f, 0.0f);
			_float3 UpF = _float3(0.05f, 0.005f, 0.0f);
			_float3 DownF = _float3(0.05f, -0.02f, 0.0f);


			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Go_Right(1.5f);
			m_vTF[1]->Turn(_float3(0.f, 0.f, 1.f), -0.15f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.3f);
			m_vTF[4]->Set_Speed(0.1f);
			m_vTF[5]->Set_Speed(0.1f);
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 10));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 5));
		}
		else if (WalkState == WALK_LEFT)
		{
			_float3 OnF = _float3(0.05f, 0.0f, 0.0f);
			_float3 UpF = _float3(0.05f, 0.005f, 0.0f);
			_float3 DownF = _float3(0.05f, -0.02f, 0.0f);


			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Go_Left(1.5f);
			m_vTF[1]->Turn(_float3(0.f, 0.f, 1.f), 0.15f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.3f);
			m_vTF[4]->Set_Speed(0.1f);
			m_vTF[5]->Set_Speed(0.1f);
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 5));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 10));
		}


	}
	_float Time = fTimeDelta * 60;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	OnF = OnF *Time;
	UpF = UpF *Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation *Time;

	//반복
	if (WalkState == WALK_STRAIGHERIGHT || WalkState == WALK_STRAIGHELEFT || WalkState == WALK_STRAIGHE|| AnFrame >= 30)
	{
		if (AnFrame < 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}

			else if (CurrentHand == HAND_RIFLE)
			{

			}
			else if (CurrentHand == HAND_SHOTGUN)
			{

			}
			else if (CurrentHand == HAND_PISTOL)
			{

			}
			else if (CurrentHand == HAND_WEAPON)
			{

			}
			else if (CurrentHand == HAND_GRENADE)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}
			//발
			if (AnFrame < 12)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			}
			if (AnFrame >= 12)
			{
				if (12.f<AnFrame&&12.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);

			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
		}

		if (AnFrame >= 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
			}
			else if (CurrentHand == HAND_RIFLE)
			{

			}
			else if (CurrentHand == HAND_SHOTGUN)
			{

			}
			else if (CurrentHand == HAND_PISTOL)
			{

			}
			else if (CurrentHand == HAND_WEAPON)
			{

			}
			else if (CurrentHand == HAND_GRENADE)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
			}
			//발
			if (AnFrame < 27)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			}
			if (AnFrame >= 27)
			{
				if (27.f<AnFrame&&27.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f*Time);
		}
	}
	else if (WalkState == WALK_BACKRIGHT || WalkState == WALK_BACKLEFT)
	{
		if (AnFrame < 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}
			//발
			if (AnFrame < 3)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			}
			if (AnFrame >= 3)
			{
				if (3.f<AnFrame&&3.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.03f*Time);
		}

		if (AnFrame >= 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);

			}
			//발
			if (AnFrame < 18)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			}
			if (AnFrame >= 18)
			{
				if (18.f<AnFrame&&18.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.03f*Time);
		}
	}
	else if (WalkState == WALK_RIGHT)
	{
		_float3 OnF = _float3(0.05f, 0.0f, 0.0f)*Time;
		_float3 UpF = _float3(0.05f, 0.005f, 0.0f)*Time;
		_float3 DownF = _float3(0.05f, -0.02f, 0.0f)*Time;

		if (AnFrame < 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}
			//발
			if (AnFrame < 12)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			}
			if (AnFrame >= 12)
			{
				if (12.f<AnFrame&&12.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));


			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.03f*Time);
		}

		if (AnFrame >= 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);

			}
			//발
			if (AnFrame < 27)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			}
			if (AnFrame >= 27)
			{
				if (27.f<AnFrame&&27.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));

			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.03f*Time);
		}
	}
	else if (WalkState == WALK_LEFT)
	{
		_float3 OnF = _float3(0.05f, 0.0f, 0.0f)*Time;
		_float3 UpF = _float3(-0.05f, 0.005f, 0.0f)*Time;
		_float3 DownF = _float3(-0.05f, -0.02f, 0.0f)*Time;
		if (AnFrame < 15)
		{
			//손
			if (HandState == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}
			//발
			if (AnFrame < 12)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			}
			if (AnFrame >= 12)
			{
				if (12.f<AnFrame&&12.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));


			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.03f*Time);
		}

		if (AnFrame >= 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
			}
			//발
			if (AnFrame < 27)
			{
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			}
			if (AnFrame >= 27)
			{
				if (27.f<AnFrame&&27.1f>AnFrame)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WALK);
					CSoundMgr::Get_Instance()->PlaySound(L"Walk2.wav", CSoundMgr::PLAYER_WALK, 0.6f);
				}
				m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
				m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));

			}
			//몸통
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.03f*Time);
		}
	}




	AnFrame += Time;

	
	key = true;
}

void CPlayerCube::Rolling(_float fTimeDelta)
{

	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_ROLL)
	{
		Set_Stay();
		State = STATE_ROLL;
		m_pTransformCom->Look(vAngle[AngleState]);
		AnFrame = 0;
	}

	_float	Time = fTimeDelta * 60.f;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	LF = LF *Time;

	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
		m_vTF[1]->Go_Straight(0.05f*Time);
	}
	else if (AnFrame >= 20)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);
	}



	AnFrame += fTimeDelta * 60;
	key = true;

	if (AnFrame >= 30)
	{
		AnFrame = 0;
		State = STATE_STAY;

	}
	else
	{
		_float3 Z = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		Z += (vAngle[AngleState] * Time) / 3;
		_float3 RollAngle;

		switch (AngleState)
		{
		case Client::CPlayerCube::ANGLE_STRAIGHE:
			RollAngle = vAngle[2];
			break;
		case Client::CPlayerCube::ANGLE_BACK:
			RollAngle = vAngle[3];
			break;
		case Client::CPlayerCube::ANGLE_RIGHT:
			RollAngle = vAngle[1];
			break;
		case Client::CPlayerCube::ANGLE_LEFT:
			RollAngle = vAngle[0];
			break;
		case Client::CPlayerCube::ANGLE_STRAIGHERIGHT:
			RollAngle = vAngle[6];
			break;
		case Client::CPlayerCube::ANGLE_STRAIGHELEFT:
			RollAngle = vAngle[4];
			break;
		case Client::CPlayerCube::ANGLE_BACKRIGHT:
			RollAngle = vAngle[7];
			break;
		case Client::CPlayerCube::ANGLE_BACKLEFT:
			RollAngle = vAngle[5];
			break;
		case Client::CPlayerCube::ANGLE_END:
			RollAngle = vAngle[1];
			break;
		default:
			break;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Z);
		m_pTransformCom->Turn(RollAngle, D3DXToRadian(12.0f*Time));
	}



}





void CPlayerCube::Falling(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	if (State != STATE_FALLING)
	{
		Set_Stay();
		State = STATE_FALLING;
		AnFrame = 0;

	}
	_float Time = fTimeDelta*60.f;
	RH = RH * Time;
	LH = LH* Time;
	RF = RF* Time;
	LF = LF* Time;
	DownAngle = DownAngle*Time;


	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
		m_vTF[1]->Go_Straight(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON_CHANGE);
	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Player_Down.wav", CSoundMgr::WEAPON_CHANGE, 0.5f);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_Die.wav", CSoundMgr::PLAYER_SHOT, 0.5f);
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
	}
	else if (AnFrame >= 30 && AnFrame < 40)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.02f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f*Time);
		m_vTF[1]->Go_Straight(0.0125f*Time);
		m_pTransformCom->Go_Y(0.0025f*Time);
	}
	else if (AnFrame >= 40 && AnFrame < 50)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.02f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.0125f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.0125f*Time);
		m_vTF[1]->Go_Straight(-0.0125f*Time);
		m_pTransformCom->Go_Y(-0.0025f*Time);
	}


	AnFrame += Time;
	key = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_DOWN;

	}
	else
	{

	}


}


void CPlayerCube::Down(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_DOWN)
	{
		Set_Stay();
		State = STATE_DOWN;
		AnFrame = 0;
	}

	if (AnFrame < 5)
	{
		CGameInstance* gameins = CGameInstance::GetInstance();
		Safe_AddRef(gameins);
		if (isDied == false) {
			gameins->Add_GameObjectToLayer(m_Current_Level, L"Died", L"Prototype_GameObject_Died");
			isDied = true;
		}
		Safe_Release(gameins);

	}

	_float Time = fTimeDelta * 60;
	AnFrame*Time;
	key = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0.f;
		State = STATE_UP;

	}
}

void CPlayerCube::Up(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	if (State != STATE_UP)
	{
		Set_Stay();
		State = STATE_UP;
		AnFrame = 0;

	}

	_float Time = fTimeDelta * 60;
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;
	DownAngle = DownAngle*Time;

	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
		m_vTF[1]->Go_Straight(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));

	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{

		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));
	}


	AnFrame += Time;
	key = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		Set_Stay();
		State = STATE_STAY;

	}
	else
	{

	}


}



void CPlayerCube::Stay(_float fTimeDelta)
{

	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_STAY)
	{
		Set_Stay();
		State = STATE_STAY;
		StandardAnTime = 0.f;
		AnTime = 0.f;

		D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), -0.5f);
		D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
		Loc = m_vTF[0]->Get_LocalMatrix();
		End = Loc * Rot * Far;
		m_vTF[0]->Set_LocalMatrix(End);
	}
	StandardAnTime += fTimeDelta;

	if (AnTime + 3.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 1.5f > StandardAnTime)
	{
		Ang = 0.03f;

	}
	else
		Ang = -0.03f;
	D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), Ang / 5);
	D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
	Loc = m_vTF[0]->Get_LocalMatrix();
	End = Loc * Rot * Far;
	m_vTF[0]->Set_LocalMatrix(End);

	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);


}

HRESULT CPlayerCube::Throw(_float fTimeDelta)
{

	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	HAND SubHandState = HandState;
	if (State != STATE_THROW)
	{
		Set_Stay();
		m_isChase = m_pTransformCom->LookAt(m_vTargetPos);
		State = STATE_THROW;
		AnFrame = 0;
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f);
		m_vTF[4]->Go_Z(-3.f);
		Grenade = true;
	}
	_float Time = fTimeDelta * 60;
	RH = RH*Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;


	if (AnFrame < 20)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.04f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.04F*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

	}



	else if (AnFrame >= 20 && AnFrame < 35)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF*1.7);
		m_vTF[4]->Go_Z(0.01f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.12f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.12f*Time);
		m_vTF[1]->Go_Straight(0.4f*Time);

		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 0.08f*Time);
	}
	if (AnFrame >= 35&& Grenade==true)
	{
		//수류탄 생성
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		TargetPos.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



		TargetPos.target = m_vTargetPos;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Grenade"), &TargetPos)))
			return E_FAIL;

		Safe_Release(pGameInstance);
		Grenade = false;
	}
	else if (AnFrame >= 30 && AnFrame < 45)
	{
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 5);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 5);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.014f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 5);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 5);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.012f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.008f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[1]->Go_BackWard(0.025f*Time);
	}
	AnFrame += Time;

	if (AnFrame >= 50)
	{
		AnFrame = 0.f;
		State = STATE_STAY;
		HandState = SubHandState;
		Set_Stay();
	}

	key = true;

	return S_OK;
}

void CPlayerCube::Attack(_float fTimeDelta)
{

	_float3 RH = _float3(-0.05f, 0.03f, 0.05f);
	_float3 LH = _float3(0.02f, 0.05, 0.01f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	HAND SubHandState = HandState;
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		m_isChase = m_pTransformCom->LookAt(m_vTargetPos);
		State = STATE_ATTACK;
		AnFrame = 0;
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f);
		m_vTF[4]->Go_Z(-3.f);
		AttackOn = true;
	}

	_float Time = fTimeDelta * 80;
	RH = RH*Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;

	if (AnFrame < 20)
	{
		//머리

		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.07f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.06f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.04f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

	}



	else if (AnFrame >= 20 && AnFrame < 30)
	{
		//머리
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.001f*Time);
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 1.8f * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 0.5f * RH);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.15f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF * 3);
		m_vTF[4]->Go_Z(0.01f*Time);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - 1.8 * LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.14f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.1f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.17f*Time);
		m_vTF[1]->Go_Straight(0.3f*Time);

		m_vTF[3]->Set_StateWorld((CSubTransform::STATE_POSITION), m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));


	}
	else if (AnFrame >= 30 && AnFrame < 45)
	{
		if (AttackOn)
		{
			CGameInstance*		pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);
			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Type = 7;
			m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 5.f;
			StatusmDesc.fMaxStamina = 1200.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 10.f;
			StatusmDesc.fPower = 0.2f;
			StatusmDesc.frange = 3.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK)*2;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;
			m_pTransformCom->Go_BackWard(0.01f);

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);

			Safe_Release(pGameInstance);
			AttackOn = false;
		}
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 5);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 5);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.014f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 5);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 5);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.012f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.008f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[1]->Go_BackWard(0.025f*Time);
	}

	AnFrame += Time;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_STAY;
		HandState = SubHandState;
		Set_Stay();
	}

	key = true;


}

CPlayerCube * CPlayerCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerCube*	pInstance = new CPlayerCube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayerCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerCube::Clone(void * pArg)
{
	CPlayerCube*	pInstance = new CPlayerCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayerCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCube::Free()
{
	__super::Free();

	

	for (auto& pGameObject : m_vVI)
		Safe_Release(pGameObject);

	m_vVI.clear();

	for (auto& pGameObject : m_vTF)
		Safe_Release(pGameObject);

	m_vTF.clear();

	for (auto& pGameObject : m_vTT)
		Safe_Release(pGameObject);

	m_vTT.clear();

	for (auto& pGameObject : m_vRD)
		Safe_Release(pGameObject);

	m_vRD.clear();
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);


	Safe_Release(m_vTFRifle);
	Safe_Release(m_vVIRifle);
	Safe_Release(m_vTTRifle);
	Safe_Release(m_vRDRifle);

	Safe_Release(m_vTFShotGun);
	Safe_Release(m_vVIShotGun);
	Safe_Release(m_vTTShotGun);
	Safe_Release(m_vRDShotGun);

	Safe_Release(m_vTFPistol);
	Safe_Release(m_vVIPistol);
	Safe_Release(m_vTTPistol);
	Safe_Release(m_vRDPistol);

	Safe_Release(m_vTFTimber);
	Safe_Release(m_vVITimber);
	Safe_Release(m_vTTTimber);
	Safe_Release(m_vRDTimber);

	Safe_Release(m_vTFGrenade);
	Safe_Release(m_vVIGrenade);
	Safe_Release(m_vTTGrenade);
	Safe_Release(m_vRDGrenade);



}

