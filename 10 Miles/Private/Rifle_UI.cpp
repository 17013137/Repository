#include "stdafx.h"
#include "..\Public\Rifle_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "EventMgr.h"

CRifle_UI::CRifle_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CRifle_UI::CRifle_UI(const CRifle_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRifle_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CRifle_UI::NativeConstruct(void * pArg)
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

	CInventory* pPlayerRifleAmmo = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pPlayerRifleAmmo)
		return E_FAIL;
	Safe_AddRef(pPlayerRifleAmmo);

	m_iMaxRifleAmmo = pPlayerRifleAmmo->Get_ItemCount(CInventory::RIFLEAMMO);

	m_iRifleAmmo = pPlayerRifleAmmo->Get_bulletCount(CInventory::RIFLEAMMO);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 25, 8, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	
	//Rifle
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-(m_fSizeX*0.13f), -m_fSizeY*0.38f, 0.0001f));
	//m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.07f, 0.0001f));

	m_rRifleAmmo = { 450,648,0,0 };

	Safe_Release(pPlayerRifleAmmo);
	Safe_Release(pGameInstance);

	return S_OK;
}

_int CRifle_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GetAsyncKeyState('1'))
		m_bWeapon = true;
	if (GetAsyncKeyState('2'))
		m_bWeapon = false;
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

void CRifle_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == m_bWeapon)
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.1f, m_fSizeY*0.1f, 0.0001f));
	else
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.07f, 0.0001f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);

}

HRESULT CRifle_UI::Render()
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
		_tchar State_RifleAmmo[MAX_PATH] = L"";
		wsprintf(State_RifleAmmo, L"%d/%d", *m_iRifleAmmo, *m_iMaxRifleAmmo);

		m_pd3dxFont->DrawText(NULL, State_RifleAmmo, -1, &m_rRifleAmmo, DT_NOCLIP, 0xffffffff);
	}
	return S_OK;
}

HRESULT CRifle_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Rifle_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CRifle_UI * CRifle_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRifle_UI*	pInstance = new CRifle_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRifle_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRifle_UI::Clone(void * pArg)
{
	CRifle_UI*	pInstance = new CRifle_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CRifle_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRifle_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
