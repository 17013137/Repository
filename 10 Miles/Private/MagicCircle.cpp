#include "stdafx.h"
#include "..\Public\MagicCircle.h"
#include "GameInstance.h"
#include "Transform.h"

CMagicCircle::CMagicCircle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMagicCircle::CMagicCircle(const CMagicCircle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMagicCircle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagicCircle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	MyType = *(EFFECT3*)pArg;
	Safe_AddRef(MyType.TargetTransform);


	m_Texture = MyType.BulltType;

	m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.0f));
	m_pTransformCom->Scaled(_float3(0.1f, 0.1f, 0.1f));


	return S_OK;
}

_int CMagicCircle::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyType.TargetTransform->Get_State(CTransform::STATE_POSITION));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(10.f, 1.f, 10.f));
	
	if (m_Texture == 0)
	{
		m_pTransformCom->Go_Y(-0.2);
		if (Page == 0)
		{
			m_pTransformCom->Scaling(fTimeDelta*7.f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				Page = 1;
				Delay = MyType.Delay;
			}
		}
		else if (Page == 1)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta / 5);
			if (Delay < 0)
			{
				Page = 2;
				Delay = 1;
			}
		}
		else if (Page == 2)
		{
			m_pTransformCom->Scaling(-fTimeDelta * 10.5f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				return 1;
			}
		}
	}
	else if (m_Texture == 1)
	{
		//m_pTransformCom->Go_Y(0.2);
		if (Page == 0)
		{
			m_pTransformCom->Scaling(fTimeDelta*2.5f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				Page = 1;
				Delay = MyType.Delay;
			}
		}
		else if (Page == 1)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta / 5);
			if (Delay < 0)
			{
				Page = 2;
				Delay = 1;
			}
		}
		else if (Page == 2)
		{
			m_pTransformCom->Scaling(-fTimeDelta * 3.75f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				return 1;
			}
		}
	}
	else if (m_Texture == 2)
	{
		if (Page == 0)
		{
			m_pTransformCom->Scaling(fTimeDelta*4.f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				Page = 1;
				Delay = MyType.Delay;
			}
		}
		else if (Page == 1)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta / 5);
			if (Delay < 0)
			{
				Page = 2;
				Delay = 1;
			}
		}
		else if (Page == 2)
		{
			m_pTransformCom->Scaling(-fTimeDelta * 6.f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				return 1;
			}
		}
	}
	else if (m_Texture == 5)
	{
		m_pTransformCom->Go_Y(-0.2);
		if (Page == 0)
		{
			m_pTransformCom->Scaling(fTimeDelta*7.f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				Page = 1;
				Delay = MyType.Delay;
			}
		}
		else if (Page == 1)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta / 5);
			if (Delay < 0)
			{
				Page = 2;
				Delay = 1;
			}
		}
		else if (Page == 2)
		{
			m_pTransformCom->Scaling(-fTimeDelta * 10.5f);
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
			if (Delay < 0)
			{
				return 1;
			}
		}
	}

	Delay -= fTimeDelta;


	__super::Tick(fTimeDelta);

	return 0;
}

void CMagicCircle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}
HRESULT CMagicCircle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_Texture)))
		return E_FAIL;
	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();

	return S_OK;
}



HRESULT CMagicCircle::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
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

	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_MagicCircle"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CMagicCircle * CMagicCircle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMagicCircle*	pInstance = new CMagicCircle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CasdfTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMagicCircle::Clone(void * pArg)
{
	CMagicCircle*	pInstance = new CMagicCircle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created asdCMagicCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMagicCircle::Free()
{
	__super::Free();

	Safe_Release(MyType.TargetTransform);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
