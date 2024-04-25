#include "StageEndTrigger.h"

#include "GameInstance.h"
#include "PlayerManager.h"


CStageEndTrigger::CStageEndTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTriggerObject{ pGraphic_Device }
{
}

CStageEndTrigger::CStageEndTrigger(const CStageEndTrigger& rhs)
	: CTriggerObject{ rhs }
{
}

HRESULT CStageEndTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStageEndTrigger::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}
HRESULT CStageEndTrigger::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, L"Transform_Default", L"Transform"));
	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));

	return S_OK;
}


void CStageEndTrigger::OnTriggerEnter(CGameObject* pOther)
{
	CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_CUTIN_SHOP);
	m_pGameInstance->Stop(L"Gameplay");
	m_pGameInstance->Stop(L"Gameplay2");

	m_pGameInstance->Play(L"Stage_End", false);
	m_pGameInstance->SetVolume(L"Stage_End", 0.5f);
	m_pGameInstance->Play(L"Congratulations", false);
	m_pGameInstance->SetVolume(L"Congratulations", 0.5f);
	m_pGameInstance->Play(L"Elevator_FX", true);
	m_pGameInstance->SetVolume(L"Elevator_FX", 1.f);

	m_bDestroyed = true;

	CGame_Manager::Get_Instance()->Set_Change_Level(m_eNextLevel);
}

CStageEndTrigger* CStageEndTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStageEndTrigger* pInstance = new CStageEndTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CStageEndTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageEndTrigger::Clone(void* pArg)
{
	CStageEndTrigger* pInstance = new CStageEndTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStageEndTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageEndTrigger::Free()
{
	__super::Free();
}
