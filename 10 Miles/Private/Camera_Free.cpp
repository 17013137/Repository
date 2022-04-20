#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"


CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_pEventmgr = CEventMgr::Get_Instance();
	m_pEventmgr->Initialize(m_Current_Level, m_pTransform);

	return S_OK;
}

_int CCamera_Free::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pEventmgr->Tick(fTimeDelta);
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pEventmgr->Get_OnAir() == true) {
		if (m_pEventmgr->Get_EventCnt() == 2) {
			Event_Scean0(fTimeDelta);
			return 0;
		}
		if (m_pEventmgr->Get_EventCnt() == 6) {
			Event_Scean1(fTimeDelta);
			return 0;
		}
	}
	Safe_Release(pGameInstance);

	return 0;
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Free::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Event_Scean0(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time() < 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Ready_Scean(L"Layer_NPC");
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Set_isInitSet(0, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 5.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 8.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_X(fTimeDelta*0.03f);
	}

	if (m_pEventmgr->Get_Time() >= 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 2.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
	}
	if (m_pEventmgr->Get_Time() >= 15.f && m_pEventmgr->Get_Time() < 20.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
	}


	if (m_pEventmgr->Get_Time() >= 20.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false)
			m_pEventmgr->Set_isInitSet(3, true);
		m_pEventmgr->Off_Air();
	}

}

void CCamera_Free::Event_Scean1(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time() < 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Set_isInitSet(0, true);
			m_pEventmgr->Ready_Scean(L"Layer_NPC");
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 15.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 10.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 3.0f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_X(fTimeDelta*0.02f);
	}

	if (m_pEventmgr->Get_Time() > 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 2.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_X(-fTimeDelta*0.02f);
	}

	if (m_pEventmgr->Get_Time() > 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z + 2.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Go_X(-fTimeDelta*0.02f);
	}

	if (m_pEventmgr->Get_Time() > 15.f && m_pEventmgr->Get_Time() < 20.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z + 2.f));
		}
		m_pTransform->Go_X(-fTimeDelta*0.02f);
		m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
	}

	if (m_pEventmgr->Get_Time() > 20.f && m_pEventmgr->Get_Time() < 25.f) {
		if (m_pEventmgr->Get_isInitSet(4) == false) {
			m_pEventmgr->Set_isInitSet(4, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z + 2.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Go_X(-fTimeDelta*0.02f);
	}

	if (m_pEventmgr->Get_Time() > 25.f && m_pEventmgr->Get_Time() < 30.f) {
		if (m_pEventmgr->Get_isInitSet(5) == false) {
			m_pEventmgr->Set_isInitSet(5, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z + 2.f));
		}
		m_pTransform->Go_X(-fTimeDelta*0.02f);
		m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
	}

	if (m_pEventmgr->Get_Time() > 30.f && m_pEventmgr->Get_Time() < 35.f) {
		if (m_pEventmgr->Get_isInitSet(6) == false) {
			m_pEventmgr->Set_isInitSet(6, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 2.0f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 1.f));
		}
		m_pTransform->Go_Left(-fTimeDelta*0.05f);
		m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
	}

	if (m_pEventmgr->Get_Time() > 35.f && m_pEventmgr->Get_Time() < 40.f) {
		if (m_pEventmgr->Get_isInitSet(7) == false) {
			m_pEventmgr->Set_isInitSet(7, true);
		}
	}

	if (m_pEventmgr->Get_Time() > 40.f) {
		m_pEventmgr->Off_Air();
	}
}

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*	pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*	pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
