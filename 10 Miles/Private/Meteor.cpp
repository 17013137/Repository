#include "stdafx.h"
#include "..\Public\Meteor.h"
#include "GameInstance.h"
#include "Boss_Lucifer.h"

CMeteor::CMeteor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMeteor::CMeteor(const CMeteor & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMeteor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteor::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	EFFECT4 MeteorData;
	if (nullptr != pArg)
		memcpy(&MeteorData, pArg, sizeof(EFFECT4));


	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;
	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pTransformCom->Scaled(_float3(1.5f, 1.5f, 1.5f));



	//m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_Pos = MeteorData.Pos;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MeteorData.Pos);
	m_pTransformCom->LookAt(MeteorData.Target);
	
	m_Texture = MeteorData.BulltType;
	return S_OK;
}

_int CMeteor::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	m_pTransformCom->Go_Straight(fTimeDelta);
	__super::Tick(fTimeDelta);

	//if (D3DXVec3Length(&(m_pStatusCom->m_Status.fLook - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))<20)
	//	m_bDead = true;


	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_delay >= 0.001) {
		EFFECT2 Type;
		Type.EffectType = 4;
		Type.EffectPos = MyPos;
		Type.EffectPos.x = MyPos.x + rand() % 100 / 50.f - 1;
		Type.EffectPos.z = MyPos.z + rand() % 100 / 50.f - 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Circle"), &Type)))
			return E_FAIL;
		Type.EffectPos = MyPos;
		Type.EffectPos.x = MyPos.x + rand() % 100 / 50.f - 1;
		Type.EffectPos.z = MyPos.z + rand() % 100 / 50.f - 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Circle"), &Type)))
			return E_FAIL;
		Type.EffectPos = MyPos;
		Type.EffectPos.x = MyPos.x + rand() % 100 / 50.f - 1;
		Type.EffectPos.z = MyPos.z + rand() % 100 / 50.f - 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Circle"), &Type)))
			return E_FAIL;
		m_delay = 0.f;
	}


	m_delay += fTimeDelta;

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 0 && On == true)
	{
		EFFECT4  Effect;

		for (int i = 0; i < 30; ++i)
		{
			Effect.Pos = MyPos;
			Effect.Target = MyPos + _float3(rand() % 7 - 3.5f, -0.1f, rand() % 7 - 3.5f);
			Effect.BulltType = 10;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

		}

		EFFECTTYPE RandPos;
		RandPos.Pos = MyPos;
		RandPos.Pos.y = 0.1f;
		RandPos.Type = 3;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;

		CStatus::STATUSDESC		StatusmDesc;
		ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
		StatusmDesc.fMaxHp = 1.f;
		StatusmDesc.fMaxStamina = 1100.f;
		StatusmDesc.fMaxStungauge = 100.f;
		StatusmDesc.fDamage = 35.f;
		StatusmDesc.fPower = 0.7f;
		StatusmDesc.frange = 5.f;
		StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
		StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;

		StatusmDesc.Transform_Com = m_pTransformCom;

		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
		if (m_Texture == 1)
		{
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Monster"), TEXT("Prototype_GameObject_Wither_skeleton"), &(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)));
		}
		
		On = false;
		return 1;
	}



	if (CStatus::STATUS_STATE_DEAD == m_pStatusCom->Status_Tick(fTimeDelta))
		m_bDead = true;

	return 0;

}

void CMeteor::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (nullptr == m_pGraphic_Device)
		return;

	Sub_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), (fTimeDelta * 5));
	Sub_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), (fTimeDelta * 3));
	m_pTransformCom->Go_Straight(fTimeDelta);



	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CMeteor::Render()
{
	Sub_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(Sub_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CMeteor::SetUp_Components(void * pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 8.f;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Meteor"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Sub"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&Sub_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CStatus::STATUSDESC		m_Status;
	ZeroMemory(&m_Status, sizeof(CStatus::STATUSDESC));

	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Status.Transform_Com->Get_State(CTransform::STATE_POSITION));
	m_Status.Transform_Com = m_pTransformCom;
	m_Range = m_Status.fMaxStungauge;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &m_Status)))
		return E_FAIL;
	return S_OK;
}

CMeteor * CMeteor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMeteor*	pInstance = new CMeteor(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMeteor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteor::Clone(void * pArg)
{
	CMeteor*	pInstance = new CMeteor(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMeteor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteor::Free()
{
	__super::Free();
	Safe_Release(pGameInstance);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
