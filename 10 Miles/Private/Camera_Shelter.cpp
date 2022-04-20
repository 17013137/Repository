#include "stdafx.h"
#include "..\Public\Camera_Shelter.h"
#include "GameInstance.h"
#include "KeyMgr.h"

CCamera_Shelter::CCamera_Shelter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Shelter::CCamera_Shelter(const CCamera_Shelter & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Shelter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Shelter::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_pEventmgr = CEventMgr::Get_Instance();
	m_pEventmgr->Initialize(m_Current_Level, m_pTransform);
	return S_OK;
}

_int CCamera_Shelter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pEventmgr->Tick(fTimeDelta);

	if (m_pEventmgr->Get_OnAir() == true) {
		switch (m_pEventmgr->Get_EventCnt())
		{
		case 0:
			Event_Scean0(fTimeDelta);
			break;
		case 1:
			Event_Scean1(fTimeDelta);
			break;
		case 4:
			Event_Scean2(fTimeDelta);
			break;
		case 5:
			Event_Scean3(fTimeDelta);
			break;
		case 8:
			Event_Scean4(fTimeDelta);
			break;
		default:
			m_pEventmgr->Off_Air();
			break;
		}
		return 0;
	}

	if (Get_PlayerPos() == true)
		Lookat(fTimeDelta);

	return 0;
}

void CCamera_Shelter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CCamera_Shelter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

_bool CCamera_Shelter::Get_PlayerPos()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return false;
	Safe_AddRef(pPlayerTransform);

	m_PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	return true;
}

void CCamera_Shelter::Lookat(_float fTimeDelta)
{

	_float3 Camera_Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vDir = m_PlayerPos - Camera_Pos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float3 temp = Camera_Pos = m_pTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&temp, &temp);

	if (temp.x != vDir.x)
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * (vDir.x - temp.x));

	if (temp.z != vDir.z)
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * (vDir.z - temp.z));

}

void CCamera_Shelter::Event_Scean0(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time()< 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Ready_Scean(L"Layer_Grandfa");
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Set_isInitSet(0, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*0.01f);

	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x + 5.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 15.f && m_pEventmgr->Get_Time() < 20.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 20.f) {
		m_pEventmgr->Off_Air();
	}


}

void CCamera_Shelter::Event_Scean1(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time()< 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Ready_Scean(L"Layer_Grandfa");
			m_pEventmgr->Set_isInitSet(0, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*0.01f);

	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x + 5.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 15.f && m_pEventmgr->Get_Time() < 20.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 20.f) {
		m_pEventmgr->Off_Air();
	}

}

void CCamera_Shelter::Event_Scean2(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time()< 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Ready_Scean(L"Layer_Grandfa");
			m_pEventmgr->Set_isInitSet(0, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*0.01f);

	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x + 5.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 10.f) {
		m_pEventmgr->Off_Air();
	}
}

void CCamera_Shelter::Event_Scean3(_float fTimeDelta)
{

	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time()< 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Ready_Scean(L"Layer_Grandfa");
			m_pEventmgr->Ready_Scean(L"Layer_Steve");
			m_pEventmgr->Set_isInitSet(0, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x + 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 7.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x + 5.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}


	if (m_pEventmgr->Get_Time() >= 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 15.f && m_pEventmgr->Get_Time() < 20.f) { //어서 여길 빠져나가야 해!
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x + 3.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z));

		}
		m_pTransform->Go_X(-fTimeDelta*0.015f);
	}

	if (m_pEventmgr->Get_Time() >= 20.f && m_pEventmgr->Get_Time() < 25.f) {
		if (m_pEventmgr->Get_isInitSet(4) == false) {
			m_pEventmgr->Set_isInitSet(4, true);
		}
		m_pTransform->Go_X(-fTimeDelta*0.015f);
	}

	if (m_pEventmgr->Get_Time() >= 25.f && m_pEventmgr->Get_Time() < 30.f) {
		if (m_pEventmgr->Get_isInitSet(5) == false) {
			m_pEventmgr->Set_isInitSet(5, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x + 3.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 3.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_Z(-fTimeDelta*0.015f);
	}
	if (m_pEventmgr->Get_Time() >= 30.f && m_pEventmgr->Get_Time() < 35.f) {
		if (m_pEventmgr->Get_isInitSet(6) == false) {
			m_pEventmgr->Set_isInitSet(6, true);
		}
		m_pTransform->Go_Z(-fTimeDelta*0.015f);
	}

	if (m_pEventmgr->Get_Time() >= 40.f && m_pEventmgr->Get_Time() < 45.f) {
		if (m_pEventmgr->Get_isInitSet(7) == false) {
			m_pEventmgr->Set_isInitSet(7, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z - 1.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 45.f) {
		m_pEventmgr->Off_Air();
	}

}

void CCamera_Shelter::Event_Scean4(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);
	if (m_pEventmgr->Get_Time()< 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Set_isInitSet(0, true);
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pEventmgr->Ready_Scean(L"Layer_Grandfa");
			m_pEventmgr->Ready_Scean(L"Layer_Steve");
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x - 3.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z - 7.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(2)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(2)).ActorPos.z));
		}
		m_pTransform->Go_X(fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
		}
		m_pTransform->Go_X(fTimeDelta*0.01f);
	}

	if (m_pEventmgr->Get_Time() >= 10.f && m_pEventmgr->Get_Time() < 15.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
		}
	}

	if (m_pEventmgr->Get_Time() >= 15.f) {
		m_pEventmgr->Off_Air();
	}
}


CGameObject * CCamera_Shelter::Clone(void * pArg)
{
	CCamera_Shelter*	pInstance = new CCamera_Shelter(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Shelter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Shelter::Free()
{
	__super::Free();

}

CCamera_Shelter * CCamera_Shelter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Shelter*	pInstance = new CCamera_Shelter(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Shelter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}