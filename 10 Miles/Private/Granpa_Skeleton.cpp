#include "stdafx.h"
#include "..\Public\Granpa_Skeleton.h"
#include "GameInstance.h"
#include<ctime>
#include "SoundMgr.h"
#include "EventMgr.h"

CGranpa_Skeleton::CGranpa_Skeleton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CGranpa_Skeleton::CGranpa_Skeleton(const CGranpa_Skeleton & rhs)
	: CBlendObject(rhs)

{
}

HRESULT CGranpa_Skeleton::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGranpa_Skeleton::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	/*_float3 temp;
	memcpy(temp, pArg, sizeof(_float3));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);

	m_col_Range = 3.f;*/
	_float4x4 temp;

	memcpy(temp, pArg, sizeof(_float4x4));
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(temp.m[0][0], temp.m[0][1], temp.m[0][2]));
	//m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(temp.m[1][0], temp.m[1][1], temp.m[1][2]));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(temp.m[2][0], temp.m[2][1], temp.m[2][2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(temp.m[3][0], temp.m[3][1], temp.m[3][2]));
	
	m_col_Range = 1.f;
	m_col_Size = { 1.f, 1.f };
	iTexture = 1;
	return S_OK;
}

_int CGranpa_Skeleton::Tick(_float fTimeDelta)
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
			Effect.BulltType = 4;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

		}
	}

	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return 0;
	TatgetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	TatgetPos.y = 2.f;
	vLook = TatgetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
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

		//����
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





	if (D3DXVec3Length(&vLook) <= 10.f &&AttackDelay < 0)
	{
		if (State != STATE_FALLING && State != STATE_DOWN && State != STATE_UP)
		{
			Action = true;
			MoveNo = false;
			Attack(fTimeDelta);

		}

	}



	if (State == STATE_ATTACK)
	{
		Action = true;
		MoveNo = false;
		Attack(fTimeDelta);
	}

	if (D3DXVec3Length(&vLook) > 5.f && D3DXVec3Length(&vLook) < 15.f && Action == false)
	{

		Action = true;
		MoveNo = false;
		m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta*0.7);
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


	//SetUp_OnTerrain();

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
	AttackDelay -= fTimeDelta;
	Safe_Release(pGameInstance);
	return 0;

}

void CGranpa_Skeleton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this))) {
		m_culling = true;
		return;
	}

	m_culling = false;

	//������ ��ġ ���߱�-------------------------------------------------------------
	_float3 Right;
	_float3 Up;
	_float3 Look;
	_float3 Pos;

	//��� ��ġ ���뿡 ���߱�
	Right = *D3DXVec3Normalize(&Right, &m_vTF[1]->Get_State(CSubTransform::STATE_RIGHT));
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Look = *D3DXVec3Normalize(&Look, &m_vTF[1]->Get_State(CSubTransform::STATE_LOOK));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[6]->Set_State(CSubTransform::STATE_POSITION, Pos + (Right * 0.4f) + (Up * 0.25f));
	m_vTF[7]->Set_State(CSubTransform::STATE_POSITION, Pos + -(Right * 0.4f) + (Up * 0.25f));

	//�Ӹ� ��ġ ���뿡 ���߱�
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[0]->Set_State(CSubTransform::STATE_POSITION, Pos + Up / 1.5f + Look / 5.f);





	//m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
}

