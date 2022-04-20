#include "stdafx.h"
#include "..\Public\Piece_Effect.h"
#include "GameInstance.h"

CPiece_Effect::CPiece_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPiece_Effect::CPiece_Effect(const CPiece_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPiece_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPiece_Effect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	MyType = *(EFFECT4*)pArg;
	vStartPos = MyType.Pos;
	vEndPos = MyType.Target;
	m_Texture = MyType.BulltType;
	vPos = MyType.Pos;
	vPos.y = 0.f;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	fMaxHeight = 1+ rand() % 30/15.f;
	//fTime =0.f;
	fMaxTime =rand() % 100/130.f;
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
	return S_OK;
}

_int CPiece_Effect::Tick(_float fTimeDelta)
{
	fTime += fTimeDelta;

	vPos.x = vStartPos.x + fV_X * fTime;

	vPos.y = vStartPos.y + (fV_Y * fTime) - (0.5f * fg * fTime * fTime);

	vPos.z = vStartPos.z + fV_Z * fTime;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	__super::Tick(fTimeDelta);
	m_pTransformCom->Scaling(-fTimeDelta / 5);
	if (fTime > 2*fMaxTime)
		return 1;
	return 0;
}

void CPiece_Effect::LateTick(_float fTimeDelta)
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

HRESULT CPiece_Effect::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_Texture)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPiece_Effect::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Tile"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

CPiece_Effect * CPiece_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPiece_Effect*	pInstance = new CPiece_Effect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPiece_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPiece_Effect::Clone(void * pArg)
{
	CPiece_Effect*	pInstance = new CPiece_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPiece_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPiece_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
