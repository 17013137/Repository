#include "stdafx.h"
#include "..\Public\Camera_Logo.h"
#include "GameInstance.h"
#include "KeyMgr.h"

CCamera_Logo::CCamera_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Logo::CCamera_Logo(const CCamera_Logo & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Logo::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Logo::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	

	return S_OK;
}

_int CCamera_Logo::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	__super::Tick(fTimeDelta);
	Safe_Release(pGameInstance);

	return 0;
}

void CCamera_Logo::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CCamera_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Logo * CCamera_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Logo*	pInstance = new CCamera_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Logo::Clone(void * pArg)
{
	CCamera_Logo*	pInstance = new CCamera_Logo(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Logo::Free()
{
	__super::Free();

}
