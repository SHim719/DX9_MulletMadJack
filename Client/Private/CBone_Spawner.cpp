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

BoneRoopArg CBone_Spawner::CreateRoop(SansBonePos Pos, SansBoneDir Dir, _float RoopGap, _uint Floor, _float Speed,_float LifeTime)
{
	BoneRoopArg pArg;

	pArg.Pos = Pos;
	pArg.Dir = Dir;
	pArg.RoopGap = RoopGap;
	pArg.Floor = Floor;
	pArg.Speed = Speed;
	pArg.LifeTime = LifeTime;

	return pArg;
}

void CBone_Spawner::SansState1(SANSPatternSTATE PatternState)
{
	BoneRoopArg pArg{};
	switch (PatternState)
	{
	case SANSPatternSTATE::READY:
		break;
	case SANSPatternSTATE::FIRST:
		pArg = CreateRoop(SansBonePos::Left, SansBoneDir::Straight, 0.5f, 1, 1.f, 3);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"BoneRoop", L"Prototype_CBoneRoop",
			&pArg);
		break;
	case SANSPatternSTATE::SECOND:
		
		break;
	case SANSPatternSTATE::THIRD:
		
		break;
	case SANSPatternSTATE::FOURTH:
		
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
		
		break;
	case SANSPatternSTATE::SECOND:
		
		break;
	case SANSPatternSTATE::THIRD:
		
		break;
	case SANSPatternSTATE::FOURTH:
		
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
