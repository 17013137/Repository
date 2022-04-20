#include "stdafx.h"
#include "..\Public\Scene_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "EventMgr.h"

CScene_UI::CScene_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CScene_UI::CScene_UI(const CScene_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CScene_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_bTab = false;
	m_bScene = false;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f,500.f, 0.0001f));
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY*0.4f, 0.0001f));


	Safe_Release(pGameInstance);

	return S_OK;
}

_int CScene_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Set_Scene();
	return 0;
}

void CScene_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);

}

HRESULT CScene_UI::Render()
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
	
	m_pVIBufferCom->Render();

	
	return S_OK;
}

HRESULT CScene_UI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Scene_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CScene_UI::Set_Scene()
{
	if (true == CEventMgr::Get_Instance()->Get_OnAir())
	{
		if (false == m_bScene)
		{
			m_pTransformCom->Go_Y(-5.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 350)
			{
				m_bScene = true;
			}
		}
	}
		else
		{
			if (true == m_bScene)
			{
				m_pTransformCom->Go_Y(5.f);
				if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > 500)
				{
					m_bScene = false;
				}
			}
		}
}


CScene_UI * CScene_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_UI*	pInstance = new CScene_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CScene_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScene_UI::Clone(void * pArg)
{
	CScene_UI*	pInstance = new CScene_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CScene_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
