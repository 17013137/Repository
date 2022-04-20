#include "stdafx.h"
#include "..\Public\Wither_skeleton.h"
#include "GameInstance.h"
#include<ctime>
#include "SoundMgr.h"
#include "EventMgr.h"

CWither_skeleton::CWither_skeleton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CWither_skeleton::CWither_skeleton(const CWither_skeleton & rhs)
	: CBlendObject(rhs)

{
}

HRESULT CWither_skeleton::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWither_skeleton::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	_float3 temp;

	memcpy(temp, pArg, sizeof(_float3));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);
	m_col_Range = 2.f;
	m_col_Size = { 1.f, 1.f };
	iTexture = 1;
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
	return S_OK;
}

_int CWither_skeleton::Tick(_float fTimeDelta)
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

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return 0;

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




	if (D3DXVec3Length(&vLook) <= 2.f || State == STATE_ATTACK)
	{
		if (State != STATE_FALLING && State != STATE_DOWN && State != STATE_UP)
		{
			Action = true;
			MoveNo = false;
			Attack(fTimeDelta);


		}

	}
	else if (Action == false)
	{

		Action = true;
		MoveNo = false;
		m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta / 1.5);
		m_pTransformCom->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));

		Crawl(fTimeDelta);
	}

	if (Action == false)
	{
		Stay(fTimeDelta);
	}


	


	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECT4 Effect;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER2_BEATEN);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_Beaten1.wav", CSoundMgr::MONSTER2_BEATEN, 0.6f);
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		for (int i = 0; i < 5; ++i)
		{
			Effect.Pos = MyPos;
			Effect.Target = MyPos + _float3(rand() % 2 - 1.f, rand() % 2 - 1.f, rand() % 2 - 1.f);
			Effect.BulltType = 10;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

		}
	}
	//if(m_pStatusCom->Get_HitState == CStatus::HIT_STATE_DEFAULT)

	Safe_Release(pGameInstance);
	return 0;

}

void CWither_skeleton::LateTick(_float fTimeDelta)
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

HRESULT CWither_skeleton::Render()
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
	Set_Part2(4, m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(5, m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);


	//어깨 각도 손으로 맞추기
	m_vTF[6]->LookAtWorld(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[7]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//m_vTF[2]->LookAtWorld(m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	//m_vTF[3]->LookAtWorld(m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//골반 각도 발에 맞추기
	_float3 RF = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[4]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	_float3 RL = m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[5]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;

	m_vTF[8]->LookRight(RF - RL);


	for (int i = 0; i < 9; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}

	return S_OK;
}

HRESULT CWither_skeleton::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 150.f;
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

	_tchar		szPath1[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath2[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath3[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };
	_tchar		szPath4[10][MAX_PATH] = { TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT(""),TEXT("") };

	for (int i = 0; i < 10; ++i)
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
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_Wither_skeleton"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}
	Set_Stay();



	return S_OK;
}



HRESULT CWither_skeleton::SetUp_OnTerrain()
{
	CGameInstance*			pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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



	Safe_Release(pGameInstance);
	return S_OK;
}

void CWither_skeleton::Set_Part(int Part, int Local, _float3 Pos)
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

void CWither_skeleton::Set_Part2(int Part, _float3 Pos)
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
	m_vTF[Part]->Set_WorldMatrix(MX);

}

void CWither_skeleton::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);
	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);


	//머리
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 1.f);

	//몸통
	m_vTF[1]->Set_LocalMatrix(i);
	m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
	m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
	m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.7f);
	//오른손
	m_vTF[2]->Set_LocalMatrix(i);
	m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.15f, -0.4f, 0.5f));
	m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
	//왼손
	m_vTF[3]->Set_LocalMatrix(i);
	m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.15f, -0.4f, 0.5f));
	m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);

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
	m_vTF[8]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, -0.3f));
	m_vTF[8]->Scaled(_float3(0.65f, 0.18f, 0.3f));

}

