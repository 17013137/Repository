#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Logo.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "Client_ObjHeaders.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;



	GameOBJ_Prototype_Load();

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CSoundMgr::Get_Instance()->PlaySound(L"Opening.mp3", CSoundMgr::BGM1, 0.3f);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER))))
			return;
		//if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS))))
			//return;
	}

	Safe_Release(pGameInstance);
		
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Logo_Sky"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Logo_Rect"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 0.f, -1.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 1.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Camera_Logo"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::GameOBJ_Prototype_Load()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));

	/*         플레이어        */

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerCube"),
		CPlayerCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*         NPC        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NPC_Shop"),
		CNPC_Shop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NPC_Grandfa"),
		CNPC_Grandfa::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grandfa_Dummy"),
		CGrandfa_Dummy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NPC_Dummy"),
		CNPC_Dummy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NPC_Steve"),
		CSteve_Dummy::Create(m_pGraphic_Device))))
		return E_FAIL;


	/*         몬스터        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NormalZombie"),
		CNormalZombie::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ZombieSpitter"),
		CZombieSpitter::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fanatic"),
		CFanatic::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skeleton"),
		CSkeleton::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Blue_Skeleton"),
		CBlue_Skeleton::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Granpa_Skeleton"),
		CGranpa_Skeleton::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wither_skeleton"),
		CWither_skeleton::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wither_skeleton2"),
		CWither_skeleton2::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GrandfaDummy"),
		CGrandfa_Dummy::Create(m_pGraphic_Device))))
		return E_FAIL;



	/*         보스        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Lucifer"),
		CBoss_Lucifer::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Meteor"),
		CMeteor::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Missile"),
		CMissile::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/*         바닥        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*         카메라        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Shelter"),
		CCamera_Shelter::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*         하늘        */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* 잡 지형 오브젝*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tile"),
		CTile::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pGraphic_Device))))

		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"),
		CDoor::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cone"),
		CCone::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wood"),
		CWood::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GateCube"),
		CGateCube::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PurpleGate"),
		CPurpleGate::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Deco"),
		CDeco::Create(m_pGraphic_Device))))
		return E_FAIL;



	/*         이펙트 오브젝트       */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Explosion"),
		CExplosion::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire"),
		CFire::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SoulFire"),
		CSoulFire::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Action"),
		CEffect_Action::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Loop"),
		CEffect_Loop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grenade"),
		CGrenade::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HitCube"),
		CHitCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HitCircle"),
		CHitCircle::Create(m_pGraphic_Device))))

		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BulletEffect"),
		CEffect_Loop::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Circle"),
		CKelp::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Blood_Effect"),
		CBlood_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Blood_Effect"),
		CBoss_Blood_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Piece_Effect"),
		CPiece_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CircleTarget"),
		CCircleTarget::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weather"),
		CWeather::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MagicCircle"),
		CMagicCircle::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma region UI오브젝트
	/*         UI       */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_State_UI"),
		CState_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HP_UI"),
		CHP_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stamina_UI"),
		CStamina_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pistol_UI"),
		CPistol_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rifle_UI"),
		CRifle_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShotGun_UI"),
		CShotGun_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Timber_UI"),
		CTimber_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Inventory_UI"),
		CInventory_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Equip_UI"),
		CEquip_UI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shop_UI"),
		CShop_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse_UI"),
		CMouse_UI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Granade_Buy"),
		CGranade_Buy::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PainKiller_Buy"),
		CPainKiller_Buy::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PistolAmmo_Buy"),
		CPistolAmmo_Buy::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RifleAmmo_Buy"),
		CRifleAmmo_Buy::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShotGunAmmo_Buy"),
		CShotGunAmmo_Buy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse_UI"),
		CMouse_UI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map"),
		MapBase::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_HP_UI"),
		CBoss_HP_UI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_State_UI"),
		CBoss_State_UI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Credit"),
		CCredit::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Died"),
		CYouDied::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Scene_UI"),
		CScene_UI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Scene_UI2"),
		CScene_UI2::Create(m_pGraphic_Device))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}


CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();


}
