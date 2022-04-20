#include "stdafx.h"
#include "..\Public\Fanatic.h"
#include "GameInstance.h"
#include <ctime>
#include "SoundMgr.h"
#include "EventMgr.h"

CFanatic::CFanatic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CFanatic::CFanatic(const CFanatic & rhs)
	: CBlendObject(rhs)

{
}

HRESULT CFanatic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFanatic::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	/*_float3 temp;
	memcpy(temp, pArg, sizeof(_float3));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);*/



	_float4x4 temp;

	memcpy(temp, pArg, sizeof(_float4x4));
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(temp.m[0][0], temp.m[0][1], temp.m[0][2]));
	//m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(temp.m[1][0], temp.m[1][1], temp.m[1][2]));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(temp.m[2][0], temp.m[2][1], temp.m[2][2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(temp.m[3][0], temp.m[3][1], temp.m[3][2]));

	m_col_Range = 1.f;
	m_col_Size = { 1.f, 1.f };
	iTexture = rand() % 3;
	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	return S_OK;
}

_int CFanatic::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return 1;
	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	
	__super::Tick(fTimeDelta);

	if (CEventMgr::Get_Instance()->Get_OnAir() == true)
	{
		Stay(fTimeDelta);
		return 0;
	}


	if (m_pTransformCom->Get_Scale().y == 0)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 1.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0.f, 0.f, 1.f));
	}

	GunLook = m_vTF[3]->Get_StateWorld(CSubTransform::STATE_LOOK);
	GunPos = m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION);
	m_pGraphic_Device->LightEnable(5, FALSE);

	pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return 0;
	Safe_AddRef(pTargetTransform);
	vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 NormalizeLook;
	_float3 MyLook;
	D3DXVec3Normalize(&NormalizeLook, &vLook);
	D3DXVec3Normalize(&MyLook, &(m_pTransformCom->Get_State(CTransform::STATE_LOOK)));

	_float TargetAngle = acos(D3DXVec3Dot(&NormalizeLook, &MyLook)) * (180 / 3.14f);


	if (D3DXVec3Length(&(MyLook - NormalizeLook)) < 0.0001f)
	{
		TargetAngle = 10.f;
	}

	if (MyLook == NormalizeLook)
		TargetAngle = 1.f;



	Action = false;




	if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
	{
		if (State != STATE_DOWN)
			Falling(fTimeDelta);

		//기어가기
		//m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta);
		//m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

		//Crawl(fTimeDelta);
	}

	if (State == STATE_DOWN)
	{
		Down(fTimeDelta);
	}
	if (State == STATE_UP)
	{
		Up(fTimeDelta);
	}



	if (iTexture == 0)
	{
		if (State == STATE_ATTACK)
		{
			Action = true;
			MoveNo = false;
			Attack(fTimeDelta);
		}
		else if (D3DXVec3Length(&vLook) <= 2.f &&AttackDelay<0)
		{
			if (State != STATE_FALLING && State != STATE_DOWN && State != STATE_UP)
			{
				Action = true;
				MoveNo = false;
				Attack(fTimeDelta);


			}

		}
		else if (D3DXVec3Length(&vLook) > 1.f && D3DXVec3Length(&vLook) < 5.f && Action == false)
		{

			Action = true;
			MoveNo = false;
			m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta / 3);
			m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

			Walk(fTimeDelta);
		}
		else if (D3DXVec3Length(&vLook) < 12.f && Action == false)
		{
			Action = true;
			MoveNo = false;
			m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta / 3);
			m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

			Walk(fTimeDelta);
		}
		


		if (Action == false && rand() % 100 > 95 && MoveNo == false)
		{
			_float3 RardPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RardPos.x = RardPos.x + rand() % 10 - 5;
			RardPos.z = RardPos.z + rand() % 10 - 5;
			m_pTransformCom->LookAt(RardPos);
			MoveNo = true;

			PTime = 0.f;
		}
	}
	else if (iTexture == 1)
	{
		if (State == STATE_ATTACK)
		{
			Action = true;
			MoveNo = false;
			Shot(fTimeDelta);
		}
		else if (D3DXVec3Length(&vLook) <= 10.f&&AttackDelay<0)
		{
			if (State != STATE_FALLING && State != STATE_DOWN && State != STATE_UP)
			{
				Action = true;
				MoveNo = false;
				Shot(fTimeDelta);
			}

		}
		else if (D3DXVec3Length(&vLook) < 13.f && Action == false)
		{

			Action = true;
			MoveNo = false;
			m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta / 3);
			m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

			Walk(fTimeDelta);
		}
		else if (D3DXVec3Length(&vLook) > 8.f && D3DXVec3Length(&vLook) < 12.f && Action == false)
		{
			Action = true;
			MoveNo = false;
			find(fTimeDelta);
		}


		if (Action == false && rand() % 100 > 95 && MoveNo == false)
		{
			_float3 RardPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RardPos.x = RardPos.x + rand() % 10 - 5;
			RardPos.z = RardPos.z + rand() % 10 - 5;
			m_pTransformCom->LookAt(RardPos);
			MoveNo = true;

			PTime = 0.f;
		}
	}
	else if (iTexture == 2)
	{
		if (State == STATE_ATTACK)
		{
			Action = true;
			MoveNo = false;
			Summon(fTimeDelta);
		}
		else if (D3DXVec3Length(&vLook) <= 12.f && AttackDelay < 0)
		{
			if (State != STATE_FALLING && State != STATE_DOWN && State != STATE_UP)
			{
				Action = true;
				MoveNo = false;
				Summon(fTimeDelta);
			}

		}
		else if (D3DXVec3Length(&vLook) < 8.f && Action == false)
		{

			Action = true;
			MoveNo = false;
			m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));
			m_pTransformCom->Go_BackWard(fTimeDelta / 3);
			BackWard(fTimeDelta);
			//Walk(fTimeDelta);
		}
		


		if (Action == false && rand() % 100 > 95 && MoveNo == false)
		{
			_float3 RardPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RardPos.x = RardPos.x + rand() % 10 - 5;
			RardPos.z = RardPos.z + rand() % 10 - 5;
			m_pTransformCom->LookAt(RardPos);
			MoveNo = true;

			PTime = 0.f;
		}
	}



	if (MoveNo == true && Action == false)
	{
		if (PTime < 3)
		{
			Move(fTimeDelta);
			m_pTransformCom->Go_Straight(fTimeDelta / 6);
		}
		else if (PTime > 5)
		{
			PTime = 0.f;
			MoveNo = false;

		}
		PTime += fTimeDelta;
	}









	if (Action == false)
	{
		Stay(fTimeDelta);
	}


	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECTTYPE m_Effect_Type;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER1_BEATEN);
		CSoundMgr::Get_Instance()->PlaySound(L"Beaten1.wav", CSoundMgr::MONSTER1_BEATEN, 0.6f);

		m_Effect_Type.Type = (_int)(fTimeDelta * 1000) % 10;
		m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_Effect_Type.Target = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Blood_Effect"), &m_Effect_Type)))
			return E_FAIL;
	}
	//if(m_pStatusCom->Get_HitState == CStatus::HIT_STATE_DEFAULT)

	AttackDelay -= fTimeDelta;
	return 0;

}

