#include "stdafx.h"
#include "MapBase.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include "Level_Loading.h"
#include "EventMgr.h"

MapBase::MapBase(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

MapBase::MapBase(const MapBase & rhs)
	: CGameObject(rhs)
{
}

HRESULT MapBase::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT MapBase::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 700.f;
	m_fSizeY = 700.f;
	m_fX -= g_iWinCX / 2.f;
	m_fY = g_iWinCY / 2.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);


	return S_OK;
}

_int MapBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* gameins = CGameInstance::GetInstance();
	Safe_AddRef(gameins);
	imgmax = CEventMgr::Get_Instance()->Get_MaxLevel();


	CTransform* trans = (CTransform*)gameins->Get_Component(m_Current_Level, L"Gate", L"Com_Transform");
	CTransform* Player_trans = (CTransform*)gameins->Get_Component(m_Current_Level, L"Layer_Player", L"Com_Transform");
	_float3 GatePos;
	_float3 PlayerPos;
	if (trans == nullptr || Player_trans == nullptr)
		return 0;

	GatePos = trans->Get_State(CTransform::STATE_POSITION);
	PlayerPos = Player_trans->Get_State(CTransform::STATE_POSITION);

	dist = D3DXVec3Length(&(GatePos - PlayerPos));

	Safe_Release(gameins);

	if (dist < 3.f)
		Key_Input();
	else
		m_fX = g_iWinCX / -2.f;

	return 0;
}

void MapBase::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT MapBase::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(imgnum)))
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

void MapBase::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('T')) {
		m_fX *= -1;
	}
	if (m_fX > 0.f) {
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)) {
			CGameInstance* gameins = CGameInstance::GetInstance();
			Safe_AddRef(gameins);
			_int nextStage = MapMoving[imgnum];
			if (nextStage == MapMoving[0])
				return;
			if (FAILED(gameins->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)nextStage))))
				return;
			m_fX = g_iWinCX / -2.f;
			Safe_Release(gameins);
		}

		if (CKeyMgr::Get_Instance()->Key_Down('E')) {
			imgnum = imgmax;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('Q')) {
			if (imgmax < 8)
				imgnum = imgmax - 1;
			else
				imgnum = imgmax;
		}
	}

}

HRESULT MapBase::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Map"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

MapBase * MapBase::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	MapBase*	pInstance = new MapBase(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created MapBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MapBase::Clone(void * pArg)
{
	MapBase*	pInstance = new MapBase(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created MapBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MapBase::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
