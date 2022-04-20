#include "stdafx.h"
#include "..\Public\Scene_UI2.h"
#include "GameInstance.h"
#include "PlayerCube.h"
#include "EventMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"

CScene_UI2::CScene_UI2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CScene_UI2::CScene_UI2(const CScene_UI2 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CScene_UI2::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_UI2::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = 100.0f;
	m_fY = 100.0f;

	m_bTab = false;
	m_bScene = false;

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pd3dxFont = NULL;

	HRESULT hr = D3DXCreateFont(m_pGraphic_Device, 30, 9, FW_HEAVY, 1, 0, DEFAULT_CHARSET, 0, 0, 0, TEXT("Arial"), &m_pd3dxFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, -500.f, 0.0001f));
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY*0.4f, 0.0001f));

	m_rScene_MSG = { 0,600,1280,700 };

	m_MSG_Cnt = 0;

	Safe_Release(pGameInstance);
	Set_Dialogue();
	return S_OK;
}

_int CScene_UI2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//if (CKeyMgr::Get_Instance()->Key_Down('V'))
	//{
	//	++m_MSG_Cnt;
	//}

	Set_Scene();
	return 0;
}

void CScene_UI2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);

}

HRESULT CScene_UI2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pVIBufferCom->Render();

	if (CEventMgr::Get_Instance()->Get_OnAir() == TRUE && flag == true)
	{
		m_pd3dxFont->DrawText(NULL, Scene_MSG[CEventMgr::Get_Instance()->Get_TotalScean()], -1, &m_rScene_MSG, DT_CENTER, 0xffffffff);
	}

	return S_OK;
}

void CScene_UI2::Set_Dialogue()
{
	// 1
	wsprintf(Scene_MSG[0], TEXT(""));
	wsprintf(Scene_MSG[1], TEXT("오빠는 오늘도 안왔니...?"));
	wsprintf(Scene_MSG[2], TEXT("네.... 할아버지 몸은 좀 괜찮으세요?"));
	wsprintf(Scene_MSG[3], TEXT("오늘은 좀 괜찮구나 그런데 이상한 소리가 들리지않니....?"));
	wsprintf(Scene_MSG[4], TEXT("그러게요 제가 주변을 둘러보고 올게요!"));

	// 2
	wsprintf(Scene_MSG[7], TEXT("할아버지! 어릴적 봤던 괴물들이 주변에 많아요!"));
	wsprintf(Scene_MSG[8], TEXT("오빠가 걱정되는구나...."));
	wsprintf(Scene_MSG[9], TEXT("오빠도 걱정되고 여기에 계속 있을 수 없을것 같아요,,,"));
	wsprintf(Scene_MSG[10], TEXT("그런것 같구나.. 어서 오빠를 찾으러 가자"));

	// 3
	wsprintf(Scene_MSG[13], TEXT("이런 세상에서 할아버지와 손녀 둘이 다니는게 신기하군요..."));
	wsprintf(Scene_MSG[14], TEXT("그게 무슨소리에요?"));
	wsprintf(Scene_MSG[15], TEXT("지금 세상은 엉망진창입니다... 둘이 다니는건 많이 위험해 보이는군요..."));
	wsprintf(Scene_MSG[16], TEXT("할아버지! 어서 오빠를 찾으러 가야겠어요!"));

	wsprintf(Scene_MSG[19], TEXT("저기 도시가 보여요 조금만 더 가면 될 것 같아요 할아버지 힘내세요!"));
	wsprintf(Scene_MSG[20], TEXT("그래 조금만 힘내보자꾸나... 오빠가 기다릴거야....."));

	wsprintf(Scene_MSG[23], TEXT("오빠 살아있었다니 다행이야!"));
	wsprintf(Scene_MSG[24], TEXT("무사했었구나! 다행이다 할아버지까지 데려오느라 고생했어...."));
	wsprintf(Scene_MSG[25], TEXT("다행이구나..."));
	wsprintf(Scene_MSG[26], TEXT("이럴때가 아니야 우린 빨리 여길 빠져나가야 해!"));
	wsprintf(Scene_MSG[27], TEXT("?? 왜그래 오빠"));
	wsprintf(Scene_MSG[28], TEXT("내가 숨어있을때 이상한 사람들이 이야기 하는 것을 들었어! 이제 곧 세계가 멸망할거야 !!"));
	wsprintf(Scene_MSG[29], TEXT("하지만 밤에 이동하지 않으면 좀비가 몰려올텐데...."));
	wsprintf(Scene_MSG[30], TEXT("지금 그런게 중요한게 아니야 빨리 이곳을 떠나야해!!"));

	wsprintf(Scene_MSG[34], TEXT("죽기전에 익숙한 얼굴을 보니 반갑네요"));
	wsprintf(Scene_MSG[35], TEXT("무슨소리세요?"));
	wsprintf(Scene_MSG[36], TEXT("오빠의 표정을 보니 당신들도 이제 알지않나요? 좀 있으면 세계가 멸망할거예요.."));
	wsprintf(Scene_MSG[37], TEXT("멸망이라니.... 막을 방법이 없을까요?"));
	wsprintf(Scene_MSG[38], TEXT("지옥에있는 그녀석은 세계의 멸망을 바라고있어.. 그녀석을 막지않는다면..."));
	wsprintf(Scene_MSG[39], TEXT(" 제가 막을거에요!!!	 어디로 가면되죠?????"));

	wsprintf(Scene_MSG[43], TEXT("여기가 진짜 지옥이야?? 저기에 그녀석이 있는거야....?"));
	wsprintf(Scene_MSG[44], TEXT("그런것 같아! 이제 얼마 안남았어!!"));
	wsprintf(Scene_MSG[45], TEXT("조심하거라...."));
	// 지옥 카메라무브
}

HRESULT CScene_UI2::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), m_Current_Level, TEXT("Prototype_Component_Texture_Scene_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CScene_UI2::Set_Scene()
{
	if (true == CEventMgr::Get_Instance()->Get_OnAir())
	{
		if (false == m_bScene)
		{
			m_pTransformCom->Go_Y(5.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > -350)
			{
				flag = true;
				m_bScene = true;
			}
		}
	}
	else
	{
		if (true == m_bScene)
		{
			m_pTransformCom->Go_Y(-5.f);
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -500)
			{
				m_bScene = false;
			}
		}
	}
}


CScene_UI2 * CScene_UI2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_UI2*	pInstance = new CScene_UI2(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CScene_UI2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScene_UI2::Clone(void * pArg)
{
	CScene_UI2*	pInstance = new CScene_UI2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CScene_UI2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_UI2::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
