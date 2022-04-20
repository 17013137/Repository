#include "stdafx.h"
#include "Cone.h"
#include "GameInstance.h"

CCone::CCone(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCone::CCone(const CCone & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCone::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCone::NativeConstruct(void * pArg)
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

	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pTransformCom->Scaled(_float3(2.f, 12.f, 2.f));
	m_pStatusCom->Look_Off();
	m_pStatusCom->Knockback_off();
	m_col_Range = 3.f;
	return S_OK;
}

_int CCone::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
	{
		if (State != STATE_DOWN)
			Falling(fTimeDelta);

		//기어가기
		//m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta);
		//m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

		//Crawl(fTimeDelta);
	}
	if (State == STATE_FALLING)
	{
		Falling(fTimeDelta);
	}
	if (State == STATE_DOWN)
	{
		Down(fTimeDelta);
	}
	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECT4 Effect;

		//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER2_BEATEN);
		//CSoundMgr::Get_Instance()->PlaySound(L"Monster_Beaten1.wav", CSoundMgr::MONSTER2_BEATEN, 0.6f);
		MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		for (int i = 0; i < 2; ++i)
		{
			Effect.Pos = MyPos;
			Effect.Target = Effect.Pos + _float3(rand() % 10 - 5.f, rand() % 3 - 4.f, rand() % 10 - 5.f);
			Effect.BulltType = 0;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

		}
	}

	return 0;
}

void CCone::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CCone::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCone::SetUp_Components()
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

	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Cone"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 1000.f;
	StatusmDesc.fMaxStamina = 100.f;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 10.f;
	StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;
	StatusmDesc.Transform_Com = m_pTransformCom;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &StatusmDesc)))
		return E_FAIL;


	return S_OK;
}

void CCone::Falling(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	if (State != STATE_FALLING)
	{

		State = STATE_FALLING;
		AnFrame = 0;
	}

	RH = RH *fTimeDelta;
	LH = LH*fTimeDelta;
	RF = RF*fTimeDelta;
	LF = LF*fTimeDelta;
	EFFECT4 Effect;

	if (AnFrame < 5)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-AnFrame / 100));
		m_pTransformCom->Go_Y(-AnFrame / 600);
		Effect.Pos = MyPos;
		Effect.Pos.y = 0.1;
		Effect.Target = Effect.Pos + _float3(rand() % 15 - 7.5f, rand() % 3, rand() % 15 - 7.5f);
		Effect.BulltType = 0;


		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);
		if (Time < 0)
		{
			_float3	vPosition = MyPos + _float3(rand() % 3 - 1.5f, rand() % 3, rand() % 3 - 1.5f);
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Explosion"), vPosition);
			Time = 1.f + rand() % 20 / 10;
		}
	}
	else if (AnFrame < 6)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-AnFrame / 100));
		m_pTransformCom->Go_Y(-AnFrame / 600);
		Effect.Pos = MyPos;
		Effect.Pos.y = 0.1;
		Effect.Target = Effect.Pos + _float3(rand() % 15 - 7.5f, rand() % 3, rand() % 15 - 7.5f);
		Effect.BulltType = 0;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);
		if (Time < 0)
		{
			_float3	vPosition = MyPos + _float3(rand() % 3 - 1.5f, rand() % 3, rand() % 3 - 1.5f);
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Explosion"), vPosition);
			Time = 1.f + rand() % 20 / 10;
		}
	}
	else if (AnFrame < 13)
	{

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-AnFrame / 100));
		//m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f));
		m_pTransformCom->Go_Y(-AnFrame / 600);
		Effect.Pos = MyPos;
		Effect.Pos.y = 0.1;
		Effect.Target = Effect.Pos + _float3(rand() % 15 - 7.5f, rand() % 3, rand() % 15 - 7.5f);
		Effect.BulltType = 0;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);
		if (Time < 0)
		{
			_float3	vPosition = MyPos + _float3(rand() % 3 - 1.5f, rand() % 3, rand() % 3 - 1.5f) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT);;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Explosion"), vPosition);
			Time = 1.f + rand() % 20 / 10;
		}
	}
	else if (AnFrame < 15)
	{
		m_pTransformCom->Go_Y(-AnFrame / 600);

	}


	Effect.Pos = MyPos;
	Effect.Pos.y = 0.1;
	Effect.Target = Effect.Pos + _float3(rand() % 15 - 7.5f, rand() % 3, rand() % 15 - 7.5f);
	Effect.BulltType = 0;
	pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);
	Time-= fTimeDelta;
	AnFrame += fTimeDelta;
	Action = true;

	if (AnFrame >= 14)
	{
		AnFrame = 0;
		State = STATE_DOWN;
	}


}
void CCone::Down(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_DOWN)
	{
		State = STATE_DOWN;
		AnFrame = 0;
	}



	if (AnFrame < 10)
	{


	}



	else if (AnFrame > 20)
	{
		//if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-fTimeDelta*20);

	}
	else
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD);
	}




	AnFrame += fTimeDelta;
	Action = true;

	if (AnFrame >= 3)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD);
		m_bDead = true;
	}

}
CCone * CCone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCone*	pInstance = new CCone(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCone::Clone(void * pArg)
{
	CCone*	pInstance = new CCone(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCone::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(pGameInstance);
}
