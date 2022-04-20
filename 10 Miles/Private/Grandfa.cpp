#include "stdafx.h"
#include "..\Public\Grandfa.h"
#include "GameInstance.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"

CNPC_Grandfa::CNPC_Grandfa(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CNPC_Grandfa::CNPC_Grandfa(const CNPC_Grandfa & rhs)
	: CGameObject(rhs)

{
}

HRESULT CNPC_Grandfa::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&TargetPos, sizeof(TargetPos));



	return S_OK;
}

HRESULT CNPC_Grandfa::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;


	CurrentHand = HAND_EMPTY;

	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, -vLook);

	m_col_Range = 1.f;
	m_col_Size = { 1.f, 1.f };

	return S_OK;
}

_int CNPC_Grandfa::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = 0.6f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (key == false)
	{
		Stay(fTimeDelta);
	}

	//CurrentAngle = ANGLE_LEFT;
	//마우스 각도와 이동각도 사이각 구하기

	if (m_pStatusCom->Status_Tick(fTimeDelta) == CStatus::STATUS_STATE_HIT)
	{
		EFFECTTYPE m_Effect_Type;

		m_Effect_Type.Type = (_int)(fTimeDelta * 1000) % 10;
		m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_Effect_Type.Target = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Blood_Effect"), &m_Effect_Type)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);


	return 0;
}

