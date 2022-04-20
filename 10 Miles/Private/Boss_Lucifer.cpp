#include "stdafx.h"
#include "..\Public\Boss_Lucifer.h"
#include "GameInstance.h"
#include "KeyMgr.h"
#include<ctime>
#include "SoundMgr.h"
#include "EventMgr.h"

CBoss_Lucifer::CBoss_Lucifer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CBoss_Lucifer::CBoss_Lucifer(const CBoss_Lucifer & rhs)
	: CBlendObject(rhs)

{
}

HRESULT CBoss_Lucifer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	m_pGraphic_Device->LightEnable(3, FALSE);
	return S_OK;
}

HRESULT CBoss_Lucifer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_iPage2Cnt = 0;
	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(40.f, 3.f, 40.f));
	pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	State = STATE_END;

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;
	light.Diffuse.a = 0.f;

	light.Direction = _float3(0.f, -0.6f, 0.4f);
	m_pGraphic_Device->SetLight(5, &light);
	m_pGraphic_Device->LightEnable(5, TRUE);
	iLaser = 0;

	CSoundMgr::Get_Instance()->PlaySound(L"Whisper.mp3", CSoundMgr::BOSS_WHISPER, 0.4f);


	return S_OK;
}

_int CBoss_Lucifer::Tick(_float fTimeDelta)
{

	if (true == m_bDead)
		return 1;

	if (ActionMoving(fTimeDelta) == 1)
		return 0;

	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	frange = D3DXVec3Length(&(pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));


	if (*m_pStatusCom->Get_pHp() < 4000.f)
	{


		++m_iPage2Cnt;
		if (2 > m_iPage2Cnt)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM2);

		}

		else if (2 < m_iPage2Cnt)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Tension2.mp3", CSoundMgr::BGM2, 0.5f);
		}
	}
	else
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Boss2.mp3", CSoundMgr::BGM2, 0.4f);
	}

	if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
	{
		if (State != STATE_DOWN)
			Falling(fTimeDelta);
	}

	__super::Tick(fTimeDelta);
	//패턴1
	
	if (SkillDelay < 0 && Page2 == false && Action == false)
	{
	_int Pattern = rand() % 100;
	if (Pattern < 40 && Attack_Time < 0 && frange < 15)
	{
	Attack(fTimeDelta);
	Attack_Time = 2.f;
	SkillDelay = 1.f;
	}
	else if (Pattern < 60 && Missile_Time < 0)
	{
	Missile(fTimeDelta);
	Missile_Time = 5.f;
	SkillDelay = 2.f;
	}
	else if (Pattern < 80 && Meteor_Time < 0)
	{
	Meteor(fTimeDelta);
	Meteor_Time = 20.f;
	SkillDelay = 3.f;
	}
	else if (Pattern < 99 && Laser_Time < 0 && frange>10)
	{
	Laser(fTimeDelta);
	Laser_Time = 15.f;
	SkillDelay = 3.f;
	}
	}
	
	//패턴2
	if (SkillDelay < 0 && Page2 == true && Action == false)
	{
		_int Pattern = rand() % 100;
		if (Pattern < 30 && Attack_Time < 0 && frange < 15)
		{
			Attack(fTimeDelta);
			Attack_Time = 1.f;
			SkillDelay = 1.f;
		}
		else if (Pattern < 50 && Missile_Time < 0)
		{
			Missile(fTimeDelta);
			Missile_Time = 5.f;
			SkillDelay = 2.f;
		}
		else if (Pattern < 75 && Meteor_Time < 0)
		{
			Meteor(fTimeDelta);
			Meteor_Time = 5.f;
			SkillDelay = 1.f;
		}
		else if (Pattern < 99 && Laser_Time < 0 && frange>10)
		{
			Laser(fTimeDelta);
			Laser_Time = 5.f;
			SkillDelay = 3.f;
		}
	}

	if (*m_pStatusCom->Get_pHp() < 4000.f&&Page2 == false)
	{
		Page(fTimeDelta);
	}




	if (CKeyMgr::Get_Instance()->Key_Down('K'))
	{
		Meteor(fTimeDelta);

	}


	if (CKeyMgr::Get_Instance()->Key_Down('O'))
	{
		Missile(fTimeDelta);

	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Laser(fTimeDelta);

	}

	if (State == STATE_MISSILE)
	{
		Missile(fTimeDelta);
	}
	if (State == STATE_LOCKON)
	{
		Meteor(fTimeDelta);
	}
	else if (State == STATE_ATTACK)
	{
		Attack(fTimeDelta);
	}
	else if (State == STATE_STAY)
	{
		Stay(fTimeDelta);
	}
	else if (State == STATE_LASER)
	{
		Laser(fTimeDelta);
	}
	else if (State == STATE_PAGE)
	{
		Page(fTimeDelta);
	}
	else if (State == STATE_HAND)
	{
		HandAction(fTimeDelta);
	}

	if (Page2)
	{
		Effect4 Effect;
		Effect.Pos = MyPos + _float3(rand() % 5 - 2.4f, rand() % 5 - 2.5f, rand() % 5 - 2.4f);
		Effect.Target = Effect.Pos + _float3(0.f, rand() % 5, 0.f);
		Effect.BulltType = 14;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

		Effect.Pos = MyPos + _float3(rand() % 5 - 2.4f, rand() % 5 - 2.5f, rand() % 5 - 2.4f);
		Effect.Target = Effect.Pos + _float3(0.f, rand() % 5, 0.f);
		Effect.BulltType = 18;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

	}



	if (Action == false)
	{
		Missile_Time -= fTimeDelta;
		Meteor_Time -= fTimeDelta;
		Laser_Time -= fTimeDelta;
		Attack_Time -= fTimeDelta;

		Stay(fTimeDelta);
		if (10 > frange)
		{
			m_pTransformCom->Go_BackWard(pow(10 - frange, 3) / 500);
		}
		else
		{
			m_pTransformCom->Go_Straight(fTimeDelta * 2);
		}

		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECTTYPE m_Effect_Type;

		m_Effect_Type.Type = (_int)(fTimeDelta * 1000) % 4;
		_float3	TagetLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float3	MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_Effect_Type.Pos = MyPos + TagetLook * 1;
		m_Effect_Type.Target = MyPos + TagetLook * 5;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Boss_Blood_Effect"), &m_Effect_Type)))
			return E_FAIL;
	}


	/*CSoundMgr::Get_Instance()->PlaySound(L"Tension_1.mp3", CSoundMgr::BGM, 1.f);
	CSoundMgr::Get_Instance()->PlaySound(L"Tension_2.mp3", CSoundMgr::BGM, 1.f);*/
	SkillDelay -= fTimeDelta;
	return 0;

}

