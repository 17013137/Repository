#include "stdafx.h"
#include "..\Public\Timber_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"

CTimber_UI::CTimber_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTimber_UI::CTimber_UI(const CTimber_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTimber_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CTimber_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3((m_fSizeX*0.14f), -m_fSizeY*0.38f, 0.0001f));
	//m_pTransformCom->Scaled(_float3(m_fSizeX*0.05f, m_fSizeY*0.05f, 0.0001f));
	return S_OK;
}

_int CTimber_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (GetAsyncKeyState('1'))
		m_bWeapon = false;
	if (GetAsyncKeyState('2'))
		m_bWeapon = false;
	if (GetAsyncKeyState('3'))
		m_bWeapon = false;
	if (GetAsyncKeyState('4'))
		m_bWeapon = true;
	if (GetAsyncKeyState('5'))
		m_bWeapon = false;
	if (GetAsyncKeyState('G'))
		m_bWeapon = false;

	return 0;
}

void CTimber_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == m_bWeapon)
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.1f, m_fSizeY*0.04f, 0.0001f));
	else
		m_pTransformCom->Scaled(_float3(m_fSizeX*0.07f, m_fSizeY*0.03f, 0.0001f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CTimber_UI::Render()
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


	return S_OK;
}

HRESULT CTimber_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Timber_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CTimber_UI * CTimber_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTimber_UI*	pInstance = new CTimber_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTimber_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTimber_UI::Clone(void * pArg)
{
	CTimber_UI*	pInstance = new CTimber_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTimber_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimber_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
