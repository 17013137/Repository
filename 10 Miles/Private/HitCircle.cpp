#include "stdafx.h"
#include "..\Public\HitCircle.h"
#include "GameInstance.h"

CHitCircle::CHitCircle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CHitCircle::CHitCircle(const CHitCircle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHitCircle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHitCircle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	CStatus::STATUSDESC m_Status;
	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));


	m_pTransformCom->Look(m_Status.fLook);
	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	//m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Status.fSpawnPos);

	//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(90.0f));

	return S_OK;
}

_int CHitCircle::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	
	__super::Tick(fTimeDelta);

	//if (D3DXVec3Length(&(m_pStatusCom->m_Status.fLook - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))<20)
	//	m_bDead = true;

	
	
	if (CStatus::STATUS_STATE_DEAD == m_pStatusCom->Status_Tick(fTimeDelta))
		return 1;
	m_pStatusCom->fStamina_Timer(fTimeDelta);
	return 0;

}

void CHitCircle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (nullptr == m_pGraphic_Device)
		return;





	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CHitCircle::Render()
{
	//if (FAILED(__super::Render()))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(1)))
	//	return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CHitCircle::SetUp_Components(void * pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	CStatus::STATUSDESC		m_Status;
	ZeroMemory(&m_Status, sizeof(CStatus::STATUSDESC));

	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Status.Transform_Com->Get_State(CTransform::STATE_POSITION));
	m_Status.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &m_Status)))
		return E_FAIL;
	return S_OK;
}

CHitCircle * CHitCircle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitCircle*	pInstance = new CHitCircle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHitCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHitCircle::Clone(void * pArg)
{
	CHitCircle*	pInstance = new CHitCircle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHitCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitCircle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