void CBoss_Lucifer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float3 Right;
	_float3 Up = *D3DXVec3Normalize(&Up, &m_vTF[2]->Get_State(CSubTransform::STATE_UP));
	_float3 TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 Pos;
	m_vTF[0]->Set_State(CSubTransform::STATE_POSITION, m_vTF[1]->Get_State(CSubTransform::STATE_POSITION) + m_vTF[1]->Get_State(CSubTransform::STATE_UP)*0.9);
	if (State == STATE_STAY)
	{
		_float3 LooKxz = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		LooKxz.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		m_pTransformCom->LookAt(LooKxz);
	}



	Right = *D3DXVec3Normalize(&Right, &m_vTF[1]->Get_State(CSubTransform::STATE_RIGHT));
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));

	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, Pos + (Right * 1.5f) + (Up * 1.2f));
	m_vTF[6]->Set_State(CSubTransform::STATE_POSITION, Pos + -(Right * 1.5f) + (Up * 1.2f));
	//m_vTF[0]->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));


	//m_vRD[0]->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	m_vRD[0]->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);

}

HRESULT CBoss_Lucifer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(2)))
	//		return E_FAIL;

	//	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//		return E_FAIL;




	for (int i = 0; i < 13; ++i)
	{
		m_vTF[i]->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}

	if (State == STATE_STAY)
	{
		m_vTF[0]->LookAtWorld(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	}
	Set_Part(3, 1, m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part(4, 1, m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));

	m_vTF[5]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[6]->LookAtWorld(m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION));

	for (int i = 1; i < 7; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}
	for (int i = 7; i < 13; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i % 2 + 2)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}

	if (FAILED(m_vTT[0]->Bind_OnGraphicDevice(Face)))
		return E_FAIL;

	if (FAILED(m_vTF[0]->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_vVI[0]->Render();

	if (bLaser == true)
	{

		m_pTransformComLaser->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		//m_pTransformComLaser->LookAt(m_pTransformCom->Get_State(CTransform::STATE_LOOK)*10);
		m_pTransformComLaser->Scaled(_float3(2.f, 2.f, 100.f));
		m_pTransformComLaser->Turn(_float3(0.f, 0.f, 1.f), 0.7f);

		//m_pTransformComLaser->Go_Up(2.f);
		if (FAILED(m_pTextureComLaser->Bind_OnGraphicDevice(iLaser)))
			return E_FAIL;

		if (FAILED(m_pTransformComLaser->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_pVIBufferComLaser->Render();
	}
	RHandPos = m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION);
	LHandPos = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION);
	HeadPos = m_vTF[0]->Get_StateWorld(CSubTransform::STATE_POSITION);

	return S_OK;
}
void CBoss_Lucifer::Set_Part(int Part, int Local, _float3 Pos)
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

HRESULT CBoss_Lucifer::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
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

	if (FAILED(__super::SetUp_Components(TEXT("Com_TransformLaser"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_pTransformComLaser, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_RendererLaser"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererComLaser)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBufferLaser"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferComLaser)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_TextureLaser"), m_Current_Level, TEXT("Prototype_Component_Texture_Lazer"), (CComponent**)&m_pTextureComLaser)))
		return E_FAIL;





	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 10000.f;
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



	_tchar		szPath1[13][MAX_PATH];
	_tchar		szPath2[13][MAX_PATH];
	_tchar		szPath3[13][MAX_PATH];
	_tchar		szPath4[13][MAX_PATH];

	for (int i = 0; i < 7; ++i)
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
		if (FAILED(__super::SetUp_Components(szPath3[i], LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_CubeVI)))
			return E_FAIL;
		m_vVI.push_back(m_CubeVI);

		wsprintf(szPath4[i], TEXT("Com_CTexture_%d"), i);
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_Lucifer"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}


	for (int i = 7; i < 13; ++i)
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
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_LuciferWing"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}
	Set_Stay();



	return S_OK;
}






void CBoss_Lucifer::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);
	_float3	Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	Pos.y = 3;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);

	//머리
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(1.5f, 1.5f, 1.5f));
	//m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), 1.f);

	//몸통
	m_vTF[1]->Set_LocalMatrix(i);
	m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
	m_vTF[1]->Scaled(_float3(2.7f, 3.f, 1.5f));
	//m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.7f);
	//다리
	m_vTF[2]->Set_LocalMatrix(i);
	m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	m_vTF[2]->Scaled(_float3(2.7f, 2.f, 1.5f));

	//오른손
	m_vTF[3]->Set_LocalMatrix(i);
	m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(1.2f, -2.f, 0.8f));
	m_vTF[3]->Scaled(_float3(1.f, 1.f, 1.f));
	m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.0f));
	m_vTF[3]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30.0f));
	//왼손
	m_vTF[4]->Set_LocalMatrix(i);
	m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, _float3(-1.2f, -2.f, 0.8f));
	m_vTF[4]->Scaled(_float3(1.f, 1.f, 1.f));
	m_vTF[4]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.0f));
	m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30.0f));
	//어깨
	m_vTF[5]->Set_LocalMatrix(i);
	m_vTF[5]->Scaled(_float3(1.f, 1.f, 1.f));

	m_vTF[6]->Set_LocalMatrix(i);
	m_vTF[6]->Scaled(_float3(1.f, 1.f, 1.f));

	//
	m_vTF[7]->Set_LocalMatrix(i);
	m_vTF[7]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[7]->Set_State(CSubTransform::STATE_POSITION, _float3(-3.5f, 3.f, -1.5f));
	m_vTF[7]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30.0f));

	m_vTF[8]->Set_LocalMatrix(i);
	m_vTF[8]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[8]->Set_State(CSubTransform::STATE_POSITION, _float3(3.5f, 3.f, -1.5f));
	m_vTF[8]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(30.0f));
	//
	m_vTF[9]->Set_LocalMatrix(i);
	m_vTF[9]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[9]->Set_State(CSubTransform::STATE_POSITION, _float3(-3.5f, 2.f, -1.3f));

	m_vTF[10]->Set_LocalMatrix(i);
	m_vTF[10]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[10]->Set_State(CSubTransform::STATE_POSITION, _float3(3.5f, 2.f, -1.3f));
	//
	m_vTF[11]->Set_LocalMatrix(i);
	m_vTF[11]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[11]->Set_State(CSubTransform::STATE_POSITION, _float3(-3.5f, 1.f, -1.f));
	m_vTF[11]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(30.0f));

	m_vTF[12]->Set_LocalMatrix(i);
	m_vTF[12]->Scaled(_float3(5.f, 5.f, 1.f));
	m_vTF[12]->Set_State(CSubTransform::STATE_POSITION, _float3(3.5f, 1.f, -1.f));
	m_vTF[12]->Turn(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30.0f));

	//m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
	m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + m_vTF[2]->Get_State(CSubTransform::STATE_UP)*1.4);
	m_vTF[1]->Go_Straight(0.5f);
	m_vTF[0]->Go_Straight(0.5f);
	m_pTransformComLaser->Set_LocalMatrix(i);
	m_pTransformComLaser->Set_State(CSubTransform::STATE_POSITION, m_pTransformComLaser->Get_State(CSubTransform::STATE_POSITION) + m_pTransformComLaser->Get_State(CSubTransform::STATE_LOOK) * 55 + m_pTransformComLaser->Get_State(CSubTransform::STATE_UP)*3.5);
	m_pStatusCom->Look_On();
	m_pStatusCom->Knockback_on();
	bLaser = false;
}

