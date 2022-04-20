#include "stdafx.h"
#include "..\Public\Shop_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "SoundMgr.h"

CShop_UI::CShop_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CShop_UI::CShop_UI(const CShop_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShop_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CShop_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_bShopOpen = false;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3((m_fSizeX*0.61f), 30.f, 0.0001f));
	m_pTransformCom->Scaled(_float3(m_fSizeX*0.22f, m_fSizeY*0.92f, 0.0001f));

	return S_OK;
}

_int CShop_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (m_Current_Level == LEVEL_STORE)

	{
		if (GetAsyncKeyState('T') && 0X0001)
		{
			m_bTab = true;
		}
	}
	Set_Shop();

	return 0;
}

void CShop_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
	
}

HRESULT CShop_UI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	

	_float4x4			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);	
	
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);	
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	if (m_Current_Level == LEVEL_STORE)
	{
		m_pVIBufferCom->Render();
	}
	
	return S_OK;
}

HRESULT CShop_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Shop_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CShop_UI::Set_Shop()
{
	if (true == m_bTab)
	{
		if (false == m_bShopOpen)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Shop_Open.wav", CSoundMgr::STORE_ONOFF, 0.4f);
			m_pTransformCom->Go_X(-20.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < (m_fSizeX*0.395f))
			{
				m_bTab = false;
				m_bShopOpen = true;
			}
		}
		else
		{
			m_pTransformCom->Go_X(20.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >(m_fSizeX*0.605f))
			{
				m_bTab = false;
				m_bShopOpen = false;
			}
		}
	}
}

CShop_UI * CShop_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShop_UI*	pInstance = new CShop_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CShop_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShop_UI::Clone(void * pArg)
{
	CShop_UI*	pInstance = new CShop_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CShop_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);


}