void CNPC_Grandfa::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//파츠별 위치 맞추기-------------------------------------------------------------
	_float3 Right;
	_float3 Up;
	_float3 Look;
	_float3 Pos;

	//어깨 위치 몸통에 맞추기
	Right = *D3DXVec3Normalize(&Right, &m_vTF[1]->Get_State(CSubTransform::STATE_RIGHT));
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));

	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[6]->Set_State(CSubTransform::STATE_POSITION, Pos + (Right * 0.4f) + (Up * 0.25f));
	m_vTF[7]->Set_State(CSubTransform::STATE_POSITION, Pos + -(Right * 0.4f) + (Up * 0.25f));

	//머리 위치 몸통에 맞추기
	Up = *D3DXVec3Normalize(&Up, &m_vTF[1]->Get_State(CSubTransform::STATE_UP));
	Pos = m_vTF[1]->Get_State(CSubTransform::STATE_POSITION);
	m_vTF[0]->Set_State(CSubTransform::STATE_POSITION, Pos + Up / 1.25f);



	m_vRD[1]->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CNPC_Grandfa::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (CEventMgr::Get_Instance()->Get_OnAir() == true)
		return S_OK;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(11)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	for (int i = 0; i < 10; ++i)
	{
		m_vTF[i]->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}

	Set_Part(2, 1, m_vTF[6]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part(3, 1, m_vTF[7]->Get_StateWorld(CSubTransform::STATE_POSITION));
	Set_Part2(m_vTF[4], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part2(m_vTF[5], m_vTF[8]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_RIGHT) / 1.8);
	Set_Part(9, 1, m_vTF[1]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[8]->Get_StateWorld(CSubTransform::STATE_LOOK) / 1.8);

	
	if (HandState == HAND_PISTOL)
	{
		m_vTFPistol->Set_StateWorld(CSubTransform::STATE_POSITION, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));

		m_vTFPistol->Look(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_LOOK));
		Set_Part2(m_vTFPistol, m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
		m_vTFPistol->LookAt(m_vTF[3]->Get_State(CSubTransform::STATE_POSITION));
		Set_Part2(m_vTF[3], m_vTFPistol->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTFPistol->Get_StateWorld(CSubTransform::STATE_LOOK) / 3);
	}


	//어깨 각도 손으로 맞추기
	m_vTF[6]->LookAtWorld(m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));
	m_vTF[7]->LookAtWorld(m_vTF[3]->Get_StateWorld(CSubTransform::STATE_POSITION));

	//골반 각도 발에 맞추기
	_float3 RF = m_vTF[4]->Get_StateWorld(CSubTransform::STATE_POSITION) + m_vTF[4]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	_float3 LF = m_vTF[5]->Get_StateWorld(CSubTransform::STATE_POSITION) - m_vTF[5]->Get_StateWorld(CSubTransform::STATE_RIGHT) * 2;
	if (State != STATE_ROLL)
	{
		m_vTF[8]->LookRight(RF - LF);
	}



	for (int i = 0; i < 8; ++i)
	{

		if (FAILED(m_vTT[i]->Bind_OnGraphicDevice(i+9)))
			return E_FAIL;

		if (FAILED(m_vTF[i]->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVI[i]->Render();
	}



	if (HandState == HAND_PISTOL)
	{

		if (FAILED(m_vTTPistol->Bind_OnGraphicDevice(3)))
			return E_FAIL;

		if (FAILED(m_vTFPistol->Bind_OnGraphicDevice()))
			return E_FAIL;

		m_vVIPistol->Render();
	}

	return S_OK;
}

HRESULT CNPC_Grandfa::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_NPC_Shop"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	CStatus::STATUSDESC		StatusmDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	StatusmDesc.fMaxHp = 100.f;
	StatusmDesc.fMaxStamina = 100.f;
	StatusmDesc.fMaxStungauge = 100.f;
	StatusmDesc.fDamage = 100.f;
	StatusmDesc.Attacktype = CStatus::ATTACK_KNOCKBACK;
	StatusmDesc.Transform_Com = m_pTransformCom;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Status"), LEVEL_STATIC, TEXT("Prototype_Component_Status"), (CComponent**)&m_pStatusCom, &StatusmDesc)))
		return E_FAIL;
	CInventory::INVENTORYDESC		InventorymDesc;
	ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
	InventorymDesc.RifleAmmo = 100;
	InventorymDesc.ShotgunAmmo = 30;
	InventorymDesc.PistolAmmo = 50;
	InventorymDesc.Medickit = 10;
	InventorymDesc.Grenade = 3;
	InventorymDesc.Coin = 100;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Inventory"), LEVEL_STATIC, TEXT("Prototype_Component_Inventory"), (CComponent**)&m_pInventoryCom, &InventorymDesc)))
		return E_FAIL;
	//그림자




	//무기(Pistol)
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_SubTransform"), (CComponent**)&m_vTFPistol, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer_Pistol"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_vRDPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer_Pistol"), m_Current_Level, TEXT("Prototype_Component_VIBuffer_Pistol"), (CComponent**)&m_vVIPistol)))
		return E_FAIL;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Pistol"), m_Current_Level, TEXT("Prototype_Component_Texture_Weapon"), (CComponent**)&m_vTTPistol)))
		return E_FAIL;


	//각 부위별 컴퍼넌트
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
		if (FAILED(__super::SetUp_Components(szPath4[i], m_Current_Level, TEXT("Prototype_Component_Texture_NPC_Shop"), (CComponent**)&m_CubeTT)))
			return E_FAIL;
		m_vTT.push_back(m_CubeTT);
	}
	Set_Stay();







	return S_OK;
}



HRESULT CNPC_Grandfa::SetUp_OnTerrain()
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
	if (State == STATE_ROLL)
		vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y + 1.f;
	else
	{
		if (RF < LF)
		{
			vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - RF + 0.5f;
		}

		else if (RF >= LF)
		{
			vPosition.y = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition).y - LF + 0.5f;
		}

	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);



	Safe_Release(pGameInstance);
	return S_OK;
}

void CNPC_Grandfa::Set_Part(int Part, int Local, _float3 Pos)
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

void CNPC_Grandfa::Set_Part2(CSubTransform* Part, _float3 Pos)
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

