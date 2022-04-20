#include "stdafx.h"
#include "Deco.h"
#include "GameInstance.h"

CDeco::CDeco(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CDeco::CDeco(const CDeco & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CDeco::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	_float4x4 temp;
	memcpy(temp, pArg, sizeof(_float4x4));
	ImgNum = (int)temp.m[0][3];
	temp.m[0][3] = 0;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(temp.m[0][0], temp.m[0][1], temp.m[0][2]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(temp.m[1][0], temp.m[1][1], temp.m[1][2]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(temp.m[2][0], temp.m[2][1], temp.m[2][2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(temp.m[3][0], temp.m[3][1], temp.m[3][2]));

	return S_OK;
}

_int CDeco::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return 0;
}

void CDeco::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * (vScale.x*1.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * (vScale.y*1.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * (vScale.z*1.f));


	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
}


HRESULT CDeco::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(ImgNum)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CDeco::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Deco"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CDeco * CDeco::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDeco*	pInstance = new CDeco(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CDeco"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDeco::Clone(void * pArg)
{
	CDeco*	pInstance = new CDeco(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CDeco"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeco::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
