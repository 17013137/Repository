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
	wsprintf(Scene_MSG[1], TEXT("������ ���õ� �ȿԴ�...?"));
	wsprintf(Scene_MSG[2], TEXT("��.... �Ҿƹ��� ���� �� ����������?"));
	wsprintf(Scene_MSG[3], TEXT("������ �� �������� �׷��� �̻��� �Ҹ��� �鸮���ʴ�....?"));
	wsprintf(Scene_MSG[4], TEXT("�׷��Կ� ���� �ֺ��� �ѷ����� �ðԿ�!"));

	// 2
	wsprintf(Scene_MSG[7], TEXT("�Ҿƹ���! ��� �ô� �������� �ֺ��� ���ƿ�!"));
	wsprintf(Scene_MSG[8], TEXT("������ �����Ǵ±���...."));
	wsprintf(Scene_MSG[9], TEXT("������ �����ǰ� ���⿡ ��� ���� �� ������ ���ƿ�,,,"));
	wsprintf(Scene_MSG[10], TEXT("�׷��� ������.. � ������ ã���� ����"));

	// 3
	wsprintf(Scene_MSG[13], TEXT("�̷� ���󿡼� �Ҿƹ����� �ճ� ���� �ٴϴ°� �ű��ϱ���..."));
	wsprintf(Scene_MSG[14], TEXT("�װ� �����Ҹ�����?"));
	wsprintf(Scene_MSG[15], TEXT("���� ������ ������â�Դϴ�... ���� �ٴϴ°� ���� ������ ���̴±���..."));
	wsprintf(Scene_MSG[16], TEXT("�Ҿƹ���! � ������ ã���� ���߰ھ��!"));

	wsprintf(Scene_MSG[19], TEXT("���� ���ð� ������ ���ݸ� �� ���� �� �� ���ƿ� �Ҿƹ��� ��������!"));
	wsprintf(Scene_MSG[20], TEXT("�׷� ���ݸ� �������ڲٳ�... ������ ��ٸ��ž�....."));

	wsprintf(Scene_MSG[23], TEXT("���� ����־��ٴ� �����̾�!"));
	wsprintf(Scene_MSG[24], TEXT("�����߾�����! �����̴� �Ҿƹ������� ���������� ����߾�...."));
	wsprintf(Scene_MSG[25], TEXT("�����̱���..."));
	wsprintf(Scene_MSG[26], TEXT("�̷����� �ƴϾ� �츰 ���� ���� ���������� ��!"));
	wsprintf(Scene_MSG[27], TEXT("?? �ֱ׷� ����"));
	wsprintf(Scene_MSG[28], TEXT("���� ���������� �̻��� ������� �̾߱� �ϴ� ���� �����! ���� �� ���谡 ����Ұž� !!"));
	wsprintf(Scene_MSG[29], TEXT("������ �㿡 �̵����� ������ ���� �������ٵ�...."));
	wsprintf(Scene_MSG[30], TEXT("���� �׷��� �߿��Ѱ� �ƴϾ� ���� �̰��� ��������!!"));

	wsprintf(Scene_MSG[34], TEXT("�ױ����� �ͼ��� ���� ���� �ݰ��׿�"));
	wsprintf(Scene_MSG[35], TEXT("�����Ҹ�����?"));
	wsprintf(Scene_MSG[36], TEXT("������ ǥ���� ���� ��ŵ鵵 ���� �����ʳ���? �� ������ ���谡 ����Ұſ���.."));
	wsprintf(Scene_MSG[37], TEXT("����̶��.... ���� ����� �������?"));
	wsprintf(Scene_MSG[38], TEXT("�������ִ� �׳༮�� ������ ����� �ٶ���־�.. �׳༮�� �����ʴ´ٸ�..."));
	wsprintf(Scene_MSG[39], TEXT(" ���� �����ſ���!!!	 ���� �������?????"));

	wsprintf(Scene_MSG[43], TEXT("���Ⱑ ��¥ �����̾�?? ���⿡ �׳༮�� �ִ°ž�....?"));
	wsprintf(Scene_MSG[44], TEXT("�׷��� ����! ���� �� �ȳ��Ҿ�!!"));
	wsprintf(Scene_MSG[45], TEXT("�����ϰŶ�...."));
	// ���� ī�޶󹫺�
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
