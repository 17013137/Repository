#include "stdafx.h"
#include "Credit.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include "Level_Loading.h"
#include "SoundMgr.h"
#include "EventMgr.h"

CCredit::CCredit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCredit::CCredit(const CCredit & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCredit::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCredit::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY/2;
	m_fX -= g_iWinCX / 2.f;
	m_fY = g_iWinCY / 2.f;
	
	m_bCredit = false;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, -720.f, 0.0001f));
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);


	return S_OK;
}

_int CCredit::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CStatus*		pBossStatus = (CStatus*)pGameInstance->Get_Component(m_Current_Level, TEXT("Monster"), TEXT("Com_Status"));
	if (nullptr == pBossStatus)
		return E_FAIL;

	Safe_AddRef(pBossStatus);
	
	if (/*0>=pBossStatus->Get_pHp()*/pBossStatus->Get_State() == CStatus::STATUS_STATE_DEAD)
		++m_iCnt;
		
	if (10 == m_iCnt)
	{
		CSoundMgr::Get_Instance()->StopAll();
		m_bCredit = true;

		}
		
	if (true == m_bCredit)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"BGM2_mp3.mp3", CSoundMgr::BGM1, 0.4f);
		m_pTransformCom->Go_Y(0.1f);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > 719.9)
		{
			m_bStay = true;
			m_bCredit = false;
			CEventMgr::Get_Instance()->Set_OnCredit();
		}
	}
	Safe_Release(pBossStatus);
	Safe_Release(pGameInstance);
	return 0;
}

void CCredit::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pTransformCom->Scaled(_float3(1280, 2160.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CCredit::Render()
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
	if (true == m_bCredit||true == m_bStay)
	{
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

HRESULT CCredit::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_GameObject_Credit"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CCredit * CCredit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCredit*	pInstance = new CCredit(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCredit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCredit::Clone(void * pArg)
{
	CCredit*	pInstance = new CCredit(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCredit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCredit::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