void CFanatic::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this))) {
		m_culling = true;
		return;
	}

	m_culling = false;

	//파츠별 위치 맞추기-------------------------------------------------------------
	_float3 Right;
	_float3 Up;
	_float3 Look;
	_float3 Pos;

	//어깨 위치 몸통에 맞추기
	Right = *D3DXVec3Normalize(&Right, &m_vTF[1]->Get_State(CSubTransform::STATE_RIGHT));
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Look = *D3DXVec3Normalize(&Look, &m_vTF[1]->Get_State(CSubTransform::STATE_LOOK));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[6]->Set_State(CSubTransform::STATE_POSITION, Pos + (Right * 0.4f) + (Up * 0.25f));
	m_vTF[7]->Set_State(CSubTransform::STATE_POSITION, Pos + -(Right * 0.4f) + (Up * 0.25f));

	//머리 위치 몸통에 맞추기
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[0]->Set_State(CSubTransform::STATE_POSITION, Pos + Up / 1.5f + Look / 5.f);





	//m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
}

HRESULT CFanatic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(2)))
	//	return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;



	for (int i = 0; i < 9; ++i)
	{
		m_vTF[i]->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}
	//손발 파츠 상하체에 고정
	Set_Part(2, 1, m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part(3, 1, m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part2(m_vTF[4], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(m_vTF[5], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);


	//어깨 각도 손으로 맞추기
	m_vTF[6]->LookAtWorld(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[7]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//m_vTF[2]->LookAtWorld(m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	//m_vTF[3]->LookAtWorld(m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//골반 각도 발에 맞추기
	_float3 RF = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[4]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	_float3 RL = m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[5]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;

	m_vTF[8]->LookRight(RF - RL);
	if (iTexture == 0)
	{
		m_vTFTimber->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFTimber->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
		m_vTFTimber->Scaled(_float3(1.f, 1.f, 5.f));
	}
	else if (iTexture == 1)
	{
		m_vTFPistol->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));

		m_vTFPistol->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
		Set_Part2(m_vTFPistol, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFPistol->LookAt(m_vTF[3]->Get_State(CSubTransform::STATE_POSITION));
		Set_Part2(m_vTF[3], m_vTFPistol->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTFPistol->Get_StateWorld(CSubTransform::STATE_LOOK) / 1.5);
	}

	for (int i = 0; i < 9; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i + 9 * iTexture)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}

	if (iTexture == 0)
	{
		if (FAILED(m_vTTTimber->Bind_OnGraphicDevice(0)))
			return E_FAIL;

		if (FAILED(m_vTFTimber->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVITimber->Render();
	}

	if (iTexture == 1)
	{
		if (FAILED(m_vTTPistol->Bind_OnGraphicDevice(0)))
			return E_FAIL;

		if (FAILED(m_vTFPistol->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIPistol->Render();
	}

	return S_OK;
}

HRESULT CFanatic::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Granpa_Skeleton"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Timber"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFTimber, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Timber"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDTimber)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Timber"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Knife"), (CComponent**)&m_vVITimber)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Timber"), m_Current_Level, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_vTTTimber)))
		return E_FAIL;
	//무기(Pistol)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFPistol, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Pistol"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Pistol"), (CComponent**)&m_vVIPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Pistol"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTPistol)))
		return E_FAIL;

	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 200.f;
	StatusmDesc.fMaxStamina = 100.f;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 10.f;
	StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;
	StatusmDesc.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &StatusmDesc)))
		return E_FAIL;

	CVIBuffer_Cube*		m_CubeVI = nullptr;
	CSubTransform*		m_CubeTF = nullptr;
	CTexture*			m_CubeTT = nullptr;
	CRenderer*			m_CubeRD = nullptr;

	_tchar		szPath1[9][MAX_PATH];
	_tchar		szPath2[9][MAX_PATH];
	_tchar		szPath3[9][MAX_PATH];
	_tchar		szPath4[9][MAX_PATH];

	for (int i = 0; i < 9; ++i)
	{
		wsprintf(szPath1[i], TEXT("Com_Transform_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath1[i], LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_CubeTF)))
			return E_FAIL;
		m_vTF.push_back(m_CubeTF);

		wsprintf(szPath2[i], TEXT("Com_Renderer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath2[i], LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_CubeRD)))
			return E_FAIL;
		m_vRD.push_back(m_CubeRD);

		wsprintf(szPath3[i], TEXT("Com_VIBuffer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath3[i], LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_CubeVI)))
			return E_FAIL;
		m_vVI.push_back(m_CubeVI);

		wsprintf(szPath4[i], TEXT("Com_CTexture_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_Fanatic"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}


	Set_Stay();





	return S_OK;
}



HRESULT CFanatic::SetUp_OnTerrain()
{


	_float RF = m_vTF[4]->Get_State(CSubTransform::STATE_POSITION).y;
	_float LF = m_vTF[5]->Get_State(CSubTransform::STATE_POSITION).y;

	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	CVIBuffer_Terrain*		pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)
		return E_FAIL;

	if (RF < LF)
	{
		vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - RF + 0.5f;
	}

	else if (RF >= LF)
	{
		vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - LF + 0.5f;
	}



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);




	return S_OK;
}

void CFanatic::Set_Part(int Part, int Local, _float3 Pos)
{
	_float4x4	MX;
	D3DXMatrixIdentity(&MX);
	_float3  Ri = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_RIGHT);
	_float3  Up = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_UP);
	_float3  Lo = m_vTF[Local]->Get_StateWorld(CSubTransform::STATE_LOOK);
	D3DXVec3Normalize(&Ri, &Ri);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Normalize(&Lo, &Lo);
	memcpy(&MX.m[0][0], &Ri, sizeof(_float3));
	memcpy(&MX.m[1][0], &Up, sizeof(_float3));
	memcpy(&MX.m[2][0], &Lo, sizeof(_float3));
	memcpy(&MX.m[3][0], &Pos, sizeof(_float3));
	m_vTF[Part]->Set_WorldMatrix(MX);

}

void CFanatic::Set_Part2(CSubTransform* Part, _float3 Pos)
{
	_float4x4	MX;
	D3DXMatrixIdentity(&MX);
	_float3  Ri = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	_float3  Up = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_float3  Lo = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&Ri, &Ri);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Normalize(&Lo, &Lo);
	memcpy(&MX.m[0][0], &Ri, sizeof(_float3));
	memcpy(&MX.m[1][0], &Up, sizeof(_float3));
	memcpy(&MX.m[2][0], &Lo, sizeof(_float3));
	memcpy(&MX.m[3][0], &Pos, sizeof(_float3));
	Part->Set_WorldMatrix(MX);

}

void CFanatic::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);


	//머리
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(0.5f, 0.5f, 0.5f));

	//머리
	if (iTexture == 0)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (iTexture == 1)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(20.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.7f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (iTexture == 2)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	//오른발
	m_vTF[4]->Set_LocalMatrix(i);
	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[4]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[4]->Turn(_float3(0.f, 1.f, 0.f), 0.15f);
	//왼발
	m_vTF[5]->Set_LocalMatrix(i);
	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[5]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[5]->Turn(_float3(0.f, 1.f, 0.f), -0.15f);

	//어꺠
	m_vTF[6]->Set_LocalMatrix(i);
	m_vTF[6]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	m_vTF[7]->Set_LocalMatrix(i);
	m_vTF[7]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	//골반
	m_vTF[8]->Set_LocalMatrix(i);
	m_vTF[8]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	m_vTF[8]->Scaled(_float3(0.65f, 0.18f, 0.3f));

	m_vTFPistol->Scaled(_float3(1.5f, 1.5f, 2.f));
	m_vTFPistol->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

}

