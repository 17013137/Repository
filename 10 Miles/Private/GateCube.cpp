#include "stdafx.h"
#include "GateCube.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include "MapBase.h"

CGateCube::CGateCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CGateCube::CGateCube(const CGateCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGateCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGateCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	_float4x4 temp;
	memcpy(temp, pArg, sizeof(_float4x4));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(temp.m[0][0], temp.m[0][1], temp.m[0][2]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(temp.m[1][0], temp.m[1][1], temp.m[1][2]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(temp.m[2][0], temp.m[2][1], temp.m[2][2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(temp.m[3][0], temp.m[3][1], temp.m[3][2]));

	m_vTFTalkBox->Set_State(CTransform::STATE_POSITION, _float3(temp.m[3][0], temp.m[3][1] + 1.5f, temp.m[3][2]));
	m_vTFTalkBox->Scaled(_float3(1.f, 0.7f, 1.f));
	m_col_Range = 1.f;



	return S_OK;
}

_int CGateCube::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);




	return 0;
}

void CGateCube::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (nullptr == m_pGraphic_Device)
		return;

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);

	CTransform* trans = (CTransform*)gameins->Get_Component(m_Current_Level, L"Layer_Player", L"Com_Transform");
	m_Player_Pos = trans->Get_State(CTransform::STATE_POSITION);

	Safe_Release(gameins);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CGateCube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(20)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(m_vTTTalkBox->Bind_OnGraphicDevice(1)))
		return E_FAIL;

	if (FAILED(m_vTFTalkBox->Bind_OnGraphicDevice()))
		return E_FAIL;
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_vTFTalkBox->Get_Scale();

	m_vTFTalkBox->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_vTFTalkBox->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_vTFTalkBox->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);
	if (D3DXVec3Length(&(m_Player_Pos - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 3.0f) {
		m_vVITalkBox->Render();
	}

	Safe_Release(gameins);
	return S_OK;
}


HRESULT CGateCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.3f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Cube"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	if (FAILED(__super::SetUp_Components(TEXT("Com_TalkBoxTransform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_vTFTalkBox, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_TalkBoxRenderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDTalkBox)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_TalkBoxVIBuffer"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_vVITalkBox)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_TalkBoxTexture"), m_Current_Level, TEXT("Prototype_Component_Texture_Interaction"), (CComponent**)&m_vTTTalkBox)))
		return E_FAIL;

	return S_OK;
}

CGateCube * CGateCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGateCube*	pInstance = new CGateCube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CGateCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGateCube::Clone(void * pArg)
{
	CGateCube*	pInstance = new CGateCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CGateCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGateCube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