void CBoss_Lucifer::Page(_float fTimeDelta)
{
	_float3 RH = _float3(-0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	if (State != STATE_PAGE)
	{
		Set_Stay();
		State = STATE_PAGE;
		StandardAnTime = 0.f;
		AnFrame = 0.f;
		Action = true;
		AttackOn = true;
		MeteorDelay = 0;
		m_pStatusCom->Knockback_off();
		m_pStatusCom->Look_Off();
		iLaser = 1;
		EFFECT4	Effect;

		for (_int i = 0; i < 30; ++i)
		{
			Effect.Pos = MyPos + _float3(rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f);
			Effect.Target = MyPos;
			Effect.BulltType = 4;
			Effect.Speed = 7;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Pos = MyPos + _float3(rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f);
			Effect.Target = MyPos;
			Effect.BulltType = 9;
			Effect.Speed = 9;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Pos = MyPos;
			Effect.Target = MyPos + _float3(rand() % 200 / 10 - 10.f, -0.1f, rand() % 200 / 10 - 10.f);
			Effect.BulltType = 20;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);
		}
	}

	if (AnFrame < 3.f)
	{
		_float T = 3.0f - AnFrame;
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 3));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[0]->Go_Straight(T / 200);
		m_vTF[1]->Go_Straight(T / 400);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		Stack = Stack * -1;
		m_pTransformCom->Go_Right((T / 30)*Stack);

	}

	else if (AnFrame >= 3.f && AnFrame < 3.25f)
	{
		Face = 7;
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-fTimeDelta * 480));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-fTimeDelta * 240));
		//m_vTF[0]->Go_Straight((-AnFrame+3.f)/8 );
		m_vTF[0]->Go_Straight(-fTimeDelta*9.f);
		m_vTF[1]->Go_Straight(-fTimeDelta*3.6f);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 16 * RH + 2 * OnB);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - 16 * LH + 2 * OnB);

		m_vTF[7]->Go_Straight(-fTimeDelta * 10);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[8]->Go_Straight(-fTimeDelta * 10);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
		m_vTF[9]->Go_Straight(-fTimeDelta * 10);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[10]->Go_Straight(-fTimeDelta * 10);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
		m_vTF[11]->Go_Straight(-fTimeDelta * 10);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[12]->Go_Straight(-fTimeDelta * 10);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
		Effect4	Effect;
		for (_int i = 0; i < 20; ++i)
		{
			Effect.Target = MyPos + _float3(rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f);
			Effect.Pos = MyPos;
			Effect.BulltType = 13;
			Effect.Speed = 15.f;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Target = MyPos + _float3(rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f, rand() % 200 / 10 - 10.f);
			Effect.Pos = MyPos;
			Effect.BulltType = 11;
			Effect.Speed = 15.f;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		if (Page2 == false)
		{
			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 1100.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 50.f;
			StatusmDesc.fPower = 1.f;
			StatusmDesc.frange = 6.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3;
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
			AttackOn = false;



			Page2 = true;
		}

	}





	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 5.5)
	{

		AnFrame = 0;
		State = STATE_STAY;
		Set_Stay();
		Action = false;
		m_pStatusCom->Knockback_on();
		m_pStatusCom->Look_On();
	}

}

