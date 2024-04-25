#include "ElevatorLevelManager.h"
#include "Monster_Headers.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CElevatorLevelManager)

CElevatorLevelManager::CElevatorLevelManager()
{
}

void CElevatorLevelManager::Initialize()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);

	m_vSpawnPos[0] = { -4.f, 3.5f, 4.f };
	m_vSpawnPos[1] = { 0.f,  3.5f, 4.f };
	m_vSpawnPos[2] = { 4.f,  3.5f, 4.f };
	m_vSpawnPos[3] = {  4.f, 3.5f, 4.f };
	m_vSpawnPos[4] = {  4.f, 3.5f, 0.f };
	m_vSpawnPos[5] = {  4.f, 3.5f, -4.f };
	m_vSpawnPos[6] = {  4.f, 3.5f, -4.f };
	m_vSpawnPos[7] = {  0.f, 3.5f, -4.f };
	m_vSpawnPos[8] = { -4.f, 3.5f, -4.f };
	m_vSpawnPos[9] = { -4.f, 3.5f, -4.f };
	m_vSpawnPos[10] = { -4.f, 3.5f,  0.f };
	m_vSpawnPos[11] = { -4.f, 3.5f, 4.f };

	m_eState = OnGoing;
}

void CElevatorLevelManager::Tick(_float fDeltaTime)
{
	switch (m_eState)
	{
	case LEVEL_STATE::CutScene:
		Cut_Scene(fDeltaTime);
		break;
	case LEVEL_STATE::OnGoing:
		On_Going(fDeltaTime);
		break;
	case LEVEL_STATE::EndState:
		End_State(fDeltaTime);
		break;
	}

}

void CElevatorLevelManager::Cut_Scene(_float fDeltaTime)
{
}

void CElevatorLevelManager::On_Going(_float fDeltaTime)
{
	m_fSpawnTimeAcc += fDeltaTime;
	
	if (m_fSpawnTimeAcc >= m_fSpawnTime)
	{
		for (_uint i = 0; i < 3; ++i)
		{
			Spawn_Monsters();
		}
		m_fSpawnTimeAcc = 0.f;
	}
	
}

void CElevatorLevelManager::End_State(_float fDeltaTime)
{
	
	
}

void CElevatorLevelManager::Spawn_Monsters()
{
	_int iRandMonsterIndex = rand() % MONSTERTYPE::WHITE_SUIT_SLOPE;
	_int iRandSpawnPosIndex = rand() % SPAWNPOS_COUNT;

	wstring strPrototypeTag = L"";
	switch ((MONSTERTYPE)iRandMonsterIndex)
	{
	case WHITE_SUIT:
		strPrototypeTag = L"Prototype_White_Suit";
		break;
	case CHAINSAW:
		strPrototypeTag = L"Prototype_Chainsaw";
		break;
	case DRONE:
		strPrototypeTag = L"Prototype_Drone";
		break;
	default:
		break;
	}

	CPawn* pPawn = static_cast<CPawn*>(m_pGameInstance->Add_Clone(LEVEL_ELEVATOR, L"Monster", strPrototypeTag));
	pPawn->SetState_Air();
	pPawn->Get_Transform()->Set_Pos(m_vSpawnPos[iRandSpawnPosIndex]);
	static_cast<CBoxCollider*>(pPawn->Find_Component(L"Collider"))->Update_BoxCollider(pPawn->Get_Transform()->Get_WorldMatrix());


}

void CElevatorLevelManager::Free()
{

	Safe_Release(m_pGameInstance);
	Destroy_Instance();
}
