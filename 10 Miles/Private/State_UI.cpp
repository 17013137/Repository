#include "stdafx.h"
#include "..\Public\State_UI.h"
#include "GameInstance.h"
#include "EventMgr.h"

CState_UI::CState_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CState_UI::CState_UI(const CState_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CState_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CState_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CInventory* pPlayerConsumableItem = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pPlayerConsumableItem)
		return E_FAIL;
	Safe_AddRef(pPlayerConsumableItem);

	m_iPainKiller = pPlayerConsumableItem->Get_ItemCount(CInventory::MEDICKIT);

	m_iGranade = pPlayerConsumableItem->Get_ItemCount(CInventory::GRENADE);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 25, 8, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_rPainKiller = { 378,695,0,0 };

	m_rGranade = { 900,695,0,0 };

	Safe_Release(pPlayerConsumableItem);
	Safe_Release(pGameInstance);

	return S_OK;
}

_int CState_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTransformCom->Scaled(_float3(m_fSizeX*0.5f, m_fSizeY*0.1f, 0.0002f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, -m_fSizeY*0.45f + 0.f, 0.0002f));

	return 0;
}

void CState_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CState_UI::Render()
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
	/*if (false == CEventMgr::Get_Instance()->Get_OnAir()&&false == CEventMgr::Get_Instance()->Get_OnCredit())
	{
		_tchar State_PainKiller[MAX_PATH] = L"";
		wsprintf(State_PainKiller, L"x%d", *m_iPainKiller);

		_tchar State_Granade[MAX_PATH] = L"";
		wsprintf(State_Granade, L"x%d", *m_iGranade);

		m_pd3dxFont->DrawText(NULL, State_PainKiller, -1, &m_rPainKiller, DT_NOCLIP, 0xffffffff);
		m_pd3dxFont->DrawText(NULL, State_Granade, -1, &m_rGranade, DT_NOCLIP, 0xffffffff);
	}*/
	return S_OK;
}

HRESULT CState_UI::SetUp_Components()
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

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_State_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CState_UI * CState_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CState_UI*	pInstance = new CState_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CState_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CState_UI::Clone(void * pArg)
{
	CState_UI*	pInstance = new CState_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CState_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
