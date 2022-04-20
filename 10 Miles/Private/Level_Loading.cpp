#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "GameInstance.h"
#include "Loader.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "Camera_Logo.h"

#include "Level_GamePlay.h"
#include "Level_Shelter.h"
#include "Level_Boss.h"
#include "Level_Forest.h"
#include "Level_Store.h"
#include "Level_Cave.h"
#include "Level_Event.h"
#include "Level_Hell.h"
#include "Level_City.h"
#include "EventMgr.h"


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM1);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM2);
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
		
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 26, 9, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	m_rLoading_MSG = { 500, 570, 1230, 700 };

	CEventMgr::Get_Instance()->Set_LoadingNum();

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	switch (m_eNextLevel)
	{
	case LEVEL_SHELTER:
		CSoundMgr::Get_Instance()->PlaySound(L"Loading_Shelter.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_STORE:
		CSoundMgr::Get_Instance()->PlaySound(L"Loading_Shelter.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_GAMEPLAY:
		CSoundMgr::Get_Instance()->PlaySound(L"BGM2_mp3.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_FOREST:
		CSoundMgr::Get_Instance()->PlaySound(L"Ending_mp3.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_CAVE:
		CSoundMgr::Get_Instance()->PlaySound(L"BGM_9.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_CITY:
		CSoundMgr::Get_Instance()->PlaySound(L"BGM3_mp3.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_EVENT:
		CSoundMgr::Get_Instance()->PlaySound(L"BGM18_mp3.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_HELL:
		CSoundMgr::Get_Instance()->PlaySound(L"Opening.mp3", CSoundMgr::BGM1, 0.1f);
		break;
	case LEVEL_BOSS:
		CSoundMgr::Get_Instance()->PlaySound(L"BGM14_mp3.mp3", CSoundMgr::BGM1, 0.1f);
		CSoundMgr::Get_Instance()->PlaySound(L"Whisper.mp3", CSoundMgr::BOSS_WHISPER, 0.3f);
		break;
	}

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (true == m_pLoader->Get_Finished())
		{
			CLevel*		pLevel = nullptr;
			pGameInstance->Set_LevelIndex(m_eNextLevel);

			switch (m_eNextLevel)
			{
			case LEVEL_SHELTER:
				pLevel = CLevel_Shelter::Create(m_pGraphic_Device);
				break;
			case LEVEL_STORE:
				pLevel = CLevel_Store::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			case LEVEL_FOREST:
				pLevel = CLevel_Forest::Create(m_pGraphic_Device);
				break;
			case LEVEL_CAVE:
				pLevel = CLevel_Cave::Create(m_pGraphic_Device);
				break;
			case LEVEL_CITY:
				pLevel = CLevel_City::Create(m_pGraphic_Device);
				break;
			case LEVEL_EVENT:
				pLevel = CLevel_Event::Create(m_pGraphic_Device);
				break;
			case LEVEL_HELL:
				pLevel = CLevel_Hell::Create(m_pGraphic_Device);
				break;
			case LEVEL_BOSS:
				pLevel = CLevel_Boss::Create(m_pGraphic_Device);
				break;
			}


			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				goto Finish;
		}
	}

Finish:
	Safe_Release(pGameInstance);
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_tchar		szText[MAX_PATH] = TEXT("");

	lstrcpy(szText, TEXT("�ε� �����Դϴ� : "));
	lstrcat(szText, m_pLoader->Get_LoadingText());

	SetWindowText(g_hWnd, szText);

	_tchar Loading_MSG1[20][MAX_PATH] = {};
	wsprintf(Loading_MSG1[1], TEXT("�߽��� ���� �ֺ����� ���Ҹ��� ���� �̻��ѼҸ��� �鸰��."));
	wsprintf(Loading_MSG1[2], TEXT("�̻��� �Ҹ��� ���� ����� ���ٸ� ������ ��������."));
	wsprintf(Loading_MSG1[3], TEXT("������� �÷¿� ���� �����Ѵ�."));
	wsprintf(Loading_MSG1[4], TEXT("������ ���� ���� �������� �����غ��߰ڴ�."));
	wsprintf(Loading_MSG1[5], TEXT("������ ������ ��𼭵� ��Ÿ�� �� �ְ�, ���� ��Ÿ���� �� �� ����."));
	wsprintf(Loading_MSG1[6], TEXT("������ ���ɼ� ��򰡿� �ִ�. ���������?"));
	wsprintf(Loading_MSG1[7], TEXT("���ô� ����� ���� ��ŭ ���� ������?"));
	wsprintf(Loading_MSG1[8], TEXT("������ ���ð� ���δ�."));
	wsprintf(Loading_MSG1[9], TEXT("�� �ʸ� ���̴� ���ÿ��� ��� ���� �������ʴ´�."));
	wsprintf(Loading_MSG1[10], TEXT("���� ������ ������ �����ִ� ������ ���δ�."));
	wsprintf(Loading_MSG1[11], TEXT("����� �׵鿡�� ���� �������̿���.. �ذ���ٸ� ���.."));
	wsprintf(Loading_MSG1[12], TEXT("�̷����� ��� ������ ��Ÿ���°ɱ�?"));
	wsprintf(Loading_MSG1[13], TEXT("Shift�� �ٱ��. �̰� �����ٸ� ����� ���̴�."));
	wsprintf(Loading_MSG1[14], TEXT("������ ���� ������?"));
	wsprintf(Loading_MSG1[15], TEXT("������ ���� �־���... ���� ����ʹ�.."));
	wsprintf(Loading_MSG1[16], TEXT("���񿡰Լ� ��ź�� ���������ʴ´�. ���� �������ٸ� �������ʾ�����?"));
	wsprintf(Loading_MSG1[17], TEXT("õ��� �ξ��� �������ִ�."));

	m_pd3dxFont->DrawText(NULL, Loading_MSG1[CEventMgr::Get_Instance()->Get_LoadingNum()], -1, &m_rLoading_MSG, DT_RIGHT, 0xff898989);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Logo_Sky"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Loading_Cube"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Loading_Rect"))))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CLevel_Loading::Ready_Layer_Camera(const _tchar * pLayerTag)
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


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Camera_Logo"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);


}
