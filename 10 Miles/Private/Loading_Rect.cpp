#include "stdafx.h"
#include "..\Public\Loading_Rect.h"
#include "GameInstance.h"

CLoading_Rect::CLoading_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLoading_Rect::CLoading_Rect(const CLoading_Rect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoading_Rect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Rect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_iCnt = _int(rand() % 18);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 35, 10, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_rMsg1 = { 100, 648, 0, 0 };
	m_rMsg2 = { 100, 748, 0, 0 };


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-m_fSizeX*0.35f, 0.f, 0.0001f));
	m_pTransformCom->Scaled(_float3(275.f, 505.f, 0.0001f));

	return S_OK;
}

_int CLoading_Rect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

	

	
	
	
	return 0;
}

void CLoading_Rect::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CLoading_Rect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iCnt)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);	
	
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);	
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	m_pVIBufferCom->Render();

	//_tchar MSG_1_1[MAX_PATH] = L"ÆÀÀå : °­ÀÎÁß";
	//_tchar MSG_1_2[MAX_PATH] = L"¾Ö´Ï¸ÞÀÌ¼Ç";

	//_tchar MSG_2_1[MAX_PATH] = L"ÆÀ¿ø : ±èµ¿¾ð";
	//_tchar MSG_2_2[MAX_PATH] = L"Åø, Ãæµ¹";

	//_tchar MSG_3_1[MAX_PATH] = L"ÆÀ¿ø : ÃÖ¿µÈÆ";
	//_tchar MSG_3_2[MAX_PATH] = L"UI";

	//_tchar MSG_1[MAX_PATH] = (MSG_%d_1, rand() % 3);

	//m_pd3dxFont->DrawText(NULL, (MSG_%d,3), -1, &m_rMsg1, DT_NOCLIP, 0xffffffff);
	//m_pd3dxFont->DrawText(NULL, State_RifleAmmo, -1, &m_rMsg2, DT_NOCLIP, 0xffffffff);

	return S_OK;
}

HRESULT CLoading_Rect::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Rect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CLoading_Rect * CLoading_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_Rect*	pInstance = new CLoading_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLoading_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Rect::Clone(void * pArg)
{
	CLoading_Rect*	pInstance = new CLoading_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLoading_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
