#include "stdafx.h"
#include "..\Public\Level_Shelter.h"

#include "GameInstance.h"
#include "Camera_Shelter.h"

#include "SoundMgr.h"
#include "KeyMgr.h"

#include "Level_Loading.h"

CLevel_Shelter::CLevel_Shelter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Shelter::NativeConstruct()
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

	if (FAILED(Ready_Layer_Monster(TEXT("Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NPC(TEXT("Layer_Grandfa"))))
		return E_FAIL;

	if (FAILED(gameins->Add_GameObjectToLayer(LEVEL_STATIC, L"Map", TEXT("Prototype_GameObject_Map"))))
		return E_FAIL;


	Load_Data(L"../../Data/Shelter/Object.dat", LEVEL_SHELTER);



	m_pGraphic_Device->LightEnable(0, FALSE);
	m_pGraphic_Device->LightEnable(2, FALSE);
	m_pGraphic_Device->LightEnable(3, FALSE);
	Safe_Release(gameins);

	CEventMgr::Get_Instance()->Set_OnAir();
	CEventMgr::Get_Instance()->Add_EventCnt();
	return S_OK;
}

void CLevel_Shelter::Tick(_float fTimeDelta)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Shelter_bgm.wav", CSoundMgr::BGM1, 0.3f);
	__super::Tick(fTimeDelta);
	CGameInstance* _gameins = CGameInstance::GetInstance();
	Safe_AddRef(_gameins);

	_float RandLight = 2 + rand() % 50 / 50.f;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 0.2f;
	light.Diffuse.b = 0.2f;
	light.Diffuse.a = 0.f;
	light.Attenuation0 = RandLight;
	light.Range = 20.f;

	light.Position = _float3(35.5f, 2.0f, 29.5f);
	m_pGraphic_Device->SetLight(4, &light);
	m_pGraphic_Device->LightEnable(4, TRUE);
	_gameins->Collison_Sheare(m_LevelIndex, L"Layer_Player", L"Object");
	_gameins->Collison_Knockback(m_LevelIndex, L"Layer_HitBox", L"Monster");
	if (_gameins->Collison_Sheare(m_LevelIndex, L"Layer_Player", L"Gate"))
		int a = 10;


	//if (SpawDelay<0)
	//RandomSpawn();
	//SpawDelay -= fTimeDelta;
	Safe_Release(_gameins);
}

HRESULT CLevel_Shelter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("½°ÅÍ"));

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(35.f, 7.f, 20.f);
	CameraDesc.vAt = _float3(45.f, -7.f, 45.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 100.0f;
	CameraDesc.fFovy = D3DXToRadian(30.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = 5.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Camera_Shelter"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_PlayerCube"))))
		return E_FAIL;

	CTransform* trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, TEXT("Layer_Player"), TEXT("Com_Transform"));
	trans->Set_State(CTransform::STATE_POSITION, _float3(38.5f, 0.6f, 32.f));
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_uint num = 61;
	_float3 _pos = { 36.f, 1.5f, 30.f };


	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Terrain"), &num)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Fire"), &_pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Fire"), &_float3(_pos.x - 0.3f, _pos.y, _pos.z - 0.2f))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Fire"), &_float3(_pos.x, _pos.y, _pos.z))))
	//	return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _tchar * pLayerTag)
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
	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Scene_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_Scene_UI2"))))
		return E_FAIL;





	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Shelter::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, pLayerTag, TEXT("Prototype_GameObject_NPC_Grandfa"))))
		return E_FAIL;

	CTransform* trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, pLayerTag, TEXT("Com_Transform"));
	trans->Set_State(CTransform::STATE_POSITION, _float3(33.f, 0.6f, 34.f));

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Grandfa_Dummy", TEXT("Prototype_GameObject_Grandfa_Dummy"))))
		return E_FAIL;

	CTransform* G_trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Grandfa_Dummy", TEXT("Com_Transform"));
	G_trans->Set_State(CTransform::STATE_POSITION, _float3(33.f, 0.6f, 34.f));

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Dummy", TEXT("Prototype_GameObject_NPC_Dummy"))))
		return E_FAIL;

	CTransform* D_trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Layer_Dummy", TEXT("Com_Transform"));
	D_trans->Set_State(CTransform::STATE_POSITION, _float3(30.5f, 0.6f, 32.f));
	D_trans->LookAt(_float3(36.f, 1.5f, 30.f));

	if (FAILED(pGameInstance->Add_GameObjectToLayer(m_LevelIndex, L"Layer_Steve", TEXT("Prototype_GameObject_NPC_Steve"))))
		return E_FAIL;
	
	CTransform* DD_trans = (CTransform*)pGameInstance->GetInstance()->Get_Component(m_LevelIndex, L"Layer_Steve", TEXT("Com_Transform"));
	DD_trans->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.6f, 33.f));
	DD_trans->LookAt(_float3(36.f, 1.5f, 30.f));

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CLevel_Shelter::RandomSpawn()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_float3 SpawnPos;
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_SHELTER, TEXT("Layer_Player"), TEXT("Com_Transform"));

	SpawnPos = _float3{ rand() % 30 - 15.f,5.f,rand() % 30 - 15.f };
	//_float3 Look = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	//_float3 Up = pTargetTransform->Get_State(CTransform::STATE_UP);


	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vPOS = *(_float3*)&ViewMatrix.m[3][0] + *(_float3*)&ViewMatrix.m[2][0] * 22;




	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawnPos = _float3{ rand() % 200 / 10 - 10.f,5.f,rand() % 200 / 10 - 10.f };
	SpawnPos = vPOS + SpawnPos;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_SHELTER, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Weather"), &SpawnPos)))
		return E_FAIL;
	SpawDelay = 0.001f;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Shelter::Ready_Layer_HitBox(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Shelter::Ready_Layer_Monster(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_Shelter * CLevel_Shelter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Shelter*	pInstance = new CLevel_Shelter(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Shelter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Shelter::Free()
{
	__super::Free();

}
