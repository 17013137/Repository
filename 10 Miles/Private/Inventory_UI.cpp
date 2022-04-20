#include "stdafx.h"
#include "..\Public\Inventory_UI.h"
#include "GameInstance.h"
#include "PlayerCube.h"

CInventory_UI::CInventory_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CInventory_UI::CInventory_UI(const CInventory_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CInventory_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory_UI::NativeConstruct(void * pArg)
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

	CInventory* pPlayerInventory = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));
	if (nullptr == pPlayerInventory)
		return E_FAIL;
	Safe_AddRef(pPlayerInventory);

	m_iMaxPistolAmmo = pPlayerInventory->Get_ItemCount(CInventory::PISTOLAMMO);
	m_iPistolAmmo = pPlayerInventory->Get_bulletCount(CInventory::PISTOLAMMO);

	m_iMaxRifleAmmo = pPlayerInventory->Get_ItemCount(CInventory::RIFLEAMMO);
	m_iRifleAmmo = pPlayerInventory->Get_bulletCount(CInventory::RIFLEAMMO);

	m_iMaxShotGunAmmo = pPlayerInventory->Get_ItemCount(CInventory::SHOTGUNAMMO);
	m_iShotGunAmmo = pPlayerInventory->Get_bulletCount(CInventory::SHOTGUNAMMO);

	m_iPainKiller = pPlayerInventory->Get_ItemCount(CInventory::MEDICKIT);

	m_iGranade = pPlayerInventory->Get_ItemCount(CInventory::GRENADE);

	m_iCoin = pPlayerInventory->Get_ItemCount(CInventory::COIN);


	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 30, 9, FW_BOLD, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	m_bInventory = false;
	m_btemporarily = false;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-(m_fSizeX*0.61f), 30.f, 0.0001f));
	m_pTransformCom->Scaled(_float3(m_fSizeX*0.22f, m_fSizeY*0.92f, 0.0001f));

	Safe_Release(pPlayerInventory);
	Safe_Release(pGameInstance);

	return S_OK;
}

_int CInventory_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_Current_Level != LEVEL_STORE)
	{
		if (GetAsyncKeyState(VK_TAB) && 0X0001)
		{
			m_bTab = true;
		}
	}
	if (m_Current_Level == LEVEL_STORE)
	{
			if (GetAsyncKeyState('T') && 0x0001)
			{
				m_bTab = true;
			}
	}

	if (GetAsyncKeyState(VK_RSHIFT) && 0x0001)
	{
		m_btemporarily = true;
	}

	Set_Inventory();
	
	m_rPainKiller = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 700.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 74.f),0,0 };
	m_rShotGunAmmo = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 700.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 136.f),0,0 };
	m_rRifleAmmo = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 700.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 105.f),0,0 };
	m_rPistolAmmo = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 700.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 167.f),0,0 };
	m_rGranade = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 700.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 198.f),0,0 };
	m_rCoin = { long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x + 680.f),long(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 593.f),0,0 };

	return 0;
}

void CInventory_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
	
}

HRESULT CInventory_UI::Render()
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

	_tchar Inventory_Pistol[MAX_PATH] = L"";
	wsprintf(Inventory_Pistol, L"%d/%d", *m_iPistolAmmo, *m_iMaxPistolAmmo);

	_tchar Inventory_Rifle[MAX_PATH] = L"";
	wsprintf(Inventory_Rifle, L"%d/%d", *m_iRifleAmmo, *m_iMaxRifleAmmo);

	_tchar Inventory_ShotGun[MAX_PATH] = L"";
	wsprintf(Inventory_ShotGun, L"%d/%d", *m_iShotGunAmmo, *m_iMaxShotGunAmmo);

	_tchar Inventory_PainKiller[MAX_PATH] = L"";
	wsprintf(Inventory_PainKiller, L"x%d", *m_iPainKiller);

	_tchar Inventory_Granade[MAX_PATH] = L"";
	wsprintf(Inventory_Granade, L"x%d", *m_iGranade);

	_tchar Inventory_Coin[MAX_PATH] = L"";
	wsprintf(Inventory_Coin, L"%d¿ø", *m_iCoin);


	m_pd3dxFont->DrawText(NULL, Inventory_PainKiller, -1, &m_rPainKiller, DT_NOCLIP, 0xffffffff);
	m_pd3dxFont->DrawText(NULL, Inventory_ShotGun, -1, &m_rShotGunAmmo, DT_NOCLIP, 0xffffffff);
	m_pd3dxFont->DrawText(NULL, Inventory_Rifle, -1, &m_rRifleAmmo, DT_NOCLIP, 0xffffffff);
	m_pd3dxFont->DrawText(NULL, Inventory_Pistol, -1, &m_rPistolAmmo, DT_NOCLIP, 0xffffffff);
	m_pd3dxFont->DrawText(NULL, Inventory_Granade, -1, &m_rGranade, DT_NOCLIP, 0xffffffff);
	m_pd3dxFont->DrawText(NULL, Inventory_Coin, -1, &m_rCoin, DT_NOCLIP, 0xffffffff);

	return S_OK;
}

HRESULT CInventory_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Inventory_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CInventory_UI::Set_Inventory()
{
	if (true == m_bTab|| true == m_btemporarily)
	{
		if (false == m_bInventory)
		{
			m_pTransformCom->Go_X(20.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > -(m_fSizeX*0.395f))
			{
				m_bTab = false;
				m_btemporarily = false;
				m_bInventory = true;
			}
		}
			else
			{
				m_pTransformCom->Go_X(-20.f);
				if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < -(m_fSizeX*0.605f))
				{
					m_bInventory = false;
					m_btemporarily = false;
					m_bTab = false;
				}
			}
	}
}


CInventory_UI * CInventory_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory_UI*	pInstance = new CInventory_UI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CInventory_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInventory_UI::Clone(void * pArg)
{
	CInventory_UI*	pInstance = new CInventory_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CInventory_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);


}
