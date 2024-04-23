#include "CBone_Spawner.h"
#include "GameInstance.h"
#include "CBoneRoop.h"

CBone_Spawner::CBone_Spawner()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

void CBone_Spawner::Spawn(SANSSTATE State, SANSPatternSTATE PatternState)
{
    if (PatternState == SANSPatternSTATE::READY)
        return;

	switch (State)
	{
	case SANSSTATE::STATE_IDLE:
		break;
	case SANSSTATE::STATE_PATTERN1:
		SansState1(PatternState);
		break;
	case SANSSTATE::STATE_PATTERN2:
		SansState2(PatternState);
		break;
	case SANSSTATE::STATE_PATTERN3:
		SansState3(PatternState);
		break;
	case SANSSTATE::STATE_PATTERN4:
		SansState4(PatternState);
		break;
	case SANSSTATE::STATE_DEATH:
		break;
	case SANSSTATE::STATE_END:
		break;
	default:
		break;
	}
}

void CBone_Spawner::FirstFloorLeft(_float Speed)
{
	SansBoneArg pArg;
	pArg.floor = 1;
	pArg.fSpeed = Speed;
	pArg.Size = SansBonePos::Left;
	m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), 
		L"Sans_Bone", L"Prototype_Sans_Bone", &pArg);
}

void CBone_Spawner::FirstFloorRight(_float Speed)
{
	SansBoneArg pArg;
	pArg.floor = 1;
	pArg.fSpeed = Speed;
	pArg.Size = SansBonePos::Right;
	m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(),
		L"Sans_Bone", L"Prototype_Sans_Bone", &pArg);
}

void CBone_Spawner::FirstFloorAll(_float Speed)
{
	SansBoneArg pArg;
	pArg.floor = 1;
	pArg.fSpeed = Speed;
	pArg.Size = SansBonePos::Left;
	m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(),
		L"Sans_Bone", L"Prototype_Sans_Bone", &pArg);

	pArg.floor = 1;
	pArg.fSpeed = Speed;
	pArg.Size = SansBonePos::Right;
	m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(),
		L"Sans_Bone", L"Prototype_Sans_Bone", &pArg);
}

void CBone_Spawner::SecondFloorLeft(_float Speed)
{
	
}

void CBone_Spawner::SecondFloorRight(_float Speed)
{
}

void CBone_Spawner::SecondFloorAll(_float Speed)
{
}


void CBone_Spawner::SansState1(SANSPatternSTATE PatternState)
{
	switch (PatternState)
	{
	case SANSPatternSTATE::READY:
		break;
	case SANSPatternSTATE::FIRST:
		FirstFloorLeft();
		break;
	case SANSPatternSTATE::SECOND:
		FirstFloorRight();
		break;
	case SANSPatternSTATE::THIRD:
		FirstFloorAll();
		break;
	case SANSPatternSTATE::FOURTH:
		FirstFloorAll();
		break;
	case SANSPatternSTATE::End:
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState2(SANSPatternSTATE PatternState)
{
	switch (PatternState)
	{
	case SANSPatternSTATE::READY:
		break;
	case SANSPatternSTATE::FIRST:
		FirstFloorAll();
		break;
	case SANSPatternSTATE::SECOND:
		FirstFloorAll();
		break;
	case SANSPatternSTATE::THIRD:
		FirstFloorLeft();
		break;
	case SANSPatternSTATE::FOURTH:
		FirstFloorAll();
		break;
	case SANSPatternSTATE::End:
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState3(SANSPatternSTATE PatternState)
{
}

void CBone_Spawner::SansState4(SANSPatternSTATE PatternState)
{
}

CBone_Spawner* CBone_Spawner::Create()
{
	CBone_Spawner* m_pInstance = new CBone_Spawner();

    return m_pInstance;
}

void CBone_Spawner::Free()
{
	Safe_Release(m_pGameInstance);
}
