#include "stdafx.h"
#include "..\Public\PurpleGate.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include "Level_Loading.h"

CPurpleGate::CPurpleGate(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPurpleGate::CPurpleGate(const CPurpleGate & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPurpleGate::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPurpleGate::NativeConstruct(void * pArg)
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
	return S_OK;
}

_int CPurpleGate::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (true == m_bDead)
		return 1;
	state = Red;
	return 0;
}

void CPurpleGate::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);

	//if (gameins->Check_EmptyLayer(m_Current_Level, L"Monster") == true)
	//	state = Green;
	//else
	//	state = Red;

	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_DEAD)
	{
		state = Green;
	}

	m_fFrame += 8.0 * fTimeDelta;

	if (m_fFrame >= 32.f * (state + 1))
		m_fFrame = (_float)state * 32.f;


	CTransform* trans = (CTransform*)gameins->Get_Component(m_Current_Level, L"Layer_Player", L"Com_Transform");
	m_Player_Pos = trans->Get_State(CTransform::STATE_POSITION);

	Safe_Release(gameins);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CPurpleGate::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPurpleGate::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_PurpleGate"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 100.f;
	StatusmDesc.fMaxStamina = 100.f;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 10.f;
	StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;
	StatusmDesc.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &StatusmDesc)))
		return E_FAIL;
	return S_OK;
}

CPurpleGate * CPurpleGate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPurpleGate*	pInstance = new CPurpleGate(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPurpleGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPurpleGate::Clone(void * pArg)
{
	CPurpleGate*	pInstance = new CPurpleGate(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPurpleGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPurpleGate::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