void CBoss_Lucifer::Meteor(_float fTimeDelta)
{
	_float3 RH = _float3(-0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	if (State != STATE_LOCKON)
	{
		Set_Stay();
		State = STATE_LOCKON;
		StandardAnTime = 0.f;
		AnFrame = 0.f;
		Action = true;
		AttackOn = true;
		MeteorDelay = 0;
		m_pStatusCom->Knockback_off();
		m_pStatusCom->Look_Off();
		MeteorCircle = true;
	}

	if (AnFrame < 3.f)
	{
		if (MeteorCircle)
		{
			if (Page2)
			{
				EFFECT3	MeteorCircleData;
				MeteorCircleData.TargetTransform = m_pTransformCom;
				MeteorCircleData.Delay = 4.f;
				MeteorCircleData.BulltType = 5;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_MagicCircle"), &MeteorCircleData);
			}
			else
			{
				EFFECT3	MeteorCircleData;
				MeteorCircleData.TargetTransform = m_pTransformCom;
				MeteorCircleData.Delay = 4.f;
				MeteorCircleData.BulltType = 0;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_MagicCircle"), &MeteorCircleData);
			}
			MeteorCircle = false;
		}

		if (Page2)
		{
			MeteorCircle = false;
			Effect4 Effect;
			Effect.Pos = MyPos + _float3(rand() % 7 - 3.4f, rand() % 5 - 2.5f, rand() % 7 - 3.4f);
			Effect.Target = Effect.Pos + _float3(0.f, rand() % 5, 0.f);
			Effect.BulltType = 17;
			Effect.Speed = 1.f;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		else
		{
			MeteorCircle = false;
			Effect4 Effect;
			Effect.Pos = MyPos + _float3(rand() % 7 - 3.4f, rand() % 5 - 2.5f, rand() % 7 - 3.4f);
			Effect.Target = Effect.Pos + _float3(0.f, rand() % 5, 0.f);
			Effect.BulltType = 5;
			Effect.Speed = 1.f;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		if (1.5f < AnFrame && AnFrame < 1.6f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_SHOT1);
			CSoundMgr::Get_Instance()->PlaySound(L"Meteor1.wav", CSoundMgr::BOSS_SHOT1, 0.3f);
		}


		_float T = 3.0f - AnFrame;
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 3));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[0]->Go_Straight(T / 200);
		m_vTF[1]->Go_Straight(T / 400);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		Stack = Stack * -1;
		m_pTransformCom->Go_Right((T / 30)*Stack);
	}

	else if (AnFrame >= 3.f && AnFrame < 3.25f)
	{
		//Face = 7;
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-fTimeDelta * 480));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-fTimeDelta * 240));
		//m_vTF[0]->Go_Straight((-AnFrame+3.f)/8 );
		m_vTF[0]->Go_Straight(-fTimeDelta*9.f);
		m_vTF[1]->Go_Straight(-fTimeDelta*3.6f);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 16 * RH + 2 * OnB);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - 16 * LH + 2 * OnB);

		m_vTF[7]->Go_Straight(-fTimeDelta * 10);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[8]->Go_Straight(-fTimeDelta * 10);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
		m_vTF[9]->Go_Straight(-fTimeDelta * 10);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[10]->Go_Straight(-fTimeDelta * 10);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
		m_vTF[11]->Go_Straight(-fTimeDelta * 10);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-fTimeDelta * 280));
		m_vTF[12]->Go_Straight(-fTimeDelta * 10);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 280));
	}

	if (AnFrame > 2)
		int i = 1;
	if (AnFrame > 2 && AnFrame < 5 && MeteorDelay >= 0.3&&Page2 == false)
	{

		EFFECT4	MeteorData;
		MeteorData.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MeteorData.Pos.y += 30.f;
		MeteorData.Pos -= (m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 30);
		MeteorData.Target = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		MeteorData.Target.x += (rand() % 15) - 7.5f;
		MeteorData.Target.z += (rand() % 15) - 7.5f;
		MeteorData.BulltType = 0;
		if (MeteorData.Target.x > 2 && MeteorData.Target.x < 78)
		{
			if (MeteorData.Target.z > 2 && MeteorData.Target.z < 78)
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Meteor"), &MeteorData);
		}
		MeteorDelay = 0.f;

	}
	if (AnFrame > 2 && AnFrame < 5 && MeteorDelay >= 0.3&&Page2)
	{

		EFFECT4	MeteorData;
		MeteorData.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MeteorData.Pos.y += 30.f;
		MeteorData.Pos -= (m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 30);
		MeteorData.Target = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		MeteorData.Target.x += (rand() % 20) - 10.f;
		MeteorData.Target.z += (rand() % 20) - 10.f;
		MeteorData.BulltType = 1;
		if (MeteorData.Target.x > 2 && MeteorData.Target.x < 78)
		{
			if (MeteorData.Target.z > 2 && MeteorData.Target.z < 78)
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Meteor"), &MeteorData);
		}
		MeteorDelay = 0.f;

	}
	MeteorDelay += fTimeDelta;
	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 5.5)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Meteor2.wav", CSoundMgr::BOSS_SHOT1, 0.5f);

		AnFrame = 0;
		State = STATE_STAY;
		Set_Stay();
		Action = false;
		m_pStatusCom->Knockback_on();
		m_pStatusCom->Look_On();
	}

}

