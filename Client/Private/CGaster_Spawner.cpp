#include "CGaster_Spawner.h"
#include "GameInstance.h"


CGaster_Spawner::CGaster_Spawner()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

void CGaster_Spawner::Spawn(SANSSTATE State, SANSPatternSTATE PatternState)
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

GasterRoopArg CGaster_Spawner::CreateRoop(SansGasterFirePos FirePos, SansGasterPos Pos, _float RoopGap, _uint Floor, _float LifeTime)
{
	GasterRoopArg pArg;
	pArg.FirePos = FirePos;
	pArg.Pos = Pos;
	pArg.RoopGap = RoopGap;
	pArg.Floor = Floor;
	pArg.LifeTime = LifeTime;

	return pArg;
}

void CGaster_Spawner::SansState1(SANSPatternSTATE PatternState)
{
	GasterRoopArg pArg{};
	switch (PatternState)
	{
	case SANSPatternSTATE::READY:
		break;
	case SANSPatternSTATE::FIRST:
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::left, 2.f, 1, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::Middle, 2.f, 1, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::Right, 2.f, 1, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::left, 2.f, 3, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::Middle, 2.f, 3, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Straight, SansGasterPos::Right, 2.f, 3, 3.1f);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		break;
	case SANSPatternSTATE::SECOND:
		pArg = CreateRoop(SansGasterFirePos::BackWard, SansGasterPos::left, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::BackWard, SansGasterPos::Middle, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::BackWard, SansGasterPos::Right, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		break;
	case SANSPatternSTATE::THIRD:
		pArg = CreateRoop(SansGasterFirePos::Left, SansGasterPos::left, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Left, SansGasterPos::Middle, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Left, SansGasterPos::Right, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		break;
	case SANSPatternSTATE::FOURTH:
		pArg = CreateRoop(SansGasterFirePos::Right, SansGasterPos::left, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Right, SansGasterPos::Middle, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Right, SansGasterPos::Right, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		break;
	case SANSPatternSTATE::End:
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState2(SANSPatternSTATE PatternState)
{
	GasterRoopArg pArg{};
	switch (PatternState)
	{
	case SANSPatternSTATE::READY:
		break;
	case SANSPatternSTATE::FIRST:
		pArg = CreateRoop(SansGasterFirePos::Up, SansGasterPos::left, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Up, SansGasterPos::Middle, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Up, SansGasterPos::Right, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		break;
	case SANSPatternSTATE::SECOND:
		pArg = CreateRoop(SansGasterFirePos::Down, SansGasterPos::left, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Down, SansGasterPos::Middle, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
		pArg = CreateRoop(SansGasterFirePos::Down, SansGasterPos::Right, 0.9f, 1, 1);
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterRoop", L"Prototype_CGasterRoop",
			&pArg);
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

void CGaster_Spawner::SansState3(SANSPatternSTATE PatternState)
{
}

void CGaster_Spawner::SansState4(SANSPatternSTATE PatternState)
{
}

CGaster_Spawner* CGaster_Spawner::Create()
{
	CGaster_Spawner* m_pInstance = new CGaster_Spawner();

	return m_pInstance;
}

void CGaster_Spawner::Free()
{
	Safe_Release(m_pGameInstance);
}
