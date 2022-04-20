#include "stdafx.h"
#include "Loading_Cube.h"
#include "GameInstance.h"

CLoading_Cube::CLoading_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLoading_Cube::CLoading_Cube(const CLoading_Cube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoading_Cube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Cube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Scaled(_float3(0.6f, 0.6f, 0.6f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(10.f, -5.3f, 10.f));


	return S_OK;
}

_int CLoading_Cube::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f),fTimeDelta);
	m_pTransformCom->Turn(_float3(0.f, 1.f, 1.f), fTimeDelta);
	m_pTransformCom->Turn(_float3(0.f, 0.f, 1.f), fTimeDelta);
	m_pTransformCom->Turn(_float3(1.f, 0.f, 1.f), fTimeDelta);
	m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), fTimeDelta);
	

	return 0;
}

void CLoading_Cube::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CLoading_Cube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(3)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();

	return S_OK;
}


HRESULT CLoading_Cube::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

CLoading_Cube * CLoading_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_Cube*	pInstance = new CLoading_Cube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLoading_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Cube::Clone(void * pArg)
{
	CLoading_Cube*	pInstance = new CLoading_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLoading_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
