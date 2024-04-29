#include "BossEntryTrigger.h"

#include "GameInstance.h"
#include "Monster_Headers.h"

#include "Level_Boss.h"

CBossEntryTrigger::CBossEntryTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTriggerObject{ pGraphic_Device }
{
}

CBossEntryTrigger::CBossEntryTrigger(const CBossEntryTrigger& rhs)
	: CTriggerObject{ rhs }
{
}

HRESULT CBossEntryTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossEntryTrigger::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}
HRESULT CBossEntryTrigger::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, L"Transform_Default", L"Transform"));
	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));

	return S_OK;
}


void CBossEntryTrigger::OnTriggerEnter(CGameObject* pOther)
{
	//m_pGameInstance->Stop(L"Elevator_BGM");
	m_pBoss_Level->SetState_Entry();

	m_bDestroyed = true;
}

CBossEntryTrigger* CBossEntryTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossEntryTrigger* pInstance = new CBossEntryTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBossEntryTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossEntryTrigger::Clone(void* pArg)
{
	CBossEntryTrigger* pInstance = new CBossEntryTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBossEntryTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossEntryTrigger::Free()
{
	__super::Free();
}