void CFanatic::Attack(_float fTimeDelta)
{
	_float3 RH = _float3(0.f, 0.f, -0.05f);
	_float3 LH = _float3(0.02f, 0.05, 0.01f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_ATTACK)
	{
		Set_Stay();
		//m_isChase = m_pTransformCom->LookAt(m_vTargetPos);
		State = STATE_ATTACK;
		AnFrame = 0;
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f);
		m_vTF[4]->Go_Z(-3.f);
		AttackOn = true;
	}

	_float Time = fTimeDelta * 80;
	RH = RH*Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;


	if (AnFrame < 20)
	{
		//머리

		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.07f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.06f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.04f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.04f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

	}



	else if (AnFrame >= 20 && AnFrame < 30)
	{
		if (AnFrame > 20.f&&AnFrame < 20.1f)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Sword Stab(Flesh) 6.wav", CSoundMgr::MONSTER3_SHOT, 0.4f);
		}
		//머리
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.001f*Time);
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2.2f * RH);
		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 0.5f * RH);
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.15f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF * 3);
		m_vTF[4]->Go_Z(0.01f*Time);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - 1.8 * LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.14f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.1f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.17f*Time);
		m_vTF[1]->Go_Straight(0.3f*Time);

		m_vTF[3]->Set_StateWorld((CSubTransform::STATE_POSITION), m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));


	}
	else if (AnFrame >= 30 && AnFrame < 45)
	{
		if (AttackOn)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shot.wav", CSoundMgr::MONSTER2_SHOT, 0.2f);
			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Type = 7;
			m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 1200.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 30.f;
			StatusmDesc.fPower = 0.2f;
			StatusmDesc.frange = 3.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;
			m_pTransformCom->Go_BackWard(0.01f);

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);


			AttackOn = false;
		}
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 5);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 5);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.014f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 5);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 5);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.012f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.008f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.008f*Time);
		m_vTF[1]->Go_BackWard(0.025f*Time);
	}



	AnFrame += Time;

	if (AnFrame >= 50)
	{

		AnFrame = 0;
		State = STATE_STAY;
		AttackDelay = 0.f;
	}
	Action = true;
}

