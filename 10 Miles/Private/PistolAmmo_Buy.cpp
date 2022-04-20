#include "stdafx.h"
#include "..\Public\PistolAmmo_Buy.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "KeyMgr.h"
#include "SoundMgr.h"

CPistolAmmo_Buy::CPistolAmmo_Buy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPistolAmmo_Buy::CPistolAmmo_Buy(const CPistolAmmo_Buy & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPistolAmmo_Buy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CPistolAmmo_Buy::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_bButton = false;

	m_iPistolStock = rand() % 5 + 5;
	m_iMaxPistolStock= 10;

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 25, 8, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Scaled(_float3(36.f, 24.f, 0.f));
	
	return S_OK;
}

_int CPistolAmmo_Buy::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* pShopTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Shop"), TEXT("Com_Transform"));
	if (nullptr == pShopTransform)
		return E_FAIL;
	Safe_AddRef(pShopTransform);

	_float3 TargetPos = pShopTransform->Get_State(CTransform::STATE_POSITION);

	Safe_Release(pShopTransform);
	Safe_Release(pGameInstance);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(TargetPos.x + 101, TargetPos.y+2.f, 0.0001f));
	
	m_BuyRect = { _long(TargetPos.x + 83.f),_long(TargetPos.y - 10.f),_long(TargetPos.x + 119.f),_long(TargetPos.y +14.f) };

	m_rPistolStock = { _long(TargetPos.x + 725), _long(TargetPos.y + 255.f), 0, 0 };

	return 0;
}

void CPistolAmmo_Buy::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* pMousePos = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Mouse"), TEXT("Com_Transform"));
	if (nullptr == pMousePos)
		return;
	Safe_AddRef(pMousePos);

	CInventory* pShopping = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pShopping)
		return;
	Safe_AddRef(pShopping);

	_float3 MousePos = pMousePos->Get_State(CTransform::STATE_POSITION);

	POINT pt;
	pt.x = long(MousePos.x + 15.f);
	pt.y = long(MousePos.y);

	_int* PlayerCoin = pShopping->Get_ItemCount(CInventory::COIN);

	if (PtInRect(&m_BuyRect, pt))
	{
		m_bButton = true;
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (0 < m_iPistolStock && 50000 <= *PlayerCoin)
			{
				--m_iPistolStock;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::STORE_BUY);
				CSoundMgr::Get_Instance()->PlaySound(L"Buy&Sell1.wav", CSoundMgr::STORE_BUY, 0.7f);
				pShopping->Buy(CInventory::PISTOLAMMO, 10, 50000);
			}
			else
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Buy_Error1.wav", CSoundMgr::STORE_BUY, 0.3f);
			
			}
		}
	}
	else
		m_bButton = false;



	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);

	Safe_Release(pShopping);
	Safe_Release(pMousePos);
	Safe_Release(pGameInstance);
}

HRESULT CPistolAmmo_Buy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_bButton)))
		return E_FAIL;
	

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	

	_float4x4			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);	
	
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);	
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	if (true == m_bShop)
	{
		m_pVIBufferCom->Render();

		_tchar tPistolStock[MAX_PATH] = L"";
		wsprintf(tPistolStock, L"%d/%d", m_iPistolStock, m_iMaxPistolStock);

		m_pd3dxFont->DrawText(NULL, tPistolStock, -1, &m_rPistolStock, DT_NOCLIP, 0xffffffff);
	}

	return S_OK;
}

HRESULT CPistolAmmo_Buy::SetUp_Components()
{
	/* For.Com_Transform */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	/* For.Com_Renderer */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	
	/* For.Com_Texture */
	//Pistol
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Buy_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CPistolAmmo_Buy * CPistolAmmo_Buy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPistolAmmo_Buy*	pInstance = new CPistolAmmo_Buy(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPistolAmmo_Buy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPistolAmmo_Buy::Clone(void * pArg)
{
	CPistolAmmo_Buy*	pInstance = new CPistolAmmo_Buy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPistolAmmo_Buy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPistolAmmo_Buy::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);


}