void CBoss_Lucifer::Missile(_float fTimeDelta)
{
	_float3 RH = _float3(-0.9f, 0.0f, 0.5f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	EFFECT4  Effect;
	if (State != STATE_MISSILE)
	{
		Set_Stay();
		State = STATE_MISSILE;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		//m_vTF[4]->Go_Z(-3.f);
		//m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.05f, -0.6f, 0.5f));
		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.05f, -0.6f, 0.5f));
		AttackOn = true;
		Action = true;
		Attack_frange = frange;
	}
	//반복


	if (AnFrame < 0.75)
	{

		//m_pTransformCom->Go_Straight(fTimeDelta * Attack_frange);
		_float T = (0.75f - AnFrame) * 15;
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Go_Straight(T / 75);
		m_vTF[1]->Go_Straight(T / 150);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH * 3);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH*2.25);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		if (Page2)
		{

			//Effect.Pos = RHandPos + _float3(rand() % 20 / 10 - 1.f, rand() % 20 / 10 - 1.f, rand() % 20 / 10 - 1.f);
			//Effect.Target = Effect.Pos;
			//Effect.Target.y -= 3.f;
			//Effect.BulltType = 4;
			//Effect.Speed = 3;
			//pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			//Effect.Pos = RHandPos + _float3(rand() % 20 / 10 - 1.f, rand() % 20 / 10 - 1.f, rand() % 20 / 10 - 1.f);
			//Effect.Target = Effect.Pos;
			//Effect.Target.y -= 3.f;
			//Effect.BulltType = 17;
			//Effect.Speed = 5;
			//pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

		}
		else
		{

			/*
			Effect.Pos = MyPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = Effect.Pos - m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7;
			Effect.Target.y += 3;
			Effect.Speed = Attack_frange / 2;
			Effect.BulltType = 5 + rand() % 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);


			Effect.Pos = MyPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = Effect.Pos - m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7;
			Effect.Target.y += 3;
			Effect.Speed = Attack_frange / 2;
			Effect.BulltType = 5 + rand() % 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			*/
		}
	}

	else if (AnFrame >= 0.75 && AnFrame < 0.95)
	{
		/*if (Page2)
		{
		for (_int i = 0; i < 20; ++i)
		{
		Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
		Effect.Pos = RHandPos;
		Effect.BulltType = 17 + rand() % 3;
		Effect.Speed = 7;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		}
		else
		{
		for (_int i = 0; i < 10; ++i)
		{
		Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
		Effect.Pos = RHandPos;
		Effect.BulltType = 9;
		Effect.Speed = 5;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		for (_int i = 0; i < 10; ++i)
		{
		Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
		Effect.Pos = RHandPos;
		Effect.BulltType = 10;
		Effect.Speed = 5;
		pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}
		}
		*/
		if (AttackOn == true)
		{

			if (Page2)
			{
				for (int i = 0; i < 15; ++i)
				{
					Effect.Pos = MyPos;
					Effect.Target = MyPos + _float3(rand() % 30 - 15.f, rand() % 5 + 1.f, rand() % 30 - 15.f);
					Effect.Speed = 0.2 + 0.2*i;
					Effect.BulltType = 1;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Missile"), &Effect);
				}
			}
			else
			{
				for (int i = 0; i < 7; ++i)
				{
					Effect.Pos = MyPos;
					Effect.Target = MyPos + _float3(rand() % 30 - 15.f, rand() % 5 + 1.f, rand() % 30 - 15.f);
					Effect.Speed = 0.4 + 0.4*i;
					Effect.BulltType = 0;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Missile"), &Effect);
				}
			}
			/*CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 1100.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 40.f;
			StatusmDesc.fPower = 1.f;
			StatusmDesc.frange = 5.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3;
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
			*/
			AttackOn = false;
		}

		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = -35 * (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH / 2);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}
	else if (AnFrame >= 0.95 && AnFrame < 1.5)
	{
		m_pTransformCom->Go_BackWard(fTimeDelta * 5);
		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - RH / 20);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - LH / 40);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}




	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 1.5)
	{
		AttackOn = true;
		Action = false;
		AnFrame = 0;
		Set_Stay();
	}

}