void CFanatic::Falling(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	if (State != STATE_FALLING)
	{
		Set_Stay();
		State = STATE_FALLING;
		AnFrame = 0;
		if (CStatus::HIT_STATE_KNOCKBACK == m_pStatusCom->Get_HitState())
			m_pTransformCom->Turn(DownAngle, D3DXToRadian(-18.0f));
	}
	_float Time = fTimeDelta* 60.f;
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;
	DownAngle = DownAngle*Time;
	if (AnFrame < 10)
	{
		//머리
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f *Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.03f *Time);
		m_vTF[1]->Go_Straight(0.05f *Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2 * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 2 * LH);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.05f *Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.12f *Time);
		m_vTF[1]->Go_BackWard(0.05f *Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
	}
	else if (AnFrame >= 30 && AnFrame < 40)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.01f *Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Go_Straight(0.0125f *Time);
		m_pTransformCom->Go_Y(0.0025f *Time);
	}
	else if (AnFrame >= 40 && AnFrame < 50)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.02f *Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.0125f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.0125f *Time);
		m_vTF[1]->Go_Straight(-0.0125f *Time);
		m_pTransformCom->Go_Y(-0.0025f *Time);
	}


	AnFrame += Time;
	Action = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_DOWN;

		BULLETTYPE  Effect;
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		Effect.Pos = MyPos;
		Effect.Target = MyPos + _float3(rand() % 3 - 1.5f, 0.f, rand() % 3 - 1.5f);
		Effect.BulltType = 13;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Item"), &Effect);

		for (_int i = rand() % 4; i < 2; ++i)
		{
			Effect.Pos = MyPos;
			Effect.Target = MyPos + _float3(rand() % 3 - 1.5f, 0.f, rand() % 3 - 1.5f);
			Effect.BulltType = 5;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Item"), &Effect);
		}

	}


}
/*
void CFanatic::Falling(_float fTimeDelta)
{
_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
_float3 LH = _float3(0.02f, 0.05, 0.05f);
_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
_float3 LF = _float3(0.01f, 0.05f, 0.04f);
_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
if (State != STATE_FALLING)
{
Set_Stay();
State = STATE_FALLING;
AnFrame = 0;

}
_float Time = fTimeDelta * 60.f;
RH = RH *Time;
LH = LH*Time;
RF = RF*Time;
LF = LF*Time;


if (AnFrame < 10)
{
//머리
m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
//손

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);

//발
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
//몸통
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.03f*Time);
m_vTF[1]->Go_Straight(0.05f*Time);

m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
}

else if (AnFrame >= 10 && AnFrame < 20)
{
m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
}
else if (AnFrame >= 20 && AnFrame < 30)
{
//머리
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
//손

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2 * RH);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 2 * LH);

//발
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
//몸통
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.12f*Time);
m_vTF[1]->Go_BackWard(0.05f*Time);

m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
}
else if (AnFrame >= 30 && AnFrame < 40)
{
//머리
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
//손

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);

//발
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
//몸통
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f*Time);
m_vTF[1]->Go_Straight(0.0125f*Time);
m_pTransformCom->Go_Y(0.0025f*Time);
}
else if (AnFrame >= 40 && AnFrame < 50)
{
//머리
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
//손

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);

//발
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF / 4);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF / 4);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.02f*Time);
//몸통
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.0125f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.0125f*Time);
m_vTF[1]->Go_Straight(-0.0125f*Time);
m_pTransformCom->Go_Y(-0.0025f*Time);
}


AnFrame += Time;
Action = true;

if (AnFrame >= 50)
{
AnFrame = 0;
State = STATE_DOWN;

}


}
*/
void CFanatic::Down(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_DOWN)
	{
		Set_Stay();
		State = STATE_DOWN;
		AnFrame = 0;
	}
	_float Time = fTimeDelta*60.f;
	RH = RH*Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;

	if (AnFrame < 10)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.005f);
		//	//머리
		//	m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1);
		//	//손
		//	if (CurrentHand == HAND_EMPTY)
		//	{
		//		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		//		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		//	}
		//	//발
		//	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		//	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		//	m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f);
		//	m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f);
		//	//몸통
		//	m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f);
		//	m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f);
		//	m_vTF[1]->Go_Straight(0.05f);
	}



	else if (AnFrame >= 20)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.005f);
		////머리
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		////손
		//if (CurrentHand == HAND_EMPTY)
		//{
		//	m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		//	m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		//}
		////발
		//m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		//m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		//m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.04f);
		//m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.08f);
		////몸통
		//m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f);
		//m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f);
		//m_vTF[1]->Go_BackWard(0.05f);
	}
	else
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.05f*Time);
	}




	++AnFrame;
	Action = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_UP;

		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_bDead = true;
	}

}

