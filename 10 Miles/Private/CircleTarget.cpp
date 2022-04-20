#include "stdafx.h"
#include "..\Public\CircleTarget.h"
#include "GameInstance.h"

CCircleTarget::CCircleTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CCircleTarget::CCircleTarget(const CCircleTarget & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CCircleTarget::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCircleTarget::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	MyType = *(EFFECT4*)pArg;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyType.Pos);
	m_pTransformCom->LookAt(MyType.Target);
	

	//m_pTransformCom->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_pSubTransformCom->Scaled(_float3(0.5f,0.5f, 0.5f));
	
	m_Texture = MyType.BulltType;
	
	
	return S_OK;
}

_int CCircleTarget::Tick(_float fTimeDelta)
{

	if (m_pSubTransformCom->Get_Scale().x<0.01)
		return 1;

	m_pSubTransformCom->Scaling(-fTimeDelta/ 3);
	m_pTransformCom->Go_Straight(fTimeDelta * MyType.Speed);
		
	__super::Tick(fTimeDelta);

	
	m_fFrame += fTimeDelta;
	return 0;
}

void CCircleTarget::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
 
	m_pSubTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//m_pSubTransformCom->Set_State(CTransform::STATE_POSITION, _float3(1.3f, 1.3f, 1.3f));
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pSubTransformCom->Get_Scale();

	m_pSubTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pSubTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_pSubTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);


	

	

	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CCircleTarget::Render()
{
	

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_Texture)))
		return E_FAIL;

	if (FAILED(m_pSubTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCircleTarget::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_SubTransform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pSubTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Circle"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CCircleTarget * CCircleTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCircleTarget*	pInstance = new CCircleTarget(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CircleTarget"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCircleTarget::Clone(void * pArg)
{
	CCircleTarget*	pInstance = new CCircleTarget(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CircleTarget"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCircleTarget::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSubTransformCom);
}
