#include "stdafx.h"
#include "..\Public\Effect_Loop.h"
#include "GameInstance.h"

CEffect_Loop::CEffect_Loop(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CEffect_Loop::CEffect_Loop(const CEffect_Loop & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CEffect_Loop::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Loop::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_Effect_Type = *(EFFECTTYPE*)pArg;
	switch (m_Effect_Type.Type)
	{
	case Client::CEffect_Loop::TYPE_SHOTGUN1:
		m_fFrame = 0.f;
		MaxfFrame = 11.f;
		m_pTransformCom->Scaled(_float3(2.5f, 2.5f, 2.5f));
		break;
	case Client::CEffect_Loop::TYPE_PISTOL1:
		m_fFrame = 12.f;
		MaxfFrame = 22.f;
		m_pTransformCom->Scaled(_float3(3.f, 3.f, 3.f));
		break;
	case Client::CEffect_Loop::TYPE_PISTOL2:
		m_fFrame = 23.f;
		MaxfFrame = 28.f;
		m_pTransformCom->Scaled(_float3(3.f, 3.f, 3.f));
		break;
	case Client::CEffect_Loop::TYPE_SHOTGUN2:
		m_fFrame = 29.f;
		MaxfFrame = 32.f;
		m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));
		break;
	case Client::CEffect_Loop::TYPE_RIFLE:
		m_fFrame = 33.f;
		MaxfFrame = 40.f;
		m_pTransformCom->Scaled(_float3(3.f, 3.f, 3.f));
		break;
	case Client::CEffect_Loop::TYPE_ZOMBIE:
		m_fFrame = 41.f;
		MaxfFrame = 46.f;
		m_pTransformCom->Scaled(_float3(4.f, 4.f, 3.f));
		break;
	case Client::CEffect_Loop::TYPE_TIMBER:
		m_fFrame = 47.f;
		MaxfFrame = 56.f;
		m_pTransformCom->Scaled(_float3(3.f, 3.f, 3.f));
		break;


	case Client::CEffect_Loop::TYPE_END:
		break;
	default:
		break;
	}
	//m_fFrame = 0.f;
	LoopTime = MaxfFrame - MinfFrame;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos);

	return S_OK;
}

_int CEffect_Loop::Tick(_float fTimeDelta)
{
	m_fFrame += 60 * fTimeDelta;

	if (m_fFrame >= MaxfFrame)
		return 1;

	__super::Tick(fTimeDelta);

	

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return 0;
}

void CEffect_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);


	

	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CEffect_Loop::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Loop::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.3f;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Bullet_Effect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	return S_OK;
}

CEffect_Loop * CEffect_Loop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Loop*	pInstance = new CEffect_Loop(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CEffect_Loop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Loop::Clone(void * pArg)
{
	CEffect_Loop*	pInstance = new CEffect_Loop(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CEffect_Loop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Loop::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