void CFanatic::Up(_float fTimeDelta)
{
	_float3 RH = _float3(-0.05f, 0.03f, 0.03f);
	_float3 LH = _float3(0.02f, 0.05, 0.05f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	_float3 DownAngle = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	if (State != STATE_UP)
	{
		Set_Stay();
		State = STATE_UP;
		AnFrame = 0;

	}
	_float Time = fTimeDelta * 60;
	RH = RH * Time;
	LH = LH * Time;
	RF = RF * Time;
	LF = LF * Time;


	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1 * Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + 2 * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + 2 * LH);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f * Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f * Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f * Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.1f * Time);
		m_vTF[1]->Go_Straight(0.05f * Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f * Time));

	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f * Time));
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{

		//머리
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 0.1 * Time);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1 * Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);

		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.04f * Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.08f * Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f * Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.03f * Time);
		m_vTF[1]->Go_BackWard(0.05f);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f * Time));
	}


	AnFrame += Time;
	Action = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		Set_Stay();
		State = STATE_STAY;

	}
	else
	{

	}


}

void CFanatic::find(_float fTimeDelta)
{

	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_FIND)
	{
		Set_Stay();
		State = STATE_FIND;
		StandardAnTime = 0.f;
		AnTime = 0.f;

		D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), -0.5f);
		D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-50.f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-20.f));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), -0.8f);
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.4f);
	}
	StandardAnTime += fTimeDelta;

	if (AnTime + 5.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 0.5f > StandardAnTime)
	{
		Ang = 1.5f*fTimeDelta;

	}
	else if (AnTime + 2.5f > StandardAnTime && AnTime + 0.5f <= StandardAnTime)
	{
		Ang = -0.06f*fTimeDelta;
	}
	else if (AnTime + 3.f > StandardAnTime && AnTime + 2.5f <= StandardAnTime)
	{
		Ang = -1.5f*fTimeDelta;
	}

	else
		Ang = 0.06f*fTimeDelta;


	m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), Ang * 2);
	m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), Ang);
	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);


}