void CBoss_Lucifer::Laser(_float fTimeDelta)
{
	_float3 RH = _float3(-0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	if (State != STATE_LASER)
	{
		Set_Stay();
		State = STATE_LASER;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		Action = true;
		AttackOn = true;
		m_pStatusCom->Knockback_off();
		m_pStatusCom->Look_Off();
	}
	//반복


	if (AnFrame < 3)
	{
		Effect4  Effect;

		if (Page2)
		{
			Effect.Pos = RHandPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = RHandPos;
			Effect.BulltType = 17;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Pos = LHandPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = LHandPos;
			Effect.BulltType = 14;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

		}
		else
		{
			Effect.Pos = RHandPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = RHandPos;
			Effect.BulltType = 0;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Pos = LHandPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = LHandPos;
			Effect.BulltType = 8;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
		}


		_float T = 3.0f - AnFrame*(fTimeDelta * 60);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 5));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 5));

		m_vTF[0]->Go_Straight(T / 400);
		m_vTF[1]->Go_Straight(T / 200);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));

	}

	else if (AnFrame >= 3 && AnFrame < 3.19)
	{
		_float3 RH = _float3(-0.9f, 0.f, 0.7f)*0.015;
		_float3 LH = _float3(0.9f, 0.f, 0.7f)*0.015;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*0.015;
		_float T = -5;
		RH = RH * 20;
		LH = LH * 20;
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));

		m_vTF[0]->Go_Straight(T / 20);
		m_vTF[1]->Go_Straight(T / 50);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH / 1.5);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH / 1.5);
		T = T * 4;
		m_vTF[7]->Go_Straight(T / 100);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 3));
		m_vTF[8]->Go_Straight(T / 100);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 3));
		m_vTF[9]->Go_Straight(T / 100);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 3));
		m_vTF[10]->Go_Straight(T / 100);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 3));
		m_vTF[11]->Go_Straight(T / 100);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 3));
		m_vTF[12]->Go_Straight(T / 100);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 3));


	}
	else
	{
		if (MeteorDelay > 0.2)
		{

			CStatus::STATUSDESC		StatusmDesc;
		
			EFFECT4  Effect;

			if (Page2)
			{
				ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
				StatusmDesc.fMaxHp = 1.f;
				StatusmDesc.fMaxStamina = 1100.f;
				StatusmDesc.fMaxStungauge = 100.f;
				StatusmDesc.fDamage = 2.f;
				StatusmDesc.fPower = 0.3f;
				StatusmDesc.frange = 5.f;
				StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
				StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;

				StatusmDesc.Transform_Com = m_pTransformCom;

				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
				StatusmDesc.fPower = 0.5f;
				StatusmDesc.fMaxStamina = 1.f;
				StatusmDesc.fDamage = 5.f;
				StatusmDesc.fMaxStungauge = 200.f;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCube"), &StatusmDesc);

				for (int i = 0; i < 2; ++i)
				{

					Effect.Pos = MyPos;
					Effect.Target = MyPos + _float3(rand() % 20 - 10.f, -0.1f, rand() % 20 - 10.f);
					Effect.BulltType = 20;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

					Effect.Target = RHandPos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);
					Effect.Pos = RHandPos;
					Effect.BulltType = 4;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

					Effect.Target = LHandPos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);
					Effect.Pos = LHandPos;
					Effect.BulltType = 4;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);


				}
				for (int i = 0; i < 10; ++i)
				{

					Effect.Pos = LHandPos;
					Effect.Target = Effect.Pos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
					Effect.BulltType = 10;
					Effect.Speed = 30 + rand() % 10;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
					Effect.Pos = RHandPos;
					Effect.Target = Effect.Pos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
					Effect.BulltType = 9;
					Effect.Speed = 20 + rand() % 10;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
				}

			}
			else
			{
				ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
				StatusmDesc.fMaxHp = 1.f;
				StatusmDesc.fMaxStamina = 1100.f;
				StatusmDesc.fMaxStungauge = 100.f;
				StatusmDesc.fDamage = 1.f;
				StatusmDesc.fPower = 0.3f;
				StatusmDesc.frange = 5.f;
				StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
				StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;

				StatusmDesc.Transform_Com = m_pTransformCom;

				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
				StatusmDesc.fPower = 0.3f;
				StatusmDesc.fMaxStamina = 1.f;
				StatusmDesc.fDamage = 3.f;
				StatusmDesc.fMaxStungauge = 200.f;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCube"), &StatusmDesc);

				for (int i = 0; i < 2; ++i)
				{

					Effect.Pos = MyPos;
					Effect.Target = MyPos + _float3(rand() % 20 - 10.f, -0.1f, rand() % 20 - 10.f);
					Effect.BulltType = 20;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Piece_Effect"), &Effect);

					Effect.Target = RHandPos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);
					Effect.Pos = RHandPos;
					Effect.BulltType = 5;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

					Effect.Target = LHandPos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);
					Effect.Pos = LHandPos;
					Effect.BulltType = 6;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
				}
				for (int i = 0; i < 10; ++i)
				{

					Effect.Pos = LHandPos;
					Effect.Target = Effect.Pos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
					Effect.BulltType = 0;
					Effect.Speed = 30 + rand() % 10;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
					Effect.Pos = RHandPos;
					Effect.Target = Effect.Pos + _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
					Effect.BulltType = 8;
					Effect.Speed = 20 + rand() % 10;
					pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
				}
			}
			bLaser = true;
			/*if (AttackOn)
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
			}*/
			MeteorDelay = 0;
		}
	}


	_float4x4 MyWorld = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixInverse(&MyWorld, nullptr, &MyWorld);
	_float4x4 Target = pTargetTransform->Get_WorldMatrix();
	Target = Target*MyWorld;
	_float X = Target.m[3][0];

	if (X > 0)
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta / 2);
	else
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta / 2);


	MeteorDelay += fTimeDelta;
	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 7)
	{
		m_pStatusCom->Knockback_on();
		bLaser = false;
		AnFrame = 0;
		Set_Stay();
		Action = false;
		m_pStatusCom->Look_On();
	}

}

