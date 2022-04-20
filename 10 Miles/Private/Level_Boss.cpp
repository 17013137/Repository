#include "stdafx.h"
#include "..\Public\Level_Boss.h"

#include "GameInstance.h"
#include "Camera_Default.h"
#include "Level_Loading.h"
#include "EventMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"


CLevel_Boss::CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Boss::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM1);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM2);

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);

	m_LevelIndex = gameins->Get_LevelIndex();

	Safe_Release(gameins);

	CSoundMgr::Get_Instance()->PlaySound(L"Boss1.wav", CSoundMgr::BGM2, 1.f);

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Monster"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	Load_Cube_Data(L"../../Data/Boss/Cube.dat", LEVEL_BOSS);
	m_pGraphic_Device->LightEnable(0, FALSE);
	m_pGraphic_Device->LightEnable(2, FALSE);
	m_pGraphic_Device->LightEnable(3, FALSE);
	m_pGraphic_Device->LightEnable(4, FALSE);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 0.7f;
	light.Diffuse.b = 0.7f;
	light.Diffuse.a = 0.f;

	light.Direction = _float3(0.f, -1.f, 0.f);
	m_pGraphic_Device->SetLight(3, &light);
	m_pGraphic_Device->LightEnable(3, TRUE);
	CEventMgr::Get_Instance()->Set_OnAir();

	for (int i = 5; i < 15; ++i)
	{
		m_pGraphic_Device->LightEnable(i, FALSE);
	}

	CEventMgr::Get_Instance()->Set_OnAir();
	return S_OK;
}

void CLevel_Boss::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
		
	
	
	CGameInstance* _gameins = CGameInstance::GetInstance();
	Safe_AddRef(_gameins);
	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		if (FAILED(_gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
			return;
	}

	_gameins->Collison_Knockback(m_LevelIndex, L"Layer_HitBox", L"Monster");
	_gameins->Collison_Knockback(m_LevelIndex, L"Layer_PlayerHitBox", L"Layer_Player");
	_gameins->Collison_Sheare(m_LevelIndex, L"Monster", L"Monster");
	_gameins->SuperCollison_Rect(m_LevelIndex, L"Monster", BoxsPos); 
	_gameins->SuperCollison_Rect(m_LevelIndex, L"Layer_Player", BoxsPos);
	
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

	Safe_Release(_gameins);
}

HRESULT CLevel_Boss::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("º¸½º"));

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _tchar * pLayerTag)
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


	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_PlayerCube"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Dummy", TEXT("Prototype_GameObject_NPC_Dummy"))))
		return E_FAIL;
	CTransform* trans = (CTransform*)pGameInstance->Get_Component(m_LevelIndex, L"Layer_Dummy", L"Com_Transform");
	trans->LookAt(_float3(45.f, 2.f, 40.f));
	trans->Set_State(CTransform::STATE_POSITION, _float3(20.f, 0.6f, 25.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_int _num = 20;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Terrain"), &_num)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_State_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_HP_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Stamina_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Pistol_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Rifle_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_ShotGun_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Timber_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Inventory_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Equip_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Mouse_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Boss_HP_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Boss_State_UI"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Credit"))))
		return E_FAIL;

	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_HitBox(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Boss_Lucifer"))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Blue_Skeleton"),_float3(10.f,1.f,10.f))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Granpa_Skeleton"), _float3(10.f, 1.f, 10.f))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Fanatic"), &_float3(10.f, 0.1f, 10.f))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Cone"), _float3(20.f, 1.f, 20.f))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Wither_skeleton2"), _float3(12.f, 1.f, 12.f))))
	//	return E_FAIL;
	

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Boss * CLevel_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Boss*	pInstance = new CLevel_Boss(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss::Free()
{
	__super::Free();

}