void CFanatic::Crawl(_float fTimeDelta)
{
	//움직이는 속도
	_float3 RH = _float3(-0.004f, 0.03f, 0.03f);
	_float3 LH = _float3(0.004f, 0.03f, 0.03f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.13f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.03f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.03f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);

	//준비
	if (State != STATE_RUN)
	{
		Set_Stay();
		State = STATE_RUN;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;

		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.8f);
		//몸통
		m_vTF[1]->Set_Speed(0.1f);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);
		m_vTF[1]->Go_Straight(3.f);
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);


		//손
		m_vTF[2]->Set_Speed(0.1f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

		m_vTF[3]->Set_Speed(0.1f);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.9f);

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);

	}
	_float Time = fTimeDelta*60.f;
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	OnF = OnF*Time;
	UpF = UpF*Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation*Time;

	//반복
	if (AnFrame < 15)
	{
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);

		//발
		if (AnFrame < 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.15f *Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
		}
		if (AnFrame >= 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f *Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f *Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f *Time);
	}

	if (AnFrame >= 15)
	{
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);


		if (AnFrame < 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.15f *Time);
		}
		if (AnFrame >= 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f *Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f *Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f *Time);
	}




	AnFrame += Time;

	if (AnFrame >= 30)
	{
		AnFrame = 0;
	}
	Action = true;
}

void CFanatic::Walk(_float fTimeDelta)
{
	_float3 RH = _float3(-0.0015f, 0.01f, 0.01f);
	_float3 LH = _float3(0.0015f, 0.01f, 0.01f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.05f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.02f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.02f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);
	if (State != STATE_WALK || AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_WALK;
		StandardAnTime = 0.f;
		AnFrame = 0.f;

		m_vTF[2]->Set_Speed(0.1f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

		m_vTF[3]->Set_Speed(0.1f);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.1f);
		m_vTF[1]->Set_Speed(0.1f);

		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);

	}

	_float Time = fTimeDelta * 60;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	OnF = OnF *Time;
	UpF = UpF *Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation *Time;

	if (AnFrame < 15)
	{
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);

		if (AnFrame < 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
		}
		if (AnFrame >= 12)
		{
			
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);

		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
	}
	if (AnFrame >= 15)
	{

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);

		if (AnFrame < 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		}
		if (AnFrame >= 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f*Time);
	}


	AnFrame += Time;
	Action = true;
}

