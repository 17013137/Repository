#include "stdafx.h"
#include "..\Public\Boss_State_UI.h"
#include "GameInstance.h"

CBoss_State_UI::CBoss_State_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBoss_State_UI::CBoss_State_UI(const CBoss_State_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_State_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_State_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 1.0f;
	m_fY = 10.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CStatus*		pBossStatus = (CStatus*)pGameInstance->Get_Component(m_Current_Level, TEXT("Monster"), TEXT("Com_Status"));
	if (nullptr == pBossStatus)
		return E_FAIL;
	Safe_AddRef(pBossStatus);
	
	m_MaxHP = &pBossStatus->m_Status.fMaxHp;

	m_HP = pBossStatus->Get_pHp();
	
	Safe_Release(pBossStatus);
	Safe_Release(pGameInstance);
	
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, m_fSizeY*0.46f, 0.0002f));
	m_pTransformCom->Scaled(_float3(m_fSizeX*0.7f, m_fSizeY*0.04f, 0.0001f));

	return S_OK;
}

_int CBoss_State_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	

	return 0;
}

void CBoss_State_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CBoss_State_UI::Render()
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

HRESULT CBoss_State_UI::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Boss_State_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CBoss_State_UI * CBoss_State_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_State_UI*	pInstance = new CBoss_State_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_State_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_State_UI::Clone(void * pArg)
{
	CBoss_State_UI*	pInstance = new CBoss_State_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_State_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_State_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