HRESULT CGranpa_Skeleton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(2)))
	//	return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;



	for (int i = 0; i < 12; ++i)
	{
		m_vTF[i]->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}
	//�չ� ���� ����ü�� ����
	Set_Part(2, 1, m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part(3, 1, m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part2(4, m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(5, m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);


	//��� ���� ������ ���߱�
	m_vTF[6]->LookAtWorld(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[7]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//m_vTF[2]->LookAtWorld(m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	//m_vTF[3]->LookAtWorld(m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//��� ���� �߿� ���߱�
	_float3 RF = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[4]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	_float3 RL = m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[5]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;

	m_vTF[8]->LookRight(RF - RL);

	m_vTFTimber->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTFTimber->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
	m_vTFTimber->Scaled(_float3(0.5f, 0.5f, 5.f));
	for (int i = 0; i < 9; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}
	for (int i = 10; i < 12; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i - 10)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}

	if (FAILED(m_vTTTimber->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_vTFTimber->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_vVITimber->Render();

	return S_OK;
}

HRESULT CGranpa_Skeleton::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Timber"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Timber"), (CComponent**)&m_vVITimber)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Timber"), m_Current_Level, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_vTTTimber)))
		return E_FAIL;

	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 20.f;
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

	_tchar		szPath1[12][MAX_PATH];
	_tchar		szPath2[12][MAX_PATH];
	_tchar		szPath3[12][MAX_PATH];
	_tchar		szPath4[12][MAX_PATH];

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
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_Granpa_Skeleton"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}

	for (int i = 10; i < 12; ++i)
	{
		wsprintf(szPath1[i], TEXT("Com_Transform_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath1[i], LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_CubeTF, &TransformDesc)))
			return E_FAIL;
		m_vTF.push_back(m_CubeTF);

		wsprintf(szPath2[i], TEXT("Com_Renderer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath2[i], LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_CubeRD)))
			return E_FAIL;
		m_vRD.push_back(m_CubeRD);

		wsprintf(szPath3[i], TEXT("Com_VIBuffer_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath3[i], LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_CubeVI)))
			return E_FAIL;
		m_vVI.push_back(m_CubeVI);

		wsprintf(szPath4[i], TEXT("Com_CTexture_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_Granpa_Skeleton_Wing"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}
	Set_Stay();





	return S_OK;
}



HRESULT CGranpa_Skeleton::SetUp_OnTerrain()
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

void CGranpa_Skeleton::Set_Part(int Part, int Local, _float3 Pos)
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

void CGranpa_Skeleton::Set_Part2(int Part, _float3 Pos)
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

void CGranpa_Skeleton::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);
	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Pos.y = 2.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);


	//�Ӹ�
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(0.5f, 0.5f, 0.5f));
	m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 1.f);

	//����
	m_vTF[1]->Set_LocalMatrix(i);
	m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
	m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
	m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.7f);
	//������
	m_vTF[2]->Set_LocalMatrix(i);
	m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.15f, -0.4f, 0.5f));
	m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), -1.f);
	m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -5.f);
	//�޼�
	m_vTF[3]->Set_LocalMatrix(i);
	m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.15f, -0.4f, 0.5f));
	m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
	m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);

	//������
	m_vTF[4]->Set_LocalMatrix(i);
	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[4]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[4]->Turn(_float3(0.f, 1.f, 0.f), 0.15f);
	m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.9f);
	//�޹�
	m_vTF[5]->Set_LocalMatrix(i);
	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, -0.5f, 0.1f));
	m_vTF[5]->Scaled(_float3(0.4f, 0.3f, 0.6f));
	m_vTF[5]->Turn(_float3(0.f, 1.f, 0.f), -0.15f);
	m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.9f);

	//���
	m_vTF[6]->Set_LocalMatrix(i);
	m_vTF[6]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	m_vTF[7]->Set_LocalMatrix(i);
	m_vTF[7]->Scaled(_float3(0.25f, 0.25f, 0.25f));

	//���
	m_vTF[8]->Set_LocalMatrix(i);
	m_vTF[8]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, -0.3f));
	m_vTF[8]->Scaled(_float3(0.65f, 0.18f, 0.3f));

	m_vTF[10]->Set_LocalMatrix(i);
	m_vTF[10]->Set_State(CSubTransform::STATE_POSITION, _float3(1.f, 0.4f, -1.f));
	m_vTF[10]->Turn(_float3(1.f, 0.f, 0.f), 1.f);
	m_vTF[10]->Scaled(_float3(2.f, 2.f, 2.f));

	m_vTF[11]->Set_LocalMatrix(i);
	m_vTF[11]->Set_State(CSubTransform::STATE_POSITION, _float3(-1.f, 0.4f, -1.f));
	m_vTF[11]->Turn(_float3(1.f, 0.f, 0.f), 1.f);
	m_vTF[11]->Scaled(_float3(2.f, 2.f, 2.f));

	m_vTFTimber->Scaled(_float3(1.f, 1.f, 5.f));
}