void CFanatic::BackWard(_float fTimeDelta)
{//움직이는 속도
	_float3 RH = _float3(-0.0015f, 0.01f, 0.01f);
	_float3 LH = _float3(0.0015f, 0.01f, 0.01f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.05f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.02f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.02f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);
	if (State != STATE_WALK || AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_WALK;
		StandardAnTime = 0.f;
		AnFrame = 0.f;

		//손
		m_vTF[2]->Set_Speed(0.1f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

		m_vTF[3]->Set_Speed(0.1f);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);
		m_vTF[1]->Set_Speed(0.1f);
		m_vTF[1]->Go_BackWard(2.f);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.3f);
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 10) + (UpF * 3));
		//m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 2) + (DownF * 3) + DownFCompensation);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.3f);
	}
	_float Time = fTimeDelta * 60;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	OnF = OnF *Time;
	UpF = UpF *Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation *Time;

	if (AnFrame < 15)
	{
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		if (AnFrame < 3)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		}
		if (AnFrame >= 3)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.03f*Time);
	}
	if (AnFrame >= 15)
	{
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		if (AnFrame < 18)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		}
		if (AnFrame >= 18)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.03f*Time);
	}

	AnFrame += Time;
	Action = true;
}

void CFanatic::Move(_float fTimeDelta)
{
	//움직이는 속도
	_float3 RH = _float3(-0.0015f, 0.01f, 0.01f);
	_float3 LH = _float3(0.0015f, 0.01f, 0.01f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.05f) / 2.f;
	_float3 UpF = _float3(0.0f, 0.03f, -0.02f) / 2.f;
	_float3 DownF = _float3(0.0f, -0.03f, 0.02f) / 2.f;
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f) / 2.f;

	//준비
	if (State != STATE_WALK || AnFrame >= 60)
	{
		Set_Stay();
		State = STATE_WALK;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;

		//몸통
		m_vTF[1]->Set_Speed(0.1f);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 3) + (UpF * 3));

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 3) + (DownF * 3) + DownFCompensation);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.6f);

	}
	_float Time = fTimeDelta * 60.f;
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	OnF = OnF*Time;
	UpF = UpF*Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation*Time;

	//반복

	if (AnFrame < 30)
	{
		//발
		if (AnFrame < 24)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF / 2));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF / 2));
		}
		if (AnFrame >= 24)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);

		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.01f*Time);
	}

	if (AnFrame >= 30)
	{
		//발
		if (AnFrame < 54)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF / 2));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF / 2));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		}
		if (AnFrame >= 54)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.01f*Time);

	}




	AnFrame += Time;


	Action = true;
}

void CFanatic::Stay(_float fTimeDelta)
{

	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_STAY)
	{
		Set_Stay();
		State = STATE_STAY;
		StandardAnTime = 0.f;
		AnTime = 0.f;

		D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), -0.5f);
		D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
		Loc = m_vTF[0]->Get_LocalMatrix();
		End = Loc * Rot * Far;
		m_vTF[0]->Set_LocalMatrix(End);
	}
	StandardAnTime += fTimeDelta;

	if (StandardAnTime > 3.f)
		StandardAnTime = 0;

	_float Ang;

	if (StandardAnTime < 1.5f)
	{
		Ang = 0.03f;

	}
	else
		Ang = -0.03f;

	D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), Ang / 5);
	D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
	Loc = m_vTF[0]->Get_LocalMatrix();
	End = Loc * Rot * Far;
	m_vTF[0]->Set_LocalMatrix(End);

	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);




}

void CFanatic::Shot(_float fTimeDelta)
{
	_float3 RH = _float3(0.f, 0.f, -0.05f);
	_float3 LH = _float3(0.02f, 0.05, 0.01f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);

	if (State != STATE_ATTACK)
	{
		Set_Stay();
		//m_isChase = m_pTransformCom->LookAt(m_vTargetPos);
		State = STATE_ATTACK;
		AnFrame = 0;

		AttackOn = true;
	}

	RH = RH*fTimeDelta;
	LH = LH*fTimeDelta;
	RF = RF*fTimeDelta;
	LF = LF*fTimeDelta;


	if (AnFrame < 1.5)
	{
		//머리
		m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));



	}




	else if (AnFrame < 2)
	{
		if (AttackOn)
		{

			m_pGraphic_Device->LightEnable(5, TRUE);
			GunPos.y = GunPos.y + 0.45f;
			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 2.f;
			StatusmDesc.fMaxStungauge = 30.f;
			StatusmDesc.fDamage = 20.f;
			StatusmDesc.fPower = 0.3f;
			StatusmDesc.frange = 2.f;
			StatusmDesc.fLook = GunLook * 130;
			StatusmDesc.fSpawnPos = GunPos;
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;
			m_pTransformCom->Go_BackWard(0.03f);

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_Bullet"), &StatusmDesc);


			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Type = 3;
			m_Effect_Type.Pos = GunPos;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_POINT;
			light.Diffuse.r = 300.f;
			light.Diffuse.g = 90.f;
			light.Diffuse.b = 90.f;
			light.Diffuse.a = 0.f;
			light.Attenuation0 = 500.f;
			light.Attenuation1 = 10.f;
			light.Attenuation2 = 10.f;
			light.Range = 5.f;

			light.Position = GunPos + GunLook;
			light.Position.y += 0.5;
			m_pGraphic_Device->SetLight(5, &light);

			//CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shot.wav", CSoundMgr::PLAYER_SHOT, 0.6f);

			AttackOn = false;
		}

	}



	AnFrame += fTimeDelta;

	if (AnFrame >= 2)
	{

		AnFrame = 0;
		State = STATE_STAY;
		AttackDelay = 2.f;
	}
	Action = true;
}

