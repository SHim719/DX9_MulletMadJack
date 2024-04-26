#include "UI_Dialogue.h"
#include "Ui_Pos.h"


CUI_Dialogue::CUI_Dialogue(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUI_Dialogue::CUI_Dialogue(const CUI_Dialogue& rhs)
	:CUi(rhs)
{
}

HRESULT CUI_Dialogue::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	m_UiDesc.m_fX = 130.f;
	m_UiDesc.m_fY = -260.f;
	m_UiDesc.m_fSizeX = m_OriginScale.x;
	m_UiDesc.m_fSizeY = m_OriginScale.y;

	m_pTransformCom->Set_Pos({ m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f });
	m_pTransformCom->Set_Scale({ m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f });

	m_tTextRect.left = long(m_UiDesc.m_fX + (_float)g_iWinSizeX * 0.5f - m_UiDesc.m_fSizeX * 0.5f ) + 15;
	m_tTextRect.top = long((_float)g_iWinSizeY * 0.5f - m_UiDesc.m_fY - m_UiDesc.m_fSizeY * 0.5f) + 15;
	m_tTextRect.right = m_tTextRect.left + (long)m_UiDesc.m_fSizeX - 10;
	m_tTextRect.bottom = m_tTextRect.top + (long)m_UiDesc.m_fSizeY - 10;

	return S_OK;
}

HRESULT CUI_Dialogue::Initialize(void* pArg)
{
	return S_OK;
}

void CUI_Dialogue::PriorityTick(_float fTimeDelta)
{
}

void CUI_Dialogue::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CUI_Dialogue::APPEAR:
		State_Appear(fTimeDelta);
		break;
	case CUI_Dialogue::ONGOING:
		State_OnGoing(fTimeDelta);
		break;
	case CUI_Dialogue::FINISH:
		State_Finish(fTimeDelta);
		break;
	}
}

void CUI_Dialogue::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Dialogue::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	if (ONGOING == m_eState && m_strDialogue.length() > 0)
		m_pGameInstance->Print_Text(info);
	return S_OK;
}


HRESULT CUI_Dialogue::Add_Components(void* pArg)
{
	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;


	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;


	return S_OK;

}

HRESULT CUI_Dialogue::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Dialogue_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUI_Dialogue::Start_Dialogue(_float fTextDelay, _float fDialogueDelay)
{
	m_fTextDelay = fTextDelay;
	m_fTextDelayAcc = 0.f;
	m_fDialogueDelay = fDialogueDelay;
	m_fDialogueDelayAcc = 0.f;
	m_strDialogue.clear();
	m_iNowDialogueIndex = 0;
	m_iTextIndex = 0;
	m_eState = APPEAR;
}

CUI_Dialogue* CUI_Dialogue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Dialogue* pInstance = new CUI_Dialogue(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUI_Dialogue Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialogue::State_Appear(_float fTimeDelta)
{
	m_fScalingY += m_fScalingSpeed * fTimeDelta;
	if (m_fScalingY >= m_OriginScale.y)
	{
		m_fScalingY = m_OriginScale.y;
		m_eState = ONGOING;
		if (m_strVoiceTag.size())
			m_pGameInstance->Play(m_strVoiceTag + L"0", false);
	}
	m_pTransformCom->Set_Scale({ m_UiDesc.m_fSizeX, m_fScalingY, 1.f });
}

void CUI_Dialogue::State_OnGoing(_float fTimeDelta)
{
	if (m_iNowDialogueIndex >= m_vecDialogues.size())
	{
		m_eState = FINISH;
		return;
	}

	if (m_iTextIndex >= m_vecDialogues[m_iNowDialogueIndex].size())
	{
		if (m_strVoiceTag.size() && false == m_pGameInstance->Is_Playing(m_strVoiceTag + to_wstring(m_iNowDialogueIndex)))
		{
			m_iNowDialogueIndex++;
			m_iTextIndex = 0;
			m_fDialogueDelayAcc = 0.f;
			if (m_iNowDialogueIndex < m_vecDialogues.size())
				m_strDialogue.clear();

			m_pGameInstance->Play(m_strVoiceTag + to_wstring(m_iNowDialogueIndex), false);
		}
		else if (!m_strVoiceTag.size())
		{
			m_fDialogueDelayAcc += fTimeDelta;
			if (m_fDialogueDelayAcc >= m_fDialogueDelay)
			{
				m_iNowDialogueIndex++;
				m_iTextIndex = 0;
				m_fDialogueDelayAcc = 0.f;
				if (m_iNowDialogueIndex < m_vecDialogues.size())
					m_strDialogue.clear();
			}
		}
		return;
	}

	m_fTextDelayAcc += fTimeDelta;
	if (m_fTextDelayAcc >= m_fTextDelay)
	{
		m_fTextDelayAcc = 0.f;
		m_strDialogue.push_back(m_vecDialogues[m_iNowDialogueIndex][m_iTextIndex++]);
	}

	info.Text = m_strDialogue.c_str();
	info.Rect = m_tTextRect;
	info.Length = (int)m_strDialogue.size();
}

void CUI_Dialogue::State_Finish(_float fTimeDelta)
{
	m_fDialogueDelayAcc += fTimeDelta;
	if (m_fDialogueDelayAcc < m_fDialogueDelay)
		return;


	m_fScalingY -= m_fScalingSpeed * fTimeDelta;
	if (m_fScalingY <= 0.f)
	{
		m_fScalingY = 0.f;
		m_bActive = false;
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		D3DXMatrixIdentity(&worldMatrix);
		m_pTransformCom->Set_Pos({ m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f });
		m_pTransformCom->Set_Scale({ m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f });
		m_fDialogueDelayAcc = 0.f;
		return;
	}
	m_pTransformCom->Set_Scale({ m_UiDesc.m_fSizeX, m_fScalingY, 1.f });
}

void CUI_Dialogue::Free()
{
	__super::Free();
}