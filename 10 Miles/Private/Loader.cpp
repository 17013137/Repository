#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Client_ObjHeaders.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_SHELTER:
		if (FAILED(pLoader->Loading_ForShelterLevel()))
			MSG_BOX(L"Loader->SHelterLevel 실패!");
		break;
	case LEVEL_STORE:
		if (FAILED(pLoader->Loading_ForStoreLevel()))
			MSG_BOX(L"Loader->Loading_StoreLevel 실패!");
		break;
	case LEVEL_GAMEPLAY:
		if (FAILED(pLoader->Loading_ForGamePlayLevel()))
			MSG_BOX(L"Loader->Loading_GamePlayLevel 실패!");
		break;
	case LEVEL_FOREST:
		if (FAILED(pLoader->Loading_ForForestLevel()))
			MSG_BOX(L"Loader->Loading_ForestLevel 실패!");
		break;
	case LEVEL_CAVE:
		if (FAILED(pLoader->Loading_ForCave()))
			MSG_BOX(L"Loader->Loading_Cave 실패!");
		break;
	case LEVEL_CITY:
		if (FAILED(pLoader->Loading_ForCity()))
			MSG_BOX(L"Loader->Loading_City 실패!");
		break;
	case LEVEL_EVENT:
		if (FAILED(pLoader->Loading_ForEventLevel()))
			MSG_BOX(L"Loader->Loading_EventLevel 실패!");
		break;
	case LEVEL_HELL:
		if (FAILED(pLoader->Loading_ForHell()))
			MSG_BOX(L"Loader->Loading_HellLevel 실패!");
		break;
	case LEVEL_BOSS:
		if (FAILED(pLoader->Loading_ForBossLevel()))
			MSG_BOX(L"Loader->Loading_BossLevel 실패!");
		break;
	}


	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}
HRESULT CLoader::Loading_ForShelterLevel() {

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Map"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Map/%d.png"), 11))))
		return E_FAIL;


	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));


	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForStoreLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));


	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Tree"),
		CVIBuffer_Tree::Create(m_pGraphic_Device))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Shop_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Shop.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Buy_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Buy_%d.png"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLoader::Loading_ForForestLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Tree"),
		CVIBuffer_Tree::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Cone"),
		CVIBuffer_Cone::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForHell()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForEventLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForCave()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 17, 129))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForCity()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 80, 80))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NormalZombie"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Normal/Z_(%d).dds"), 18))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ZombieSpitter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Spitter/P_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_skeleton/Z_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Tree"),
		CVIBuffer_Tree::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForBossLevel() {

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(Basic_DataLoad())) {
		MSG_BOX(L"Basic_DataLoad 실패!");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 120, 120))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Boss_Blood"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Boss_Blood/Boss_Blood(%d).png"), 64))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_LuciferWing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss_Lucifer/Wing(%d).png"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Lucifer"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Boss_Lucifer/L_(%d).dds"), 8))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Meteor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Boss_Lucifer/Meteor/Meteor_%d.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Missile"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss_Lucifer/Missile/Missile(%d).png"),30))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blue_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Blue_skeleton/P_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Wither_skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Wither_Skeleton/Z_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/S_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Granpa_Skeleton/Wing(%d).png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fanatic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Monster/Fanatic/F(%d).dds"), 27))))
		return E_FAIL;
	

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLoader::Basic_DataLoad()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_NPC_Shop"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/NPC/N_(%d).dds"), 26))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Grenade"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grenade/Grenade_%d.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Tile"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/CartoonTile/%d.png"), 72))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Cone"),
		CVIBuffer_Cone::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Cube"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Wall/%d.dds"), 41))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Deco"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/RectY_Blend/%d.png"), 47))))

		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Black"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/BLACK.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Brown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/BROWN.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Green"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/GREEN.dds")))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_PurpleGate"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Gate/Type1/Purple/gate_(%d).png"), 96))))
		return E_FAIL;

	/* 바인드묶기 */
	if (FAILED(pGameInstance->Set_TextureCom(m_eLevel, TEXT("Prototype_Component_Renderer"), TEXT("Prototype_Component_Texture_Cube"), TEXT("Prototype_Component_Texture_Tile")))) {
		return E_FAIL;
	}

	/* For. Prototype_Component_Texture_Explosion */
	//이펙트 텍스쳐
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Fire"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Fire/fire_%d.png"), 32))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_SoulFire"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/SoulFire/soul_fire(%d).png"), 32))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Effect_Loop/Effect(%d).png"), 72))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Circle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Circle/Circle(%d).png"), 20))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Bullet_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Bullet_Effect/Bullet(%d).png"), 57))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Blood"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Blood/Blood(%d).png"), 160))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Weather"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Weather/Weather(%d).png"), 8))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_MagicCircle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/MagicCircle/MagicCircle(%d).png"), 6))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Lazer"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Boss_Lucifer/Lazer/Lazer_%d.dds"), 2))))
		return E_FAIL;



	//오브젝트 텍스쳐
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_PlayerCube"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Player/p_%d.dds"), 10))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Weapon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Weapon/Weapon(%d).dds"),5))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Bullet/Bullet(%d).dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Item_(%d).png"), 13))))
		return E_FAIL;
#pragma region UI텍스처
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_State_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/State.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_HP_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/HP.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Boss_HP_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Boss_HP.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Boss_State_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Boss_State.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Stamina_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Stamina.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Pistol_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Pistol.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Rifle_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Rifle.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_ShotGun_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/ShotGun.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Timber_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Timber.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Scene_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/lettering_box.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Inventory_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Inventory.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Equip_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Equip.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Shop_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Shop.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Mouse_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Mouse.png")))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_Texture_Interaction"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Interaction/%d.png"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_GameObject_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/YouDied/YouDied.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_GameObject_Credit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Credit.png")))))
		return E_FAIL;

#pragma endregion UI텍스처



	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));

	/* 오브젝트 버퍼 */

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Tile"),
		CVIBuffer_Tile::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Rifle"),
		CVIBuffer_Rifle::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Timber"),
		CVIBuffer_Timber::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Knife"),
		CVIBuffer_Knife::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Pistol"),
		CVIBuffer_Pistol::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_ShotGun"),
		CVIBuffer_ShotGun::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_UI_Rect"),
		CVIBuffer_UI_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(m_eLevel, TEXT("Prototype_Component_VIBuffer_Door"),
		CVIBuffer_Door::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	
	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGraphic_Device);
}
