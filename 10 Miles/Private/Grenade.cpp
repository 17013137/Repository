#include "stdafx.h"
#include "..\Public\Grenade.h"
#include "GameInstance.h"
#include "SoundMgr.h"

CGrenade::CGrenade(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CGrenade::CGrenade(const CGrenade & rhs)
	: CGameObject(rhs)
{

}

HRESULT CGrenade::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;




	return S_OK;
}

HRESULT CGrenade::NativeConstruct(void * pArg)
{
	_float3		Pos;
	ZeroMemory(&TargetPos, sizeof(TargetPos));
	memcpy(&TargetPos, pArg, sizeof(TargetPos));
	//(TargetPos*)pArg;
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	TargetPos.Pos.y += 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TargetPos.Pos);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, TargetPos.target);
	m_pTransformCom->LookAt(TargetPos.target);
	Sub_pTransformCom->Scaled(_float3(0.6f, 0.6f, 0.6f));


	return S_OK;
}

_int CGrenade::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;

	__super::Tick(fTimeDelta);

	m_pTransformCom->Go_Straight(fTimeDelta);
	Sub_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), fTimeDelta * 5);
	Sub_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	_float3 D = TargetPos.target - TargetPos.Pos;
	_float R = D3DXVec3Length(&D) / 2.f;
	_float d = D3DXVec3Length(&D) / 1.5f;

	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 C = TargetPos.Pos + D / 2.f;
	C.y = C.y - sqrt((d*d) - (R*R));
	_float Y = sqrt(d*d - ((Pos.x - C.x)*(Pos.x - C.x)) - ((Pos.z - C.z)*(Pos.z - C.z))) + C.y;
	Pos.y = Y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);

	//if (FAILED(SetUp_OnTerrain()))
	//	return;
	
	On_OnTerrain();
	return 0;
}

void CGrenade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CGrenade::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	Sub_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

	if (FAILED(Sub_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(Sub_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	Sub_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CGrenade::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_PlayerCube"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	//¼­ºê
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Sub"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&Sub_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Sub"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&Sub_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Sub"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&Sub_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Sub"), m_Current_Level, TEXT("Prototype_Component_Texture_Grenade"), (CComponent**)&Sub_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrenade::On_OnTerrain()
{
	CGameInstance*			pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CVIBuffer_Terrain*		pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)
		return E_FAIL;

	if (vPosition.y < pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y && Count==true)
	{
		EFFECTTYPE RandPos;
		RandPos.Type = 0;
		//vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPosition.x += (rand() % 15)/10.f-0.75f;
		vPosition.z += (rand() % 15)/10.f-0.75f;
		vPosition.y = 0.1f;
		RandPos.Pos =_float3(vPosition.x + (rand() % 10) / 10.f - 0.5f, vPosition.y, vPosition.z + (rand() % 10) / 10.f - 0.5f);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;
		RandPos.Pos = _float3(vPosition.x + (rand() % 10) / 10.f - 0.5f, vPosition.y, vPosition.z + (rand() % 10) / 10.f - 0.5f);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;
		RandPos.Pos = _float3(vPosition.x + (rand() % 10) / 10.f - 0.5f, vPosition.y, vPosition.z + (rand() % 10) / 10.f - 0.5f);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;
		RandPos.Pos = _float3(vPosition.x + (rand() % 10) / 10.f - 0.5f, vPosition.y, vPosition.z + (rand() % 10) / 10.f - 0.5f);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;
		RandPos.Pos = _float3(vPosition.x + (rand() % 10) / 10.f - 0.5f, vPosition.y, vPosition.z + (rand() % 10) / 10.f - 0.5f);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Action"), &RandPos)))
			return E_FAIL;

		

		CStatus::STATUSDESC		StatusmDesc;
		ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
		StatusmDesc.fMaxHp = 1000.f;
		StatusmDesc.fMaxStamina = 1.f;
		StatusmDesc.fMaxStungauge = 100.f;
		StatusmDesc.fDamage = 30.f;
		StatusmDesc.fPower = 0.5f;
		StatusmDesc.frange = 7.f;
		StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;

		StatusmDesc.Transform_Com = m_pTransformCom;
		
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc)))
			return E_FAIL;

		
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Explosion"), vPosition)))
			return E_FAIL;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GRANADE);
		CSoundMgr::Get_Instance()->PlaySound(L"Explosion_0.wav", CSoundMgr::GRANADE, 1.f);

		m_bDead = true;
		Count = false;
	}
	

	Safe_Release(pGameInstance);
	return S_OK;
}

CGrenade * CGrenade::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrenade*	pInstance = new CGrenade(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CGrenade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGrenade::Clone(void * pArg)
{
	CGrenade*	pInstance = new CGrenade(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CGrenade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrenade::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(Sub_pTransformCom);
	Safe_Release(Sub_pTextureCom);
	Safe_Release(Sub_pVIBufferCom);
	Safe_Release(Sub_pRendererCom);

}