void CFanatic::Summon(_float fTimeDelta)
{
	_float3 RH = _float3(0.f, 0.055f, 0.01f);
	_float3 LH = _float3(0.f, 0.055f, 0.01f);
	_float3 OnB = _float3(0.0f, 0.0f, 0.13f);
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		State = STATE_ATTACK;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		AttackOn = true;
	}
	//반복
	_float Time = fTimeDelta* 20.f;
	RH = RH *Time;
	LH = LH*Time;
	OnB = OnB*Time;

	if (AnFrame < 60)
	{
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 3);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 3);
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.03f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.02f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);
		m_vTF[1]->Go_Up(-0.03f*Time);
		m_vTF[5]->Go_Z(0.05f*Time);
		m_vTF[4]->Go_Z(-0.03f*Time);

		if (AttackOn)
		{
			EFFECT3	MeteorCircleData;
			MeteorCircleData.TargetTransform = m_pTransformCom;
			MeteorCircleData.Delay = 2.f;
			MeteorCircleData.BulltType = 1;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_MagicCircle"), &MeteorCircleData);

			_float3 SummonPos1 = MyPos + _float3(rand() % 30 / 10 - 1.5f, 0.6f, rand() % 30 / 10 - 1.5f);

			EFFECT4  Effect;
			CSoundMgr::Get_Instance()->PlaySound(L"Summons.wav", CSoundMgr::MONSTER1_SHOT, 0.3f);
			for (int i = 0; i < 10; ++i)
			{
				Effect.Pos = SummonPos1;
				Effect.Target = SummonPos1 + _float3(rand() % 7 - 3.5f, -0.1f, rand() % 7 - 3.5f);
				Effect.BulltType = 10;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

			}
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Monster"), TEXT("Prototype_GameObject_Wither_skeleton"), &(SummonPos1));



			AttackOn = false;
		}
	}

	else if (AnFrame < 90)
	{

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		//m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.06f*Time);
		//m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[4]->Go_Y(0.1f*Time);
		m_vTF[5]->Go_Y(0.1f*Time);

		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.04f*Time);
		m_vTF[1]->Go_Straight(0.4f*Time);
		m_vTF[1]->Go_Up(0.04f*Time);
		m_vTF[5]->Go_Z(-0.05f*Time);
		m_vTF[4]->Go_Z(0.03f*Time);
	}

	AnFrame += Time;

	if (AnFrame >= 90)
	{

		AnFrame = 0;
		State = STATE_STAY;
		AttackDelay = 6.f;
	}
	Action = true;

}


CFanatic * CFanatic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFanatic*	pInstance = new CFanatic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFanatic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFanatic::Clone(void * pArg)
{
	CFanatic*	pInstance = new CFanatic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CFanatic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFanatic::Free()
{
	__super::Free();

	for (auto& pGameObject : m_vVI)
		Safe_Release(pGameObject);

	m_vVI.clear();

	for (auto& pGameObject : m_vTF)
		Safe_Release(pGameObject);

	m_vTF.clear();

	for (auto& pGameObject : m_vTT)
		Safe_Release(pGameObject);

	m_vTT.clear();

	for (auto& pGameObject : m_vRD)
		Safe_Release(pGameObject);

	m_vRD.clear();

	Safe_Release(m_pStatusCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(pGameInstance);
	Safe_Release(pTargetTransform);

}


