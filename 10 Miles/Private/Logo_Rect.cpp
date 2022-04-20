#include "stdafx.h"
#include "..\Public\Logo_Rect.h"
#include "GameInstance.h"
#include "SoundMgr.h"

CLogo_Rect::CLogo_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLogo_Rect::CLogo_Rect(const CLogo_Rect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLogo_Rect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Rect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_fFrame = 0.f;
	MaxfFrame = 36.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.0f));
	m_pTransformCom->Scaled(_float3(1280.f, 1280.f, 0.0f));
	return S_OK;
}

_int CLogo_Rect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//Safe_AddRef(pGameInstance);
	
	if (m_fFrame >= MaxfFrame-1.f)
	{
		m_fFrame = MaxfFrame - 1.f;
		//pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SHELTER));
		//return 1;
	}
	if (17 == (_uint)m_fFrame&&17.081f>m_fFrame)
		CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.4f);

	if (22 == (_uint)m_fFrame&&22.081f>m_fFrame)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
		CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.4f);
	}

	if (27 == (_uint)m_fFrame&&27.081f>m_fFrame)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
		CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.4f);
	}

	if (32 == (_uint)m_fFrame&&32.081f>m_fFrame)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
		CSoundMgr::Get_Instance()->PlaySound(L"ShotGun_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.4f);
	}

	/*Safe_Release(pGameInstance);*/

	return 0;
}

void CLogo_Rect::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_fFrame += 6.f * fTimeDelta;
	
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CLogo_Rect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
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

HRESULT CLogo_Rect::SetUp_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Logo_Rect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CLogo_Rect * CLogo_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLogo_Rect*	pInstance = new CLogo_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLogo_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLogo_Rect::Clone(void * pArg)
{
	CLogo_Rect*	pInstance = new CLogo_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLogo_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