void CNPC_Grandfa::Set_Stay()
{
	_float4x4 i;
	D3DXMatrixIdentity(&i);



	//머리
	m_vTF[0]->Set_LocalMatrix(i);
	m_vTF[0]->Scaled(_float3(0.5f, 0.5f, 0.5f));


	if (HandState == HAND_EMPTY)
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
	else if (HandState == HAND_RIFLE)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(50.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_SHOTGUN)
	{
		m_vTF[0]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(-5.f));
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.3f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), D3DXToRadian(20.f));
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(50.f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, 0.f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(0.0f, 0.f, 1.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_PISTOL)
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
	else if (HandState == HAND_WEAPON)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		//m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//m_vTF[2]->Turn(_float3(0.f, 1.f, 0.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else if (HandState == HAND_GRENADE)
	{
		//몸통
		m_vTF[1]->Set_LocalMatrix(i);
		m_vTF[1]->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.55f, 0.f));
		m_vTF[1]->Scaled(_float3(0.7f, 0.8f, 0.4f));
		//오른손
		m_vTF[2]->Set_LocalMatrix(i);
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, _float3(0.3f, -0.4f, 0.5f));
		m_vTF[2]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[2]->Turn(_float3(0.f, 0.f, 1.f), 5.f);
		//왼손
		m_vTF[3]->Set_LocalMatrix(i);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, _float3(-0.3f, -0.4f, 0.f));
		m_vTF[3]->Scaled(_float3(0.3f, 0.3f, 0.3f));
		m_vTF[3]->Turn(_float3(0.f, 0.f, 1.f), -5.f);
	}
	else
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

	m_vTF[9]->Scaled(_float3(0.5f, 0.6f, 0.5f));

	m_vTFPistol->Scaled(_float3(1.5f, 1.5f, 2.f));
	m_vTFPistol->Set_State(CSubTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	//m_vTFTimber->Turn(_float3(0.f, 1.f, 0.f), -0.5f);

}

void CNPC_Grandfa::Run(_float fTimeDelta)
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
	if (State != STATE_RUN || CurrentHand != HandState || AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_RUN;
		CurrentHand = HandState;

		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;


		if (HandState == HAND_EMPTY)
		{
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
		}

		else if (HandState == HAND_PISTOL)
		{
			//몸통
			m_vTF[1]->Set_Speed(0.1f);
			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
			m_vTF[1]->Go_Straight(1.f);
			m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);
		}


		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.9f);

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);

	}

	_float Time = fTimeDelta* 60.f;
	RH = RH *Time;
	LH = LH *Time;
	RF = RF *Time;
	OnF = OnF  *Time;
	UpF = UpF  *Time;
	DownF = DownF*Time;
	DownFCompensation = DownFCompensation*Time;

	//반복
	if (AnFrame < 15)
	{
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		}
		else if (CurrentHand == HAND_WEAPON)
		{

			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
		}
		//발
		if (AnFrame < 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.15f*Time);
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
		}
		if (AnFrame >= 12)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.08f*Time);
	}

	if (AnFrame >= 15)
	{
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		}

		else if (CurrentHand == HAND_WEAPON)
		{

			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
		}
		//발
		if (AnFrame < 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF));
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.15f*Time);
		}
		if (AnFrame >= 27)
		{
			m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + (UpF));
			m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (DownF));
			m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
			m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.3f*Time);
		}
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.08f*Time);
	}




	AnFrame += Time;


	key = true;
}

