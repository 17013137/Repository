#include "stdafx.h"
#include "..\Public\Effect_Action.h"
#include "GameInstance.h"
#include  <time.h>

CEffect_Action::CEffect_Action(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CEffect_Action::CEffect_Action(const CEffect_Action & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CEffect_Action::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Action::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	LoopTime = 0;
	m_Effect_Type = *(EFFECTTYPE*)pArg;

	switch (m_Effect_Type.Type)
	{
	case Client::CEffect_Action::TYPE_FIRE:
		MinfFrame = 0;
		MaxfFrame = 31;
		//m_fFrame = MinfFrame + rand() % MaxfFrame;
		LoopTime = MaxfFrame - MinfFrame;
		Loop = 8;
		Move = _float3(0.f, -0.001f, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos);
		m_pTransformCom->Go_Y(3.f);
		m_pTransformCom->Scaled(_float3(1.5f, 1.5f, 1.5f));
		break;
	case Client::CEffect_Action::TYPE_SMOKE:
		MinfFrame = 32;
		MaxfFrame = 43;
		//m_fFrame = MinfFrame + rand() % MaxfFrame;
		LoopTime = MaxfFrame - MinfFrame;
		Loop = 5;
		Move = _float3(0.f, 0.01f, 0.f);
		m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos + Move * 70);
		break;
	case Client::CEffect_Action::TYPE_BUBBLE:
		MinfFrame = 44;
		MaxfFrame = 49;
		LoopTime = MaxfFrame - MinfFrame;
		Loop = 1;
		Move = _float3(0.f, 0.01f, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos + Move * 70);
		break;
	case Client::CEffect_Action::TYPE_CRACK:
		MinfFrame = 50;
		MaxfFrame = 59;
		LoopTime = MaxfFrame - MinfFrame;
		Loop = 1;
		m_pTransformCom->Go_Y(3.f);
		Move = _float3(0.f, 0.0f, 0.f);
		DelayTime = 50;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos);
		
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(rand() % 90));
		m_pTransformCom->Scaled(_float3(5.f, 5.f, 5.f));
		break;
	case Client::CEffect_Action::TYPE_BOTTOMEXPLOSION:
		MinfFrame = 60;
		MaxfFrame = 71;
		LoopTime = MaxfFrame - MinfFrame;
		Loop = 1;
		Move = _float3(0.f, 0.0f, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Effect_Type.Pos);
		m_pTransformCom->Go_Y(0.2f);
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
		m_pTransformCom->Scaled(_float3(5.f, 5.f, 5.f));
		break;
	case Client::CEffect_Action::TYPE_END:
		break;
	default:
		break;
	}

	m_fFrame = MinfFrame;


	return S_OK;
}

_int CEffect_Action::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;

	__super::Tick(fTimeDelta);
	CGameInstance*			pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + Move);
	if (m_Effect_Type.Type == TYPE_FIRE)
	{
		_float3 Scale = m_pTransformCom->Get_Scale();
		Scale.x = Scale.x*0.997f;
		Scale.z = Scale.z*0.997f;
		Scale.y = Scale.z*0.997f;
		m_pTransformCom->Scaled(Scale);

		if (rand() % 200 > 196 && Loop > 3)
		{
			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_Effect_Type.Type = TYPE_SMOKE;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &m_Effect_Type)))
				return 0;

		}
	}
	else if (m_Effect_Type.Type == TYPE_SMOKE)
	{
		_float3 Scale = m_pTransformCom->Get_Scale();
		m_pTransformCom->Scaled(Scale*0.995f);
	}
	else if (m_Effect_Type.Type == TYPE_BUBBLE)
	{

	}
	else if (m_Effect_Type.Type == TYPE_CRACK)
	{

	}
	else if (m_Effect_Type.Type == TYPE_BOTTOMEXPLOSION)
	{
		
	}

	m_fFrame += LoopTime * fTimeDelta;

	if (m_Effect_Type.Type == TYPE_CRACK)
	{
		m_fFrame += LoopTime * fTimeDelta * 5;
	}
	if (m_fFrame >= MaxfFrame && Loop > 0)
	{
		m_fFrame = MinfFrame;
		--Loop;
	}

	if (m_fFrame >= MaxfFrame)
		return 1;

	Safe_Release(pGameInstance);
	return 0;
}

void CEffect_Action::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_Effect_Type.Type != TYPE_CRACK && m_Effect_Type.Type != TYPE_BOTTOMEXPLOSION)
	{
		_float4x4		ViewMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

		_float3		vScale = m_pTransformCom->Get_Scale();

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);
	}

	

	else if (Loop <= 0 && m_Effect_Type.Type != TYPE_CRACK)
	{
		m_bDead = true;
	}
	else if (Loop <= 0 && m_Effect_Type.Type == TYPE_CRACK)
	{
		if (DelayTime > 0 )
		{
			m_fFrame = MaxfFrame;
			--DelayTime;
		}
		else if (DelayTime <= 0 && m_fFrame-1 > MinfFrame)
		{
			m_fFrame -= (LoopTime * fTimeDelta)*6 + (LoopTime * fTimeDelta)/2;
		}
		else if (m_fFrame-1 <= MinfFrame)
		{
			m_bDead = true;
		}
		
	}



	Compute_CamDistance(m_pTransformCom);

	if(m_Effect_Type.Type==TYPE_FIRE)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

	
}

HRESULT CEffect_Action::Render()
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

HRESULT CEffect_Action::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Effect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CEffect_Action * CEffect_Action::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Action*	pInstance = new CEffect_Action(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CEffect_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Action::Clone(void * pArg)
{
	CEffect_Action*	pInstance = new CEffect_Action(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CEffect_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Action::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
