#include "stdafx.h"
#include "..\Public\Item.h"
#include "GameInstance.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CItem::CItem(const CItem & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CItem::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	MyType = *(BULLETTYPE*)pArg;
	vStartPos = MyType.Pos;
	vEndPos = MyType.Target;
	m_Texture = MyType.BulltType;
	vPos = MyType.Pos;
	vPos.y = 0.f;
	Falling = true;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Scaled(_float3(0.9f, 0.9f, 0.9f));
	Item = (ITEM_TYPE)MyType.BulltType;
	if (Item >= ITEM_END)
	{
		Item = (ITEM_TYPE)(rand() % ITEM_END);
	}

	if(Item== COIN1|| Item == COIN2)
	{
		m_pTransformCom->Scaled(_float3(1.f, 0.5f, 0.5f));
	}
	
	

	
	
	fMaxHeight = 1 + rand() % 30 / 30.f;
	//fTime =0.f;
	fMaxTime = 0.2f;
	fEndHeight = vEndPos.y - vStartPos.y;
	fHeight = fMaxHeight - vStartPos.y;
	fg = 2 * fHeight / (fMaxTime * fMaxTime);
	fV_Y = sqrtf(2 * fg * fHeight);
	float a = fg;
	float b = -2 * fV_Y;
	float c = 2 * fEndHeight;
	fEndTime = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);
	fV_X = -(vStartPos.x - vEndPos.x) / fEndTime;
	fV_Z = -(vStartPos.z - vEndPos.z) / fEndTime;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_TargetTransformCom = (CTransform*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Transform"));
	m_TargetStatusCom = (CStatus*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Status"));
	m_TargetInventoryCom = (CInventory*)pGameInstance->Get_Component(m_Current_Level, TEXT("Layer_Player"), TEXT("Com_Inventory"));

	Safe_Release(pGameInstance);
	return S_OK;
}

_int CItem::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	_float3 vLook = m_TargetTransformCom->Get_State(CTransform::STATE_POSITION) -  m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float Length = D3DXVec3Length(&vLook);
	if (Length>5.f)
	{
		

		if (Falling == true)
		{
			fTime += fTimeDelta;

			vPos.x = vStartPos.x + fV_X * fTime;

			vPos.y = vStartPos.y + (fV_Y * fTime) - (0.5f * fg * fTime * fTime);

			vPos.z = vStartPos.z + fV_Z * fTime;
		}
		else
		{
			if ((_int)LifeTime % 2 == 0)
			{
				vPos.y += fTimeDelta/2;
			}
			else
			{
				vPos.y -= fTimeDelta/2;
			}



		}
	}
	else if (Length <= 5.f && Length > 0.2f)
	{
		//_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos= vPos + (*D3DXVec3Normalize(&vLook, &vLook)*(pow(5-Length,2)) * fTimeDelta/2);
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos + (vLook*pow(fTimeDelta,3)));
	}
	else
	{
		switch (Item)
		{
		case Client::CItem::RIFLEAMMO:
			m_TargetInventoryCom->GetItem(CInventory::RIFLEAMMO, 10 + rand() % 30);
			break;
		case Client::CItem::SHOTGUNAMMO:
			m_TargetInventoryCom->GetItem(CInventory::SHOTGUNAMMO, 3 + rand() % 7);
			break;
		case Client::CItem::PISTOLAMMO:
			m_TargetInventoryCom->GetItem(CInventory::PISTOLAMMO, 5 + rand() % 10);
			break;
		case Client::CItem::MEDICKIT:
			m_TargetInventoryCom->GetItem(CInventory::MEDICKIT, 1);
			break;
		case Client::CItem::COIN1:
			m_TargetInventoryCom->GetItem(CInventory::COIN, 50000);
			break;
		case Client::CItem::COIN2:
			m_TargetInventoryCom->GetItem(CInventory::COIN, 10000);
			break;
		case Client::CItem::FOOD1:
			m_TargetStatusCom->Heal(10.f);
			break;
		case Client::CItem::FOOD2:
			m_TargetStatusCom->Heal(10.f);
			break;
		case Client::CItem::FOOD3:
			m_TargetStatusCom->Heal(10.f);
			break;
		case Client::CItem::FOOD4:
			m_TargetStatusCom->Heal(10.f);
			break;
		case Client::CItem::FOOD5:
			m_TargetStatusCom->Heal(20.f);
			break;
		case Client::CItem::DRINK:
			m_TargetStatusCom->Heal_Stamina(40.f);
			break;
		case Client::CItem::POISON:
			m_TargetStatusCom->Heal(-30.f);
			break;
		case Client::CItem::ITEM_END:
			break;
		default:
			break;
		}






		return 1;
	}





	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	LifeTime -= fTimeDelta;

	if (vPos.y <= 0.8f)
	{
		vPos.y = 0.8f;
		Falling = false;
	}
		


	if (LifeTime < 0)
		return 1;


	return 0;
}

void CItem::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * vScale.z);




	//m_pTransformCom->Scaling(-fTimeDelta);




	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_EFFECT, this);
}

HRESULT CItem::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Item)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CItem::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Item"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem*	pInstance = new CItem(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem*	pInstance = new CItem(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
