#include "stdafx.h"
#include "..\Public\Missile.h"
#include "GameInstance.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CMissile::CMissile(const CMissile & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CMissile::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMissile::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	MyType = *(EFFECT4*)pArg;




	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyType.Pos);
	//m_pTransformCom->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_pSubTransformCom->Scaled(_float3(0.1f, 0.1f, 0.1f));
	m_Type = MyType.BulltType;
	m_Delay = 0.7f;
	m_pTransformCom->LookAt(MyType.Target);
	m_fFrame = 6 / m_Delay;
	m_Texture = m_Type * 15.f;
	m_Speed = 0.5 + rand() % 30 / 50;
	return S_OK;
}

_int CMissile::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (m_pSubTransformCom->Get_Scale().x < 0.01)
		return 1;

	//m_pSubTransformCom->Scaling(-fTimeDelta/ 3);

	__super::Tick(fTimeDelta);
	if (Page == 1)
	{
		m_Delay -= fTimeDelta;
		m_pSubTransformCom->Turn(_float3(0.f, 0.f, 1.f), 0.5f);
		m_pSubTransformCom->Scaling(fTimeDelta * 8);
		m_pTransformCom->Go_Straight(m_Speed*m_Delay);
		m_Texture += m_fFrame*fTimeDelta;
		if (m_Delay < 0)
		{
			Page = 2;
			m_Delay = MyType.Speed;
		}
	}
	else if (Page == 2)
	{
		m_Delay -= fTimeDelta;
		m_pSubTransformCom->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		if (m_Delay < 0)
		{

			Page = 3;
			_float3 P = ((CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
			_float3 L = ((CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK);
			MyType.Target = P+L*10.f;
			m_pTransformCom->LookAt(MyType.Target);

		}
	}
	else if (Page == 3)
	{
		if (m_Delay < 35)
			m_Delay += fTimeDelta * 20;


		m_pTransformCom->Go_Straight(fTimeDelta*m_Delay);
		if (D3DXVec3Length(&(MyType.Target - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 1.f)
			Page = 4;

	}
	else if (Page == 4)
	{
		m_Texture += 60 * fTimeDelta;
		m_pSubTransformCom->Scaling(fTimeDelta * 30);
	}

	if (m_Texture - m_Type * 15 >= 15)
	{
		CStatus::STATUSDESC		StatusmDesc;
		ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
		StatusmDesc.fMaxHp = 1.f;
		StatusmDesc.fMaxStamina = 1100.f;
		StatusmDesc.fMaxStungauge = 100.f;
		StatusmDesc.fDamage = 20.f;
		StatusmDesc.fPower = 0.5f;
		StatusmDesc.frange = 3.f;
		StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
		StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3;
		StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

		StatusmDesc.Transform_Com = m_pTransformCom;

		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
		if (MyType.BulltType == 0)
		{
			for (int i = 0; i < 20; ++i)
			{
				EFFECT4 Effect;
				Effect.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				Effect.Target = Effect.Pos + _float3(rand() % 30 - 15.f, rand() % 5 + 1.f, rand() % 30 - 15.f);
				Effect.BulltType = 6;
				Effect.Speed = 5;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			}
		}
		else
		{
			for (int i = 0; i < 20; ++i)
			{
				EFFECT4 Effect;
				Effect.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				Effect.Target = Effect.Pos + _float3(rand() % 30 - 15.f, rand() % 5 + 1.f, rand() % 30 - 15.f);
				Effect.BulltType = 13;
				Effect.Speed = 5;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			}
		}
		return 1;
	}



	EFFECT4 Effect;
	Effect.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (MyType.BulltType == 0)
	{
		Effect.Target = Effect.Pos + _float3(rand() % 30 - 15.f, rand() % 5 + 1.f, rand() % 30 - 15.f);
		Effect.BulltType = 6;
	}
	else
	{
		Effect.Target = Effect.Pos;
		Effect.Target.y -= 5;
		Effect.BulltType = 13;
	}

	Effect.Speed = 2;
	pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);


	Safe_Release(pGameInstance);
	return 0;
}

void CMissile::LateTick(_float fTimeDelta)
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
	//m_pSubTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);






	Compute_CamDistance(m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CMissile::Render()
{


	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_int)m_Texture)))
		return E_FAIL;

	if (FAILED(m_pSubTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMissile::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Missile"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CMissile * CMissile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMissile*	pInstance = new CMissile(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CircleTarget"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMissile::Clone(void * pArg)
{
	CMissile*	pInstance = new CMissile(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CircleTarget"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissile::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSubTransformCom);
}