void CNPC_Grandfa::Walk(_float fTimeDelta)
{
	//움직이는 속도
	_float3 RH = _float3(-0.0015f, 0.01f, 0.01f);
	_float3 LH = _float3(0.0015f, 0.01f, 0.01f);
	_float3 RF = _float3(-0.004f, 0.03f, 0.03f);
	_float3 OnF = _float3(0.0f, 0.0f, 0.05f);
	_float3 UpF = _float3(0.0f, 0.03f, -0.02f);
	_float3 DownF = _float3(0.0f, -0.03f, 0.02f);
	_float3 DownFCompensation = _float3(0.0f, 0.09f, 0.0f);

	//준비
	if (State != STATE_WALK || CurrentHand != HandState || AnFrame >= 30)
	{
		Set_Stay();
		State = STATE_WALK;
		CurrentHand = HandState;
		StandardAnTime = 0.f;
		AnTime = 0.f;
		AnFrame = 0;

		if (CurrentHand == HAND_EMPTY)
		{
			//손
			m_vTF[2]->Set_Speed(0.1f);
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 20);

			m_vTF[3]->Set_Speed(0.1f);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 10);

			m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.1f);
		}


		else if (CurrentHand == HAND_PISTOL)
		{

		}

		//몸통
		m_vTF[1]->Set_Speed(0.1f);

		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.6f);

		//발
		m_vTF[4]->Set_Speed(0.1f);
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - (OnF * 6) + (UpF * 3));

		m_vTF[5]->Set_Speed(0.1f);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + (OnF * 6) + (DownF * 3) + DownFCompensation);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.3f);


		_float Time = fTimeDelta * 60;
		RH = RH *Time;
		LH = LH *Time;
		RF = RF *Time;
		OnF = OnF *Time;
		UpF = UpF *Time;
		DownF = DownF*Time;
		DownFCompensation = DownFCompensation *Time;

		//반복

		if (AnFrame < 15)
		{
			//손
			if (CurrentHand == HAND_EMPTY)
			{
				m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH * 2);
				m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH * 2);
			}


			else if (CurrentHand == HAND_PISTOL)
			{

			}

			//발
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


			if (AnFrame >= 15)
			{
				//손
				if (CurrentHand == HAND_EMPTY)
				{
					m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH * 2);
					m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH * 2);
				}

				else if (CurrentHand == HAND_PISTOL)
				{

				}


				//발
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
		}

		AnFrame += Time;


		key = true;
	}
}









void CNPC_Grandfa::Falling(_float fTimeDelta)
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
	_float Time = fTimeDelta*60.f;
	RH = RH * Time;
	LH = LH* Time;
	RF = RF* Time;
	LF = LF* Time;
	DownAngle = DownAngle*Time;


	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
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
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(-3.0f*Time));
	}
	else if (AnFrame >= 30 && AnFrame < 40)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH / 4);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH / 4);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF / 4);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF / 4);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.01f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.02f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.0125f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.0125f*Time);
		m_vTF[1]->Go_Straight(0.0125f*Time);
		m_pTransformCom->Go_Y(0.0025f*Time);
	}
	else if (AnFrame >= 40 && AnFrame < 50)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.05);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH / 4);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH / 4);
		}
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
	key = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0;
		State = STATE_DOWN;

	}
	else
	{

	}


}


void CNPC_Grandfa::Down(_float fTimeDelta)
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

	if (AnFrame < 10)
	{
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


	_float Time = fTimeDelta * 60;
	AnFrame*Time;
	key = true;

	if (AnFrame >= 50)
	{
		AnFrame = 0.f;
		State = STATE_UP;

	}




}

void CNPC_Grandfa::Up(_float fTimeDelta)
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
	RH = RH *Time;
	LH = LH*Time;
	RF = RF*Time;
	LF = LF*Time;
	DownAngle = DownAngle*Time;

	if (AnFrame < 10)
	{
		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) + RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.08f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.05f*Time);
		m_vTF[1]->Go_Straight(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));

	}

	else if (AnFrame >= 10 && AnFrame < 20)
	{
		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));
	}
	else if (AnFrame >= 20 && AnFrame < 30)
	{

		//머리
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.1*Time);
		//손
		if (CurrentHand == HAND_EMPTY)
		{
			m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - RH);
			m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - LH);
		}
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.06f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.05f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.05f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

		m_pTransformCom->Turn(DownAngle, D3DXToRadian(3.0f*Time));
	}


	AnFrame += Time;
	key = true;

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



void CNPC_Grandfa::Stay(_float fTimeDelta)
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

	if (AnTime + 3.f < StandardAnTime)
		AnTime = StandardAnTime;

	_float Ang;

	if (AnTime + 1.5f > StandardAnTime)
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



