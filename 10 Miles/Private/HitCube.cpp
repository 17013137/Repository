#include "stdafx.h"
#include "..\Public\HitCube.h"
#include "GameInstance.h"

CHitCube::CHitCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CHitCube::CHitCube(const CHitCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHitCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHitCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	CStatus::STATUSDESC m_Status;
	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));

	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pTransformCom->Look(m_Status.fLook);
	m_pTransformCom->Scaled(_float3(5.f, 5.f, 5.f));

	//m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Status.fSpawnPos);

	//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(90.0f));

	return S_OK;
}

_int CHitCube::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	
	__super::Tick(fTimeDelta);

	//if (D3DXVec3Length(&(m_pStatusCom->m_Status.fLook - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))<20)
	//	m_bDead = true;

	
	m_pTransformCom->Go_Straight(fTimeDelta);

	if (CStatus::STATUS_STATE_DEAD == m_pStatusCom->Status_Tick(fTimeDelta))
		return 1;
	m_pStatusCom->fStamina_Timer(fTimeDelta);


	EFFECT4  Effect;
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 MyLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	for (int i = 0; i < 2; ++i)
	{

		Effect.Pos = MyPos + _float3(rand() % 20 - 10.f, -0.1f, rand() % 20 - 10.f) - MyLook * 2;
		Effect.Target = Effect.Pos - MyLook * (rand() % 10 / 5);
		Effect.BulltType = 20;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

	}



	return 0;

}

void CHitCube::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (nullptr == m_pGraphic_Device)
		return;





	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CHitCube::Render()
{
	//if (FAILED(__super::Render()))
	//	return E_FAIL;
	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
	//	return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CHitCube::SetUp_Components(void * pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	CStatus::STATUSDESC		m_Status;
	ZeroMemory(&m_Status, sizeof(CStatus::STATUSDESC));

	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));

	TransformDesc.fSpeedPerSec = m_Status.fMaxStungauge;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Lazer"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Status.Transform_Com->Get_State(CTransform::STATE_POSITION));
	m_Status.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &m_Status)))
		return E_FAIL;
	return S_OK;
}

CHitCube * CHitCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitCube*	pInstance = new CHitCube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHitCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHitCube::Clone(void * pArg)
{
	CHitCube*	pInstance = new CHitCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHitCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitCube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(pGameInstance);
}