void CGranpa_Skeleton::Attack(_float fTimeDelta)
{
	_float3 RH = _float3(0.f, 0.f, 0.1f);
	_float3 LH = _float3(-0.005f, 0.055f, 0.015f);
	_float3 OnB = _float3(0.0f, 0.0f, 0.13f);
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		State = STATE_ATTACK;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.05f, -0.6f, 0.5f));


		AttackOn = true;
		m_pTransformCom->LookAt(TatgetPos);
	}
	//�ݺ�
	_float Time = fTimeDelta* 72.f;
	RH = RH *Time;
	LH = LH*Time;
	OnB = OnB*Time;

	if (AnFrame < 80)
	{
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 10);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -fTimeDelta*1.4);
		m_pTransformCom->Go_Straight(-fTimeDelta / 3);

	}

	else if (AnFrame < 120)
	{
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 3);
		m_pTransformCom->Go_Straight(fTimeDelta * 3);

		EFFECT4  Effect;
		Effect.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f);
		Effect.Target = Effect.Pos;

		Effect.BulltType = 4;
		Effect.Speed = 3;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		Effect.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f);
		Effect.Target = Effect.Pos;

		Effect.BulltType = 4;
		Effect.Speed = 3;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		EFFECTTYPE m_Effect_Type;
		m_Effect_Type.Type = 6;
		m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

		CStatus::STATUSDESC		StatusmDesc;
		ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
		StatusmDesc.fMaxHp = 1.f;
		StatusmDesc.fMaxStamina = 1100.f;
		StatusmDesc.fMaxStungauge = 100.f;
		StatusmDesc.fDamage = 2.5f;
		StatusmDesc.fPower = 0.1f;
		StatusmDesc.frange = 4.f;
		StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
		StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

		StatusmDesc.Transform_Com = m_pTransformCom;

		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
	}
	else if (AnFrame < 150)
	{


		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 2.7);
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


	Safe_Release(pGameInstance);
	AnFrame += Time;

	if (AnFrame >= 200)
	{

		AnFrame = 0;
		State = STATE_STAY;
		AttackDelay = 3.f;
	}
	Action = true;
}

