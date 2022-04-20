#include "stdafx.h"
#include "..\Public\SoulFire.h"
#include "GameInstance.h"

CSoulFire::CSoulFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CSoulFire::CSoulFire(const CSoulFire & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CSoulFire::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSoulFire::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	_float3 Pos = *(_float3*)pArg;
	Pos.y += 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);
	_float Scale = 1.5f + rand() % 2;
	m_pTransformCom->Scaled(_float3(Scale, Scale, Scale));
	m_fFrame = rand() % 32;
	return S_OK;
}

_int CSoulFire::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;

	__super::Tick(fTimeDelta);

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return 0;
}

void CSoulFire::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);


	m_fFrame += 32.0f * fTimeDelta;

	if (m_fFrame >= 32.0f)
		m_fFrame = 0.f;

	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CSoulFire::Render()
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

HRESULT CSoulFire::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_SoulFire"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CSoulFire * CSoulFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSoulFire*	pInstance = new CSoulFire(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CSoulFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSoulFire::Clone(void * pArg)
{
	CSoulFire*	pInstance = new CSoulFire(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CSoulFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoulFire::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
