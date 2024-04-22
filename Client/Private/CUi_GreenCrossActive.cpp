#include "CUi_GreenCrossActive.h"
#include "GameInstance.h"


CUi_GreenCrossActive::CUi_GreenCrossActive(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_GreenCrossActive::CUi_GreenCrossActive(const CUi_GreenCrossActive& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_GreenCrossActive::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_GreenCrossActive::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_GreenCrossActive::PriorityTick(_float fTimeDelta)
{
}

void CUi_GreenCrossActive::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	if (m_fActiveTime < 0)
	{
		Set_Active(false);
	}
	CreateGreenCross(fTimeDelta);
}

void CUi_GreenCrossActive::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_GreenCrossActive::Render()
{
	return S_OK;
}

HRESULT CUi_GreenCrossActive::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return S_OK;
}

void CUi_GreenCrossActive::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 1.5f;
}

void CUi_GreenCrossActive::Initialize_Set_Size()
{
}

void CUi_GreenCrossActive::Initialize_Set_Speed()
{
}

void CUi_GreenCrossActive::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = -100;
}

HRESULT CUi_GreenCrossActive::Add_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUi_GreenCrossActive::Add_Texture(void* pArg)
{
	return S_OK;
}

void CUi_GreenCrossActive::CreateGreenCross(_float fTimeDelta)
{
	Ui_Pos_Size_Rotation CreatePos{};
	m_fCreateTime += fTimeDelta;
	if (m_fCreateTime > 0.05)
	{
		m_fCreateTime = 0;
		CreatePos.m_fX = _float((rand() % 1000) - 500);
		CreatePos.m_fY = _float((rand() % 50) - 250);
		m_pGameInstance->Add_Ui_LifeClone(L"CUi_GreenCross",
			eUiRenderType::Render_Blend, &CreatePos);
	}
}

CUi_GreenCrossActive* CUi_GreenCrossActive::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_GreenCrossActive* pInstance = new CUi_GreenCrossActive(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_GreenCrossActive Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_GreenCrossActive::Free()
{
	__super::Free();
}