void CNPC_Grandfa::Attack(_float fTimeDelta)
{

	_float3 RH = _float3(-0.05f, 0.03f, 0.05f);
	_float3 LH = _float3(0.02f, 0.05, 0.01f);
	_float3 RF = _float3(-0.01f, 0.03f, 0.02f);
	_float3 LF = _float3(0.01f, 0.05f, 0.04f);
	HAND SubHandState = HandState;
	if (State != STATE_ATTACK)
	{
		Set_Stay();
		m_isChase = (_bool)m_pTransformCom->LookAt(m_vTargetPos);
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
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) + LH);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), -0.07f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) + RF);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) + LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), -0.06f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), -0.04f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), -0.04f*Time);
		m_vTF[1]->Go_BackWard(0.05f*Time);

	}



	else if (AnFrame >= 20 && AnFrame < 30)
	{
		//머리
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.001f*Time);
		//손
		m_vTF[2]->Set_State(CSubTransform::STATE_POSITION, m_vTF[2]->Get_State(CSubTransform::STATE_POSITION) - 1.8f * RH);
		m_vTF[3]->Set_State(CSubTransform::STATE_POSITION, m_vTF[3]->Get_State(CSubTransform::STATE_POSITION) - 0.5f * RH);
		m_vTF[2]->Turn(_float3(1.f, 0.f, 0.f), 0.15f*Time);
		//발
		m_vTF[4]->Set_State(CSubTransform::STATE_POSITION, m_vTF[4]->Get_State(CSubTransform::STATE_POSITION) - RF * 3);
		m_vTF[4]->Go_Z(0.01f*Time);
		m_vTF[5]->Set_State(CSubTransform::STATE_POSITION, m_vTF[5]->Get_State(CSubTransform::STATE_POSITION) - 1.8 * LF);
		m_vTF[4]->Turn(_float3(1.f, 0.f, 0.f), 0.14f*Time);
		m_vTF[5]->Turn(_float3(1.f, 0.f, 0.f), 0.1f*Time);
		//몸통
		m_vTF[1]->Turn(_float3(0.f, 1.f, 0.f), 0.1f*Time);
		m_vTF[1]->Turn(_float3(1.f, 0.f, 0.f), 0.17f*Time);
		m_vTF[1]->Go_Straight(0.3f*Time);

		m_vTF[3]->Set_StateWorld((CSubTransform::STATE_POSITION), m_vTF[2]->Get_StateWorld(CSubTransform::STATE_POSITION));


	}
	else if (AnFrame >= 30 && AnFrame < 45)
	{
		if (AttackOn)
		{
			CGameInstance*		pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);
			EFFECTTYPE m_Effect_Type;
			m_Effect_Type.Type = 7;
			m_Effect_Type.Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2;
			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_BulletEffect"), &m_Effect_Type);

			CStatus::STATUSDESC		StatusmDesc;
			ZeroMemory(&StatusmDesc, sizeof(CStatus::STATUSDESC));
			StatusmDesc.fMaxHp = 5.f;
			StatusmDesc.fMaxStamina = 1200.f;
			StatusmDesc.fMaxStungauge = 100.f;
			StatusmDesc.fDamage = 10.f;
			StatusmDesc.fPower = 0.2f;
			StatusmDesc.frange = 3.f;
			StatusmDesc.fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2;
			StatusmDesc.fSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			StatusmDesc.Attacktype = CStatus::ATTACK_PUSH;

			StatusmDesc.Transform_Com = m_pTransformCom;
			m_pTransformCom->Go_BackWard(0.01f);

			pGameInstance->Add_GameObjectToLayer(m_Current_Level, TEXT("Layer_HitBox"), TEXT("Prototype_GameObject_HitCircle"), &StatusmDesc);

			Safe_Release(pGameInstance);
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
		HandState = SubHandState;
		Set_Stay();
	}

	key = true;


}

CNPC_Grandfa * CNPC_Grandfa::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CNPC_Grandfa*	pInstance = new CNPC_Grandfa(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CNPC_Grandfa"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNPC_Grandfa::Clone(void * pArg)
{
	CNPC_Grandfa*	pInstance = new CNPC_Grandfa(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CNPC_Grandfa"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_Grandfa::Free()
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

	Safe_Release(m_vTFPistol);
	Safe_Release(m_vVIPistol);
	Safe_Release(m_vTTPistol);
	Safe_Release(m_vRDPistol);




}

