#include "stdafx.h"
#include "..\Public\Pistol_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "EventMgr.h"

CPistol_UI::CPistol_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPistol_UI::CPistol_UI(const CPistol_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPistol_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CPistol_UI::NativeConstruct(void * pArg)
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

	CInventory* pPlayerPistolAmmo = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pPlayerPistolAmmo)
		return E_FAIL;
	Safe_AddRef(pPlayerPistolAmmo);

	m_iMaxPistolAmmo = pPlayerPistolAmmo->Get_ItemCount(CInventory::PISTOLAMMO);

	m_iPistolAmmo = pPlayerPistolAmmo->Get_bulletCount(CInventory::PISTOLAMMO);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 25, 8, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	//Pistol
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3((m_fSizeX*0.05f), -m_fSizeY*0.38f, 0.0001f));
	//m_pTransformCom->Scaled(_float3(m_fSizeX*0.05f, m_fSizeY*0.05f, 0.0001f));

	m_rPistolAmmo = { 680,648,0,0 };

	Safe_Release(pPlayerPistolAmmo);
	Safe_Release(pGameInstance);
	
	return S_OK;
}

_int CPistol_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GetAsyncKeyState('1'))
		m_bWeapon = false;
	if (GetAsyncKeyState('2'))
		m_bWeapon = false;
	if (GetAsyncKeyState('3'))
		m_bWeapon = true;
	if (GetAsyncKeyState('4'))
		m_bWeapon = false;
	if (GetAsyncKeyState('5'))
		m_bWeapon = false;
	if (GetAsyncKeyState('G'))
		m_bWeapon = false;

	return 0;
}

void CPistol_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == m_bWeapon)
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.07f, 0.0001f));
	else
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.05f, m_fSizeY*0.05f, 0.0001f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
	
}

HRESULT CPistol_UI::Render()
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
		_tchar State_PistolAmmo[MAX_PATH] = L"";
		wsprintf(State_PistolAmmo, L"%d/%d", *m_iPistolAmmo, *m_iMaxPistolAmmo);

		m_pd3dxFont->DrawText(NULL, State_PistolAmmo, -1, &m_rPistolAmmo, DT_NOCLIP, 0xffffffff);
	}
	return S_OK;
}

HRESULT CPistol_UI::SetUp_Components()
{
	/* For.Com_Transform */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	/* For.Com_Renderer */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	
	/* For.Com_Texture */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Pistol_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CPistol_UI * CPistol_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPistol_UI*	pInstance = new CPistol_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPistol_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPistol_UI::Clone(void * pArg)
{
	CPistol_UI*	pInstance = new CPistol_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPistol_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPistol_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);


}
