#include "stdafx.h"
#include "..\Public\Bullet.h"
#include "GameInstance.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	CStatus::STATUSDESC m_Status;
	if (nullptr != pArg)
		memcpy(&m_Status, pArg, sizeof(CStatus::STATUSDESC));
	_int i = (_int)m_Status.fMaxStamina;
	m_BulletType = (BULLET)i;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	
	m_pTransformCom->Look(m_Status.fLook);
	switch (m_BulletType)
	{
	case BULLET_RIFLE:
		m_pTransformCom->Scaled(_float3(0.12f, 0.12f, 0.24f));
		m_Texture = 0;
		break;
	case BULLET_SHOTGUN:
		m_pTransformCom->Scaled(_float3(0.1f, 0.1f, 0.2f));
		m_Texture = 0;
		break;
	case BULLET_PISTOL:
		m_pTransformCom->Scaled(_float3(0.1f, 0.1f, 0.2f));
		m_Texture = 0;
		break;
	case BULLET_ZOMBIE:
		m_pTransformCom->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_Texture = 1;
		break;
	default:
		m_pTransformCom->Scaled(_float3(0.1f, 0.1f, 0.2f));
		m_Texture = 0;
		break;
	}
	

	//m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_Pos = m_Status.fSpawnPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Pos);

	

	return S_OK;
}

_int CBullet::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	m_pTransformCom->Go_Straight(fTimeDelta);
	__super::Tick(fTimeDelta);

	//if (D3DXVec3Length(&(m_pStatusCom->m_Status.fLook - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))<20)
	//	m_bDead = true;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_delay <= 0) {
		EFFECT2 Type;
		Type.EffectType = m_BulletType;
		Type.EffectPos = MyPos;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Circle"), &Type)))
			return E_FAIL;
		m_delay = 0.01f;
	}
	
	if(m_Range < D3DXVec3Length(&(MyPos - m_Pos)))
		return 1;


	m_delay -= fTimeDelta;
	Safe_Release(pGameInstance);
	if(CStatus::STATUS_STATE_DEAD == m_pStatusCom->Status_Tick(fTimeDelta))
		m_bDead = true;
	m_pStatusCom->fStamina_Timer(fTimeDelta);
	return 0;

}

void CBullet::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	

	if (nullptr == m_pGraphic_Device)
		return;
	if (m_BulletType == BULLET_ZOMBIE)
	{
		Sub_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), fTimeDelta * 5);
		Sub_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	

	

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CBullet::Render()
{
	Sub_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

	if (FAILED(__super::Render()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_Texture)))
		return E_FAIL;

	if (FAILED(Sub_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	


	return S_OK;
}

HRESULT CBullet::SetUp_Components(void * pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if(m_BulletType== BULLET_ZOMBIE)
		TransformDesc.fSpeedPerSec = 8.f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Bullet"), (CComponent**)&m_pTextureCom)))
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

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet*	pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBullet::Clone(void * pArg)
{
	CBullet*	pInstance = new CBullet(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
