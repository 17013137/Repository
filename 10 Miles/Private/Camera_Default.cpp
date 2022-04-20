#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include "EventMgr.h"

CCamera_Default::CCamera_Default(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	ReturnView = false;

	m_pEventmgr = CEventMgr::Get_Instance();
	m_pEventmgr->Initialize(m_Current_Level, m_pTransform);
	return S_OK;
}

_int CCamera_Default::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (true == m_bDead)
		return 1;

	m_pEventmgr->Tick(fTimeDelta);

	if (m_pEventmgr->Get_OnAir() == true) {
		if (m_Current_Level == LEVEL_BOSS)
			Boss_Event_Scean(fTimeDelta);
		else
			Stage_Event_Scean(fTimeDelta);
		return 0;
	}

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (false == ReturnView)
	{
		SetUp_CameraMoveTopView();
	}

	Safe_Release(pGameInstance);

	if (true == ReturnView)
	{
		SetUp_CameraMoveToMouse();
	}

	return 0;
}

void CCamera_Default::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::SetUp_CameraMoveTopView()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return E_FAIL;
	Safe_AddRef(pPlayerTransform);
	_float3 PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_float3 CameraPos = PlayerPos + ThirdPos;

	m_pTransform->Set_State(CTransform::STATE_POSITION, CameraPos);
	m_pTransform->LookAt(PlayerPos);

	pPlayerTransform->LookAt(pPlayerTransform->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 0.f, -1.f));


	ReturnView = true;
	Safe_Release(pPlayerTransform);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CCamera_Default::SetUp_CameraMoveToMouse()
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_Terrain*		pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pVIBuffer_Terrain)
		return E_FAIL;
	Safe_AddRef(pVIBuffer_Terrain);
	CTransform*			pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTransform_Terrain)
		return E_FAIL;
	Safe_AddRef(pTransform_Terrain);
	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return E_FAIL;
	Safe_AddRef(pPlayerTransform);

	pVIBuffer_Terrain->Picking(pTransform_Terrain, &m_vMousePos);

	_float3 PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 CameraPos;
	fRadius = sqrt((PlayerPos.x - m_vMousePos.x)*(PlayerPos.x - m_vMousePos.x) + (PlayerPos.z - m_vMousePos.z)*(PlayerPos.z - m_vMousePos.z));

	if (20.f > fRadius)
	{
		CameraPos = _float3(PlayerPos.x + (m_vMousePos.x - PlayerPos.x)*0.2f, PlayerPos.y + 15.f, PlayerPos.z + (m_vMousePos.z - PlayerPos.z)*0.2f - 12.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, CameraPos);
	}
	else
	{
		_float3 PlayerLook = _float3(0.f, 0.f, 0.f);
		PlayerLook.x = m_vMousePos.x - PlayerPos.x;
		PlayerLook.z = m_vMousePos.z - PlayerPos.z;

		PlayerLook = *D3DXVec3Normalize(&PlayerLook, &PlayerLook)*4.1f;



		CameraPos = _float3(PlayerPos.x + PlayerLook.x, PlayerPos.y + 15.f, PlayerPos.z + PlayerLook.z - 12.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, CameraPos);

	}
	m_pTransform->LookAt(_float3(CameraPos.x, PlayerPos.y, CameraPos.z + 12.f));

	Safe_Release(pPlayerTransform);
	Safe_Release(pTransform_Terrain);
	Safe_Release(pVIBuffer_Terrain);
	Safe_Release(pGameInstance);

	return S_OK;
}

void CCamera_Default::Boss_Event_Scean(_float fTimeDelta)
{
	m_Time += fTimeDelta;

	if (m_Time < 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Set_isInitSet(0, true);
			m_pEventmgr->Ready_Scean(L"Monster");
			m_pEventmgr->Ready_Scean(L"Layer_Dummy");
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 4.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*0.03f);
	}

	if (m_Time > 5.f && m_Time < 10.f) {
		if (m_pEventmgr->Get_isInitSet(2) == false) {
			m_pEventmgr->Set_isInitSet(2, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 2.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.5f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z + 10.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 1.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta*0.03f);
	}

	if (m_Time > 10.f && m_Time < 15.f) {
		if (m_pEventmgr->Get_isInitSet(3) == false) {
			m_pEventmgr->Set_isInitSet(3, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(1)).ActorPos.x - 5.f, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.y, (*m_pEventmgr->Get_ActorInfo(1)).ActorPos.z - 5.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_X(fTimeDelta*0.03f);
	}
	if (m_Time > 15.f && m_Time < 20.f) {
		if (m_pEventmgr->Get_isInitSet(4) == false) {
			m_pEventmgr->Set_isInitSet(4, true);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x - 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 2.5f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z - 5.f));
			m_pTransform->LookAt(_float3((*m_pEventmgr->Get_ActorInfo(0)).ActorPos.x, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.y + 5.f, (*m_pEventmgr->Get_ActorInfo(0)).ActorPos.z));
		}
		m_pTransform->Go_Straight(fTimeDelta*0.03f);
	}



	if (m_Time > 20.f) {
		m_pEventmgr->Off_Air();
		m_Time = 0.f;
	}
}

void CCamera_Default::Stage_Event_Scean(_float fTimeDelta)
{
	m_pEventmgr->Add_Time(fTimeDelta);

	if (m_pEventmgr->Get_Time() < 5.f) {
		if (m_pEventmgr->Get_isInitSet(0) == false) {
			m_pEventmgr->Set_isInitSet(0, true);
			m_pGraphic_Device->LightEnable(0, FALSE);
			m_pGraphic_Device->LightEnable(2, FALSE);
			m_pGraphic_Device->SetRenderState(D3DRS_AMBIENT, 0x00505050);
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 15.f, -10.f));
			m_pTransform->LookAt(_float3(40.f, 0.f, 40.f));
		}
		m_pTransform->Go_Z(fTimeDelta*0.3f);
	}

	if (m_pEventmgr->Get_Time() >= 5.f && m_pEventmgr->Get_Time() < 10.f) {
		if (m_pEventmgr->Get_isInitSet(1) == false) {
			m_pEventmgr->Set_isInitSet(1, true);
		}
		m_pTransform->Go_Z(fTimeDelta*0.3f);
	}


	if (m_pEventmgr->Get_Time() >= 10.f) {
		m_pGraphic_Device->LightEnable(0, TRUE);
		m_pGraphic_Device->LightEnable(2, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_AMBIENT, 0x00202020);
		m_pEventmgr->Off_Air();
	}

}


CCamera_Default * CCamera_Default::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Default*	pInstance = new CCamera_Default(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
