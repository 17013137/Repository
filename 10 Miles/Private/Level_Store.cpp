#include "stdafx.h"
#include "..\Public\Level_Store.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"

#include "Level_Loading.h"

CLevel_Store::CLevel_Store(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Store::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);

	m_LevelIndex = gameins->Get_LevelIndex();

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Shop(TEXT("Layer_Shop"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Mouse(TEXT("Layer_Mouse"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	Load_Data(L"../../Data/Store/Object.dat", LEVEL_STORE);

	CSoundMgr::Get_Instance()->PlaySound(L"Shop1.wav", CSoundMgr::BGM1, 0.2f);
	m_pGraphic_Device->LightEnable(0, FALSE);
	m_pGraphic_Device->LightEnable(2, FALSE);


	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.3f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 1.f;
	light.Diffuse.a = 0.f;

	light.Direction = _float3(0.f, -1.f, 0.f);
	m_pGraphic_Device->SetLight(3, &light);
	m_pGraphic_Device->LightEnable(3, TRUE);

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse.r = 0.5f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 1.f;
	light.Diffuse.a = 0.f;
	light.Attenuation0 = 1.f;
	light.Range = 5.f;

	light.Position = _float3(39.f, 0.5f, 39.f);
	m_pGraphic_Device->SetLight(4, &light);
	m_pGraphic_Device->LightEnable(4, TRUE);

	CEventMgr::Get_Instance()->Set_OnAir();
	CEventMgr::Get_Instance()->Add_EventCnt();
	Safe_Release(gameins);
	return S_OK;
}

void CLevel_Store::Tick(_float fTimeDelta)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Shop2.wav", CSoundMgr::BGM1, 0.1f);
	__super::Tick(fTimeDelta);
	CGameInstance* _gameins = CGameInstance::GetInstance();
	Safe_AddRef(_gameins);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)) {
		if (FAILED(_gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
			return;
		isRain = false;
	}

	//if (SpawDelay<0)
	if (isRain == true)
		RandomSpawn();
	//SpawDelay -= fTimeDelta;


	Safe_Release(_gameins);
}

HRESULT CLevel_Store::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("»óÁ¡"));

	return S_OK;
}
HRESULT CLevel_Store::RandomSpawn()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_float3 SpawnPos;
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STORE, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	SpawnPos = _float3{ rand() % 30 - 15.f,5.f,rand() % 30 - 15.f };
	//_float3 Look = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	//_float3 Up = pTargetTransform->Get_State(CTransform::STATE_UP);


	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vPOS = *(_float3*)&ViewMatrix.m[3][0] + *(_float3*)&ViewMatrix.m[2][0] * 12;




	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,10.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawDelay = 0.001f;
	Safe_Release(pGameInstance);
	return S_OK;
}
HRESULT CLevel_Store::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(40.f, 1.2f, 37.0f);
	CameraDesc.vAt = _float3(40.f, 1.5f, 40.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;



	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_PlayerCube"))))
		return E_FAIL;

	CTransform* trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, TEXT("Layer_Player"), TEXT("Com_Transform"));
	trans->Set_State(CTransform::STATE_POSITION, _float3(38.5f, 0.f, 32.f));
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_uint num = 0;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Terrain"), &num)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_UI(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Granade_Buy"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_PainKiller_Buy"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_PistolAmmo_Buy"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_RifleAmmo_Buy"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_ShotGunAmmo_Buy"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Scene_UI"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Scene_UI2"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_NPC_Shop"))))
		return E_FAIL;

	CTransform* trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, TEXT("Layer_NPC"), TEXT("Com_Transform"));
	trans->Set_State(CTransform::STATE_POSITION, _float3(40.f, 0.5f, 40.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Dummy", TEXT("Prototype_GameObject_NPC_Dummy"))))
		return E_FAIL;

	CTransform* D_trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Layer_Dummy", TEXT("Com_Transform"));
	D_trans->Set_State(CTransform::STATE_POSITION, _float3(40.f, 0.6f, 35.f));
	D_trans->LookAt(_float3(40.f, 1.5f, 40.f));

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Grandfa_Dummy", TEXT("Prototype_GameObject_GrandfaDummy"))))
		return E_FAIL;

	CTransform* D_trans2 = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Layer_Grandfa_Dummy", TEXT("Com_Transform"));
	D_trans2->Set_State(CTransform::STATE_POSITION, _float3(39.f, 0.6f, 34.f));
	D_trans2->LookAt(_float3(40.f, 1.5f, 40.f));

	if (CEventMgr::Get_Instance()->Get_EventCnt() > 4) {
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Steve", TEXT("Prototype_GameObject_NPC_Steve"))))
			return E_FAIL;
		CTransform* DD_trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Layer_Steve", TEXT("Com_Transform"));
		DD_trans->Set_State(CTransform::STATE_POSITION, _float3(41.f, 0.6f, 32.f));
		DD_trans->LookAt(_float3(40.f, 1.5f, 38.f));

	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_Shop(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Shop_UI"))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_Mouse(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Mouse_UI"))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_HitBox(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Store::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_Store * CLevel_Store::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Store*	pInstance = new CLevel_Store(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Store"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Store::Free()
{
	__super::Free();

}
