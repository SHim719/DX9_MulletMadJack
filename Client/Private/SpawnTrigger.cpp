#include "SpawnTrigger.h"

#include "GameInstance.h"
#include "Monster_Headers.h"

vector<SPAWN_DESC> CSpawnTrigger::m_vecSpawnInfos;

CSpawnTrigger::CSpawnTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTriggerObject { pGraphic_Device }
{
}

CSpawnTrigger::CSpawnTrigger(const CSpawnTrigger& rhs)
	: CTriggerObject { rhs }
{
}

HRESULT CSpawnTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpawnTrigger::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;
	
	return S_OK;
}
HRESULT CSpawnTrigger::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, L"Transform_Default", L"Transform"));
	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));
	
	return S_OK;
}


void CSpawnTrigger::OnTriggerEnter(CGameObject* pOther)
{
	if (pOther->Get_Tag() == "Monster" && !static_cast<CPawn*>(pOther)->Is_Flying())
		return;

	_uint iLevelID = m_pGameInstance->Get_CurrentLevelID();
	for (_int i = m_iMinIdx; i <= m_iMaxIdx; ++i)
	{
		CGameObject* pMonster = nullptr;
		switch (m_vecSpawnInfos[i].eType)
		{
		case WHITE_SUIT:
			pMonster = m_pGameInstance->Add_Clone(iLevelID, L"Monster", L"Prototype_White_Suit");
			break;
		case CHAINSAW:
			pMonster = m_pGameInstance->Add_Clone(iLevelID, L"Monster", L"Prototype_Chainsaw");
			break;
		case DRONE:
			pMonster = m_pGameInstance->Add_Clone(iLevelID, L"Monster", L"Prototype_Drone");
			break;

		case WHITE_SUIT_SLOPE:
			pMonster = m_pGameInstance->Add_Clone(iLevelID, L"SlopeMonster", L"Prototype_White_Suit_Slope");
			break;
		}

		if (pMonster)
		{
			auto tr = pMonster->Get_Transform();
			tr->Set_Pos(m_vecSpawnInfos[i].vPosition);
			static_cast<CBoxCollider*>(pMonster->Find_Component(L"Collider"))->Update_BoxCollider(tr->Get_WorldMatrix());
		}
	}
	m_bDestroyed = true;
}

CSpawnTrigger* CSpawnTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpawnTrigger* pInstance = new CSpawnTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSpawnTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawnTrigger::Clone(void* pArg)
{
	CSpawnTrigger* pInstance = new CSpawnTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSpawnTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawnTrigger::Free()
{
	__super::Free();
}