void CWither_skeleton::Attack(_float fTimeDelta)
{
	_float3 RH = _float3(0.005f, 0.055f, 0.015f);
	_float3 LH = _float3(-0.005f, 0.055f, 0.015f);
	_float3 OnB = _float3(0.0f, 0.0f, 0.13f);
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		State = STATE_ATTACK;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		m_vTF[4]->Go_Z(-3.f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.05f, -0.6f, 0.5f));
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.05f, -0.6f, 0.5f));
		AttackOn = true;
	}
	//반복
	_float Time = fTimeDelta* 79.2f;
	RH = RH *Time;
	LH = LH*Time;
	OnB = OnB*Time;

	if (AnFrame < 36)
	{
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 2);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.02f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.01f*Time);
		m_vTF[4]->Go_Y(-0.04f);
		m_vTF[5]->Go_Y(-0.04f);

		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.03f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.02f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);
		m_vTF[1]->Go_Up(-0.03f*Time);
		m_vTF[5]->Go_Z(0.05f*Time);
		m_vTF[4]->Go_Z(-0.03f*Time);
	}

	else if (AnFrame >= 36 && AnFrame < 43)
	{
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 3.5);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 3.5);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.13f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.07f*Time);
		m_vTF[4]->Go_Y(0.25f*Time);
		m_vTF[5]->Go_Y(0.25f*Time);
		//몸통
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 0.2f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.11f*Time);
		m_vTF[1]->Go_Straight(0.3f*Time);
		m_vTF[1]->Go_Up(0.2f*Time);
		m_vTF[5]->Go_Z(-0.3f*Time);
		m_vTF[4]->Go_Z(0.2f*Time);
	}
	else if (AnFrame >= 43 && AnFrame < 57)
	{
		if (AttackOn)
		{
			CGameInstance*		pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);
			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Type = 6;
			m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 1100.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 30.f;
			StatusmDesc.fPower = 0.15f;
			StatusmDesc.frange = 2.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);

			Safe_Release(pGameInstance);
			AttackOn = false;
		}
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 2.7);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 2.7);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.005f*Time);
		m_vTF[4]->Go_Y(-0.02f*Time);
		m_vTF[5]->Go_Y(-0.02f*Time);

		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.015f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
		m_vTF[1]->Go_BackWard(0.025f*Time);
		m_vTF[1]->Go_Up(-0.015f*Time);
		m_vTF[5]->Go_Z(0.025f*Time);
		m_vTF[4]->Go_Z(-0.015f*Time);
	}



	AnFrame += Time;

	if (AnFrame >= 57)
	{

		AnFrame = 0;
		State = STATE_STAY;

	}
	Action = true;
}

void CWither_skeleton::Falling(_float fTimeDelta)
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

	}


}
/*
void CWither_skeleton::Falling(_float fTimeDelta)
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
void CWither_skeleton::Down(_float fTimeDelta)
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

void CWither_skeleton::Up(_float fTimeDelta)
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

void CWither_skeleton::find(_float fTimeDelta)
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

void CWither_skeleton::Crawl(_float fTimeDelta)
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
	if (State != STATE_RUN || AnFrame >= 30)
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

	//if (AnFrame >= 30)
	//{
	//	AnFrame = 0;
	//}
	Action = true;
}

void CWither_skeleton::Walk(_float fTimeDelta)
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


		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.8f);


		//손
		m_vTF[2]->Set_Speed(0.1f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.15f, -0.f, 0.5f));
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

		m_vTF[3]->Set_Speed(0.1f);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.15f, -0.4f, 0.5f));
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

		//몸통
		m_vTF[1]->Set_Speed(0.1f);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);
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
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
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
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
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

void CWither_skeleton::Move(_float fTimeDelta)
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

void CWither_skeleton::Stay(_float fTimeDelta)
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
	_float Time = fTimeDelta * 60.f;
	if (AnTime + 3.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 1.5f > StandardAnTime)
	{
		Ang = 0.03f*Time;

	}
	else
		Ang = -0.03f*Time;

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


CWither_skeleton * CWither_skeleton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWither_skeleton*	pInstance = new CWither_skeleton(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CWither_skeleton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWither_skeleton::Clone(void * pArg)
{
	CWither_skeleton*	pInstance = new CWither_skeleton(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CWither_skeleton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWither_skeleton::Free()
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

}