void CBoss_Lucifer::Attack(_float fTimeDelta)
{
	_float3 RH = _float3(-0.9f, 0.0f, 0.5f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	EFFECT4  Effect;
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		State = STATE_ATTACK;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		//m_vTF[4]->Go_Z(-3.f);
		//m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.05f, -0.6f, 0.5f));
		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.05f, -0.6f, 0.5f));
		AttackOn = true;
		Action = true;
		Attack_frange = frange;
		m_pStatusCom->Look_Off();
	}
	//반복


	if (AnFrame < 0.75)
	{
		m_pTransformCom->Go_Straight(fTimeDelta * Attack_frange);
		_float T = (0.75f - AnFrame) * 15;
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Go_Straight(T / 75);
		m_vTF[1]->Go_Straight(T / 150);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH * 3);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH*2.25);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		if (Page2)
		{

			Effect.Pos = RHandPos + _float3(rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f);
			Effect.Target = Effect.Pos;
			Effect.Target.y -= 3.f;
			Effect.BulltType = 4;
			Effect.Speed = 3;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

			Effect.Pos = RHandPos + _float3(rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f, rand() % 15 / 10 - 0.75f);
			Effect.Target = Effect.Pos;
			Effect.Target.y -= 3.f;
			Effect.BulltType = 17;
			Effect.Speed = 5;
			//pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

		}
		else
		{

			Effect.Pos = MyPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = Effect.Pos - m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7;
			Effect.Target.y += 3;
			Effect.Speed = Attack_frange / 2;
			Effect.BulltType = 5 + rand() % 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);


			Effect.Pos = MyPos + _float3(rand() % 5 - 2.5f, rand() % 5 - 2.5f, rand() % 5 - 2.5f);
			Effect.Target = Effect.Pos - m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7;
			Effect.Target.y += 3;
			Effect.Speed = Attack_frange / 2;
			Effect.BulltType = 5 + rand() % 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);

		}
	}

	else if (AnFrame >= 0.75 && AnFrame < 0.95)
	{
		if (Page2)
		{
			for (_int i = 0; i < 20; ++i)
			{
				Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
				Effect.Pos = RHandPos;
				Effect.BulltType = 17 + rand() % 3;
				Effect.Speed = 7;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			}
		}
		else
		{
			for (_int i = 0; i < 10; ++i)
			{
				Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
				Effect.Pos = RHandPos;
				Effect.BulltType = 0;
				Effect.Speed = 5;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			}
			for (_int i = 0; i < 10; ++i)
			{
				Effect.Target = RHandPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3 + _float3(rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f, rand() % 30 / 10 - 1.5f);
				Effect.Pos = RHandPos;
				Effect.BulltType = 3;
				Effect.Speed = 5;
				pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_CircleTarget"), &Effect);
			}
		}

		if (AttackOn == true)
		{
			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 1.f;
			StatusmDesc.fMaxStamina = 1100.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 40.f;
			StatusmDesc.fPower = 1.f;
			StatusmDesc.frange = 5.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 150;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3;
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_PlayerHitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);
			AttackOn = false;
		}

		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = -35 * (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH / 2);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}
	else if (AnFrame >= 0.95 && AnFrame < 1.5)
	{
		m_pTransformCom->Go_BackWard(fTimeDelta * 5);
		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - RH / 20);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - LH / 40);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}




	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 1.5)
	{
		AttackOn = true;
		Action = false;
		AnFrame = 0;
		Set_Stay();
		m_pStatusCom->Look_On();
	}

}

void CBoss_Lucifer::Falling(_float fTimeDelta)
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
		Action = true;
	}
	_float Time = fTimeDelta* 10.f;
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;
	DownAngle = DownAngle*Time;
	if (AnFrame < 10)
	{
		//머리

		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);


		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.03f *Time);
		//m_vTF[1]->Go_Straight(0.05f *Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
		m_pTransformCom->Go_Y(-Time / 30);
	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
		m_pTransformCom->Go_Y(-Time / 30);
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{
		//머리
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 2 * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 2 * LH);


		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.02f *Time);
		//m_vTF[1]->Go_BackWard(0.05f *Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f *Time));
		m_pTransformCom->Go_Y(-Time / 30);
	}
	else if (AnFrame >= 30 && AnFrame < 40)
	{
		//머리
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);

		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f *Time);
		m_vTF[1]->Go_Straight(0.0125f *Time);
		//m_pTransformCom->Go_Y(0.0025f *Time);
	}
	else if (AnFrame >= 40 && AnFrame < 50)
	{
		//머리
		//m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05 *Time);
		//손

		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);

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

void CBoss_Lucifer::Down(_float fTimeDelta)
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
		m_pStatusCom->Knockback_off();
	}
	_float Time = fTimeDelta*60.f;
	RH = RH*Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;

	if (AnFrame < 10)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.05f);

	}



	else if (AnFrame >= 20)
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.05f);

	}
	else
	{
		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_pTransformCom->Go_Y(-0.05f*Time);
	}





	AnFrame += Time;
	Action = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_UP;

		if (m_pStatusCom->Get_State() == CStatus::STATUS_STATE_DEAD)
			m_bDead = true;
	}

}

void CBoss_Lucifer::Up(_float fTimeDelta)
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
		m_pStatusCom->Knockback_on();
	}
	else
	{

	}


}






void CBoss_Lucifer::Move(_float fTimeDelta)
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

		}
		if (AnFrame >= 24)
		{


		}
		//몸통

	}

	if (AnFrame >= 30)
	{
		//발
		if (AnFrame < 54)
		{

		}
		if (AnFrame >= 54)
		{

		}
		//몸통


	}




	AnFrame += Time;


	Action = true;
}

