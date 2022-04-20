#include "stdafx.h"
#include "..\Public\Kelp.h"
#include "GameInstance.h"

CKelp::CKelp(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CKelp::CKelp(const CKelp & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CKelp::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKelp::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	MyType = *(EFFECT2*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyType.EffectPos);
	m_pTransformCom->Scaled(_float3(0.3f, 0.3f, 0.3f));

	if (MyType.EffectType == 4)
	{
		m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
		m_Texture = 3;
	}

	if (MyType.EffectType == 3)
	{
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_Texture = 2;
		MaxfFrame = 15;
	}
		
	return S_OK;
}

_int CKelp::Tick(_float fTimeDelta)
{
	m_fFrame += 60 * fTimeDelta;
	if (MyType.EffectType == 3)
	{
		m_pTransformCom->Go_Y(-(fTimeDelta));
	}
	if (m_fFrame >= MaxfFrame)
		return 1;
	
		
	__super::Tick(fTimeDelta);

	

	return 0;
}

void CKelp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);


	

	m_pTransformCom->Scaling(-fTimeDelta);
	if (MyType.EffectType == 4)
	{
		m_pTransformCom->Scaling(-fTimeDelta*5);
	}

	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CKelp::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_Texture)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CKelp::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Circle"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CKelp * CKelp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKelp*	pInstance = new CKelp(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CKelp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKelp::Clone(void * pArg)
{
	CKelp*	pInstance = new CKelp(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CKelp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKelp::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
