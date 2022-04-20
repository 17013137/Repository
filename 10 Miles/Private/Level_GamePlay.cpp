#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Default.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"

#include "Level_Loading.h"
CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM1);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM2);

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_HitBox(TEXT("Layer_HitBox"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	Load_Data(L"../../Data/GamePlay/Object.dat", LEVEL_GAMEPLAY);
	//Load_Tile_Data(L"../../Data/GamePlay/Tile.dat", LEVEL_GAMEPLAY);
	Load_Cube_Data(L"../../Data/GamePlay/Cube.dat", LEVEL_GAMEPLAY);
	m_pGraphic_Device->LightEnable(0, TRUE);
	m_pGraphic_Device->LightEnable(2, TRUE);
	m_pGraphic_Device->LightEnable(3, FALSE);
	m_pGraphic_Device->LightEnable(4, FALSE);

	CEventMgr::Get_Instance()->Set_OnAir();

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	CSoundMgr::Get_Instance()->PlaySound(L"bgm_3Low.wav", CSoundMgr::BGM1, 0.3f);
	CGameInstance* _gameins = CGameInstance::GetInstance();

	Safe_AddRef(_gameins);
	//충돌검사할것들
	_gameins->Collison_Sheare(LEVEL_GAMEPLAY, L"Layer_Player", L"Monster");
	_gameins->Collison_Sheare(LEVEL_GAMEPLAY, L"Monster", L"Monster");
	_gameins->Collison_Sheare(LEVEL_GAMEPLAY, L"Monster", L"Layer_Player");

	_gameins->Collison_Knockback(LEVEL_GAMEPLAY, L"Layer_HitBox", L"Monster");
	_gameins->Collison_Knockback(LEVEL_GAMEPLAY, L"Layer_PlayerHitBox", L"Layer_Player");
	_gameins->SuperCollison_Rect(LEVEL_GAMEPLAY, L"Layer_Player", BoxsPos);
	_gameins->SuperCollison_Rect(LEVEL_GAMEPLAY, L"Monster", BoxsPos);


	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CEventMgr::Get_Instance()->Add_MaxLevel();
		if (FAILED(_gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
			return;
	}

	if (m_MonsterClear == true) {
		CStatus*_GateStatus = (CStatus*)_gameins->Get_Component(m_LevelIndex, L"Gate", L"Com_Status");
		_GateStatus->Set_Damage(CStatus::ATTACK_DEFAULT, 101.f);
		CTransform* _GateTrans = (CTransform*)_gameins->Get_Component(m_LevelIndex, L"Gate", L"Com_Transform");
		_float3 _GatePos = _GateTrans->Get_State(CTransform::STATE_POSITION);
		CTransform* _PlayerTrans = (CTransform*)_gameins->Get_Component(m_LevelIndex, L"Layer_Player", L"Com_Transform");
		_float3 _PlayerPos = _PlayerTrans->Get_State(CTransform::STATE_POSITION);

		if (_PlayerPos == nullptr || _GatePos == nullptr)
			return;

		_float dist = D3DXVec3Length(&(_GatePos - _PlayerPos));
		if (dist < 3.f)
			if (CKeyMgr::Get_Instance()->Key_Down('T')) {
				CEventMgr::Get_Instance()->Add_MaxLevel();
				if (FAILED(_gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
					return;
			}
	}

	CStatus* PlayerState = (CStatus*)_gameins->Get_Component(m_LevelIndex, L"Layer_Player", L"Com_Status");


	if (PlayerState == nullptr) {
		Safe_Release(_gameins);
		return;
	}

	if (PlayerState->Get_State() == 2) {
		if (CKeyMgr::Get_Instance()->Key_Down('T')) {
			if (FAILED(_gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
				return;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F10) ) //BGMVolumeDown
		CSoundMgr::Get_Instance()->BGMVolumeDown(0.1f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F11)) //BGMVolumeUp
		CSoundMgr::Get_Instance()->BGMVolumeUp(0.1f);


	Safe_Release(_gameins);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PlayerCube"))))
		return E_FAIL;
	CTransform* trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, TEXT("Layer_Player"), TEXT("Com_Transform"));
	trans->Set_State(CTransform::STATE_POSITION, _float3(40.f, 0.6f, 20.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_HitBox(const _tchar * pLayerTag)
{

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	
//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ZombieSpitter"))))
//		return E_FAIL;

	

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_State_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HP_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Stamina_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Pistol_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Rifle_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ShotGun_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Timber_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Inventory_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Equip_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Mouse_UI"))))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}