void CBoss_Lucifer::Stay(_float fTimeDelta)
{

	_float4x4 Rot, Far, Loc, End;
	if (State != STATE_STAY || AnTime + 2.f < StandardAnTime)
	{
		Set_Stay();
		State = STATE_STAY;
		StandardAnTime = 0.f;
		AnTime = 0.f;

		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);

		m_vTF[7]->Turn(_float3(0.f, 0.f, 1.f), -1.f);

		m_vTF[8]->Turn(_float3(0.f, 0.f, 1.f), 1.f);

		m_vTF[9]->Turn(_float3(0.f, 0.f, 1.f), -1.f);

		m_vTF[10]->Turn(_float3(0.f, 0.f, 1.f), 1.f);

		m_vTF[11]->Turn(_float3(0.f, 0.f, 1.f), -1.f);

		m_vTF[12]->Turn(_float3(0.f, 0.f, 1.f), 1.f);

		AnTime = StandardAnTime;
	}
	StandardAnTime += fTimeDelta;

	_float Ang;

	if (AnTime + 1.5f > StandardAnTime)
	{
		Ang = -0.3f;
	}
	else
		Ang = 0.9f;


	m_pTransformCom->Go_Y(Ang *fTimeDelta);

	m_vTF[7]->Go_Up(Ang * fTimeDelta);
	m_vTF[7]->Turn(_float3(0.f, 0.f, 1.f), Ang *fTimeDelta);

	m_vTF[8]->Go_Up(Ang * fTimeDelta);
	m_vTF[8]->Turn(_float3(0.f, 0.f, 1.f), -Ang *fTimeDelta);

	m_vTF[9]->Go_Up(Ang * fTimeDelta);
	m_vTF[9]->Turn(_float3(0.f, 0.f, 1.f), Ang *fTimeDelta);

	m_vTF[10]->Go_Up(Ang * fTimeDelta);
	m_vTF[10]->Turn(_float3(0.f, 0.f, 1.f), -Ang *fTimeDelta);

	m_vTF[11]->Go_Up(Ang * fTimeDelta);
	m_vTF[11]->Turn(_float3(0.f, 0.f, 1.f), Ang *fTimeDelta);

	m_vTF[12]->Go_Up(Ang * fTimeDelta);
	m_vTF[12]->Turn(_float3(0.f, 0.f, 1.f), -Ang *fTimeDelta);

	//m_vTF[1]->Set_Speed(Ang * fTimeDelta);
	//m_vTF[1]->Go_Up(fTimeDelta);

	//m_vTF[2]->Set_Speed(Ang * fTimeDelta);
	//m_vTF[2]->Go_Up(fTimeDelta);

	//m_vTF[3]->Set_Speed(Ang * fTimeDelta);
	//m_vTF[3]->Go_Up(fTimeDelta);

	//m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + m_vTF[2]->Get_State(CSubTransform::STATE_UP)*1.4);
}

void CBoss_Lucifer::HandAction(_float fTimeDelta)
{
	_float3 RH = _float3(-0.9f, 0.0f, 0.5f)*fTimeDelta;
	_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
	_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
	EFFECT4  Effect;
	if (State != STATE_HAND)
	{
		Set_Stay();
		State = STATE_HAND;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;
		//m_vTF[4]->Go_Z(-3.f);
		//m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.05f, -0.6f, 0.5f));
		//m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.05f, -0.6f, 0.5f));
		AttackOn = true;
		Action = true;
		Attack_frange = frange;
		m_pStatusCom->Look_Off();
	}
	//반복


	if (AnFrame < 0.75)
	{

		_float T = (0.75f - AnFrame) * 15;
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T / 6));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 6));
		m_vTF[0]->Go_Straight(T / 75);
		m_vTF[1]->Go_Straight(T / 150);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH * 3);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH*2.25);

		m_vTF[7]->Go_Straight(T / 250);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[8]->Go_Straight(T / 250);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[9]->Go_Straight(T / 250);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[10]->Go_Straight(T / 250);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));
		m_vTF[11]->Go_Straight(T / 250);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T / 10));
		m_vTF[12]->Go_Straight(T / 250);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T / 10));

	}

	else if (AnFrame >= 0.75 && AnFrame < 0.95)
	{

		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = -35 * (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + RH);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + LH / 2);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}
	else if (AnFrame >= 0.95 && AnFrame < 1.5)
	{

		_float3 RH = _float3(-0.7f, -0.6f, 0.5f)*fTimeDelta;
		_float3 LH = _float3(0.6f, 0.5f, 0.2f)*fTimeDelta;
		_float3 OnB = _float3(0.0f, 3.f, 0.f)*fTimeDelta;
		_float T = (AnFrame - 0.75);
		RH = -RH * 250 * (AnFrame - 0.75);
		LH = -LH * 150 * (AnFrame - 0.75);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(T));
		m_vTF[0]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));

		m_vTF[0]->Go_Straight(T / 10);
		m_vTF[1]->Go_Straight(T / 20);

		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - RH / 20);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - LH / 40);

		m_vTF[7]->Go_Straight(T / 30);
		m_vTF[7]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[8]->Go_Straight(T / 30);
		m_vTF[8]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[9]->Go_Straight(T / 30);
		m_vTF[9]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[10]->Go_Straight(T / 30);
		m_vTF[10]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
		m_vTF[11]->Go_Straight(T / 30);
		m_vTF[11]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(T));
		m_vTF[12]->Go_Straight(T / 30);
		m_vTF[12]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(-T));
	}




	AnFrame += fTimeDelta;
	Action = true;
	if (AnFrame >= 1.5)
	{
		AttackOn = true;
		Action = false;
		AnFrame = 0;
		Set_Stay();
		m_pStatusCom->Look_On();
	}

}

int CBoss_Lucifer::ActionMoving(_float fTimeDelta)
{

	if (CEventMgr::Get_Instance()->Get_OnAir() == false)
		return 0;

	if (CEventMgr::Get_Instance()->Get_isInitSet(4) == true) {
		Meteor(fTimeDelta);
		return 1;
	}

	Stay(fTimeDelta);



	return 1;
}



CBoss_Lucifer * CBoss_Lucifer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Lucifer*	pInstance = new CBoss_Lucifer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_Lucifer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_Lucifer::Clone(void * pArg)
{
	CBoss_Lucifer*	pInstance = new CBoss_Lucifer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBoss_Lucifer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Lucifer::Free()
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

	Safe_Release(pGameInstance);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(pTargetTransform);
}



