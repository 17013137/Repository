#include "stdafx.h"
#include "..\Public\ShotGun_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "EventMgr.h"

CShotGun_UI::CShotGun_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CShotGun_UI::CShotGun_UI(const CShotGun_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShotGun_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CShotGun_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CInventory* pPlayerShotGunAmmo = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pPlayerShotGunAmmo)
		return E_FAIL;
	Safe_AddRef(pPlayerShotGunAmmo);

	m_iMaxShotGunAmmo = pPlayerShotGunAmmo->Get_ItemCount(CInventory::SHOTGUNAMMO);

	m_iShotGunAmmo = pPlayerShotGunAmmo->Get_bulletCount(CInventory::SHOTGUNAMMO);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 25, 8, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-(m_fSizeX*0.04f), -m_fSizeY*0.38f, 0.0001f));
	m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.07f, 0.0001f));

	m_rShotGunAmmo = { 565,648,0,0 };

	Safe_Release(pPlayerShotGunAmmo);
	Safe_Release(pGameInstance);

	return S_OK;
}

_int CShotGun_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (GetAsyncKeyState('1'))
		m_bWeapon = false;
	if (GetAsyncKeyState('2'))
		m_bWeapon = true;
	if (GetAsyncKeyState('3'))
		m_bWeapon = false;
	if (GetAsyncKeyState('4'))
		m_bWeapon = false;
	if (GetAsyncKeyState('5'))
		m_bWeapon = false;
	if (GetAsyncKeyState('G'))
		m_bWeapon = false;

	return 0;
}

void CShotGun_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == m_bWeapon)
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.1f, m_fSizeY*0.1f, 0.f));
	else
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.07f, 0.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CShotGun_UI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);	
	
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);	
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	m_pVIBufferCom->Render();
	if (false == CEventMgr::Get_Instance()->Get_OnAir() && false == CEventMgr::Get_Instance()->Get_OnCredit())
	{
		_tchar State_ShotGunAmmo[MAX_PATH] = L"";
		wsprintf(State_ShotGunAmmo, L"%d/%d", *m_iShotGunAmmo, *m_iMaxShotGunAmmo);

		m_pd3dxFont->DrawText(NULL, State_ShotGunAmmo, -1, &m_rShotGunAmmo, DT_NOCLIP, 0xffffffff);
	}
	return S_OK;
}

HRESULT CShotGun_UI::SetUp_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_ShotGun_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}


CShotGun_UI * CShotGun_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShotGun_UI*	pInstance = new CShotGun_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CShotGun_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShotGun_UI::Clone(void * pArg)
{
	CShotGun_UI*	pInstance = new CShotGun_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CShotGun_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShotGun_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