void CGranpa_Skeleton::Falling(_float fTimeDelta)
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
		//�Ӹ�
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f *Time);
		//����
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
		//�Ӹ�
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1 *Time);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2 * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 2 * LH);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.05f *Time);
		//����
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.12f *Time);
		m_vTF[1]->Go_BackWard(0.05f *Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
	}
	else if (AnFrame >= 30 && AnFrame < 40)
	{
		//�Ӹ�
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.01f *Time);
		//����
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Go_Straight(0.0125f *Time);
		m_pTransformCom->Go_Y(0.0025f *Time);
	}
	else if (AnFrame >= 40 && AnFrame < 50)
	{
		//�Ӹ�
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f *Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.02f *Time);
		//����
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
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		BULLETTYPE  Effect;
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		Effect.Pos = MyPos;
		Effect.Target = MyPos + _float3(rand() % 3 - 1.5f, 0.f, rand() % 3 - 1.5f);
		Effect.BulltType = 13;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Item"), &Effect);

		for (_int i = rand() % 4; i < 10; ++i)
		{
			Effect.Pos = MyPos;
			Effect.Target = MyPos + _float3(rand() % 3 - 1.5f, 0.f, rand() % 3 - 1.5f);
			Effect.BulltType = 5;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Item"), &Effect);
		}
		Safe_Release(pGameInstance);
	}


}
/*
void CGranpa_Skeleton::Falling(_float fTimeDelta)
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
//�Ӹ�
m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
//��

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);

//��
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
//����
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
//�Ӹ�
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
//��

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2 * RH);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 2 * LH);

//��
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
//����
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.12f*Time);
m_vTF[1]->Go_BackWard(0.05f*Time);

m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
}
else if (AnFrame >= 30 && AnFrame < 40)
{
//�Ӹ�
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
//��

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);

//��
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
//����
m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f*Time);
m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f*Time);
m_vTF[1]->Go_Straight(0.0125f*Time);
m_pTransformCom->Go_Y(0.0025f*Time);
}
else if (AnFrame >= 40 && AnFrame < 50)
{
//�Ӹ�
m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
//��

m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);

//��
m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF / 4);
m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF / 4);
m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.01f*Time);
m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.02f*Time);
//����
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
void CGranpa_Skeleton::Down(_float fTimeDelta)
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
		//	//�Ӹ�
		//	m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1);
		//	//��
		//	if (CurrentHand == HAND_EMPTY)
		//	{
		//		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		//		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		//	}
		//	//��
		//	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		//	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		//	m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f);
		//	m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f);
		//	//����
		//	m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f);
		//	m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f);
		//	m_vTF[1]->Go_Straight(0.05f);
	}



	else if (AnFrame >= 20)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.005f);
		////�Ӹ�
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		////��
		//if (CurrentHand == HAND_EMPTY)
		//{
		//	m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		//	m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		//}
		////��
		//m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		//m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		//m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.04f);
		//m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.08f);
		////����
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

void CGranpa_Skeleton::Up(_float fTimeDelta)
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
		//�Ӹ�
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1 * Time);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + 2 * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + 2 * LH);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f * Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f * Time);
		//����
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

		//�Ӹ�
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 0.1 * Time);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1 * Time);
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);

		//��
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.04f * Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.08f * Time);
		//����
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

void CGranpa_Skeleton::find(_float fTimeDelta)
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

void CGranpa_Skeleton::Crawl(_float fTimeDelta)
{
	//�����̴� �ӵ�
	_float3 RH = _float3(-0.004f, 0.03f, 0.03f);
	_float3 LH = _float3(0.004f, 0.03f, 0.03f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.13f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.03f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.03f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);

	//�غ�
	if (State != STATE_RUN)
	{
		Set_Stay();
		State = STATE_RUN;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;

		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), -0.8f);
		//����
		m_vTF[1]->Set_Speed(0.1f);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.5f);
		m_vTF[1]->Go_Straight(3.f);
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);


		//��
		m_vTF[2]->Set_Speed(0.1f);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

		m_vTF[3]->Set_Speed(0.1f);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

		//��
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

	//�ݺ�
	if (AnFrame < 15)
	{
		//��

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);

		//��
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
		//����
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f *Time);
	}

	if (AnFrame >= 15)
	{
		//��
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
		//����
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f *Time);
	}




	AnFrame += Time;

	if (AnFrame >= 30)
	{
		AnFrame = 0;
	}
	Action = true;
}

void CGranpa_Skeleton::Walk(_float fTimeDelta)
{
	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_WALK)
	{
		Set_Stay();
		State = STATE_WALK;
		StandardAnTime = 0.f;
		AnTime = 0.f;


		D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), -0.5f);
		D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
		Loc = m_vTF[0]->Get_LocalMatrix();
		End = Loc * Rot * Far;
		//m_vTF[0]->Set_LocalMatrix(End);
	}
	StandardAnTime += fTimeDelta;
	_float Time = fTimeDelta * 60.f;
	if (AnTime + 0.5f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 0.375f > StandardAnTime)
	{
		Ang = 0.04f*Time;

	}
	else
		Ang = -0.12f*Time;

	D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), Ang / 5);
	D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
	Loc = m_vTF[0]->Get_LocalMatrix();
	End = Loc * Rot * Far;
	//m_vTF[0]->Set_LocalMatrix(End);

	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);
	m_pTransformCom->Go_Y(-Ang / 20);
	m_vTF[10]->Turn(m_vTF[10]->Get_State(CSubTransform::STATE_UP), Ang);
	m_vTF[10]->Go_Straight(-Ang / 10);
	m_vTF[11]->Turn(m_vTF[11]->Get_State(CSubTransform::STATE_UP), -Ang);
	m_vTF[11]->Go_Straight(-Ang / 10);


	Action = true;
}

void CGranpa_Skeleton::Move(_float fTimeDelta)
{
	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_MOVE)
	{
		Set_Stay();
		State = STATE_MOVE;
		StandardAnTime = 0.f;
		AnTime = 0.f;

		D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), -0.5f);
		D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
		Loc = m_vTF[0]->Get_LocalMatrix();
		End = Loc * Rot * Far;
		//m_vTF[0]->Set_LocalMatrix(End);
	}
	StandardAnTime += fTimeDelta;
	_float Time = fTimeDelta * 60.f;
	if (AnTime + 1.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 0.75f > StandardAnTime)
	{
		Ang = 0.02f*Time;

	}
	else
		Ang = -0.06f*Time;

	D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), Ang / 5);
	D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
	Loc = m_vTF[0]->Get_LocalMatrix();
	End = Loc * Rot * Far;
	//m_vTF[0]->Set_LocalMatrix(End);

	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);
	m_pTransformCom->Go_Y(-Ang / 20);
	m_vTF[10]->Turn(m_vTF[10]->Get_State(CSubTransform::STATE_UP), Ang);
	m_vTF[10]->Go_Straight(-Ang / 10);
	m_vTF[11]->Turn(m_vTF[11]->Get_State(CSubTransform::STATE_UP), -Ang);
	m_vTF[11]->Go_Straight(-Ang / 10);


	Action = true;
}

void CGranpa_Skeleton::Stay(_float fTimeDelta)
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
		//m_vTF[0]->Set_LocalMatrix(End);
	}
	StandardAnTime += fTimeDelta;
	_float Time = fTimeDelta * 60.f;
	if (AnTime + 1.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 0.75f > StandardAnTime)
	{
		Ang = 0.02f*Time;

	}
	else
		Ang = -0.06f*Time;

	D3DXMatrixRotationAxis(&Rot, &_float3(0.f, 1.f, 0.f), Ang / 5);
	D3DXMatrixTranslation(&Far, 0.f, 0.f, 0.f);
	Loc = m_vTF[0]->Get_LocalMatrix();
	End = Loc * Rot * Far;
	//m_vTF[0]->Set_LocalMatrix(End);

	m_vTF[1]->Set_Speed(Ang * 2);
	m_vTF[1]->Go_Up(fTimeDelta);

	m_vTF[2]->Set_Speed(Ang * 2);
	m_vTF[2]->Go_Up(fTimeDelta);

	m_vTF[3]->Set_Speed(Ang * 2);
	m_vTF[3]->Go_Up(fTimeDelta);
	m_pTransformCom->Go_Y(-Ang / 20);
	m_vTF[10]->Turn(m_vTF[10]->Get_State(CSubTransform::STATE_UP), Ang);
	m_vTF[10]->Go_Straight(-Ang / 10);
	m_vTF[11]->Turn(m_vTF[11]->Get_State(CSubTransform::STATE_UP), -Ang);
	m_vTF[11]->Go_Straight(-Ang / 10);




}


CGranpa_Skeleton * CGranpa_Skeleton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGranpa_Skeleton*	pInstance = new CGranpa_Skeleton(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CGranpa_Skeleton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGranpa_Skeleton::Clone(void * pArg)
{
	CGranpa_Skeleton*	pInstance = new CGranpa_Skeleton(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CGranpa_Skeleton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGranpa_Skeleton::Free()
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


