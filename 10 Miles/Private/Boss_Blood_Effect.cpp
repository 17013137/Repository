#include "stdafx.h"
#include "..\Public\Boss_Blood_Effect.h"
#include "GameInstance.h"

CBoss_Blood_Effect::CBoss_Blood_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CBoss_Blood_Effect::CBoss_Blood_Effect(const CBoss_Blood_Effect & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CBoss_Blood_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Blood_Effect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_Effect_Type = *(EFFECTTYPE*)pArg;

	m_fFrame = (m_Effect_Type.Type)*16.f;
	MaxfFrame = (m_Effect_Type.Type + 1)*16.f-1.f;
	
	//m_fFrame = 0.f;
	LoopTime = MaxfFrame - MinfFrame;
	m_Effect_Type.Pos.y += 2;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos);
	m_pTransformCom->Scaled(_float3(5.f, 5.f, 1.f));
	m_pTransformCom->LookAt(m_Effect_Type.Target + _float3(rand()%10-5.f, rand() % 5 - 2.5f, rand() % 10 - 5.f));
	m_pTransformCom->Go_Straight(1.8f);
	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(90.0f));
	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(15.f+rand()%60));
	m_pTransformCom->Go_Straight(-0.5f);
	return S_OK;
}

_int CBoss_Blood_Effect::Tick(_float fTimeDelta)
{
	m_fFrame += 30 * fTimeDelta;
	
	if (m_fFrame >= MaxfFrame)
		return 1;

	__super::Tick(fTimeDelta);
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return 0;
}

void CBoss_Blood_Effect::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//_float4x4		ViewMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	//_float3		vScale = m_pTransformCom->Get_Scale();

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);




	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CBoss_Blood_Effect::Render()
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

HRESULT CBoss_Blood_Effect::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Boss_Blood"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

CBoss_Blood_Effect * CBoss_Blood_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Blood_Effect*	pInstance = new CBoss_Blood_Effect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_Blood_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_Blood_Effect::Clone(void * pArg)
{
	CBoss_Blood_Effect*	pInstance = new CBoss_Blood_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_Blood_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Blood_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
