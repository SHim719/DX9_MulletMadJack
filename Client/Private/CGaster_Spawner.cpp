#include "CGaster_Spawner.h"
#include "GameInstance.h"


CGaster_Spawner::CGaster_Spawner()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	m_AdjustRandFloor.reserve(8);
	m_AdjustRandPos.reserve(8);
}

void CGaster_Spawner::Spawn(_uint PatternCount, _uint DetailPatternCount)
{
	switch (PatternCount)
	{
	case 0:
		break;
	case 1:
		SansState1(DetailPatternCount);
		break;
	case 2:
		SansState2(DetailPatternCount);
		break;
	case 3:
		SansState3(DetailPatternCount);
		break;
	case 4:
		SansState4(DetailPatternCount);
		break;
	case 5:
		SansState5(DetailPatternCount);
		break;
	case 6:
		SansState6(DetailPatternCount);
		break;
	case 7:
		DefaultState(DetailPatternCount);
		break;
	case 8:
		DefaultState(DetailPatternCount);
		break;
	case 9:
		DefaultState(DetailPatternCount);
		break;
	case 10:
		DefaultState(DetailPatternCount);
		break;
	case 11:
		DefaultState(DetailPatternCount);
		break;
	case 12:
		DefaultState(DetailPatternCount);
		break;
	case 13:
		DefaultState(DetailPatternCount);
		break;
	case 14:
		break;
	case 15:
		DefaultState(DetailPatternCount);
		break;
	case 16:
		DefaultState(DetailPatternCount);
		break;
	case 17:
		SansState17(DetailPatternCount);
		break;
	case 18:
		SansState18(DetailPatternCount);
		break;
	case 19:
		DefaultState(DetailPatternCount);
		break;
	case 20:
		DefaultState(DetailPatternCount);
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

void CGaster_Spawner::Add_Laser(SansGasterFirePos FirePos, SansGasterPos Pos, _float RoopGap, _uint Floor, _float LifeTime)
{
	GasterRoopArg pArg{};
	pArg = CreateRoop(FirePos, Pos, RoopGap, Floor, LifeTime);
	m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Gaster", L"Prototype_CGasterRoop",
		&pArg);
}

void CGaster_Spawner::Add_Bundle(SansGasterFirePos FirePos, _uint Count)
{
	m_AdjustRandPos.clear();
	m_AdjustRandFloor.clear();
	GasterRoopArg pArg{};

	for (_uint i = 0; i < Count; ++i)
	{
		_uint Pos, Floor;
		Pos = rand() % 3;
		Floor = rand() % 3;
		Adjust_Bundle(Pos, Floor);
		switch (Pos)
		{
		case 0:
			pArg.Pos = SansGasterPos::left;
			break;
		case 1:
			pArg.Pos = SansGasterPos::Middle;
			break;
		case 2:
			pArg.Pos = SansGasterPos::Right;
			break;
		default:
			break;
		}
		m_AdjustRandFloor.emplace_back(Floor);
		m_AdjustRandPos.emplace_back(Pos);
		pArg = CreateRoop(FirePos, pArg.Pos, 0, Floor, 0);
		m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Gaster", L"Prototype_CGasterRoop",
			&pArg);
	}
}

void CGaster_Spawner::Adjust_Bundle(_uint& Pos, _uint& Floor)
{
	for (auto iter : m_AdjustRandFloor)
	{
		if (Floor == iter)
		{
			for (auto iter : m_AdjustRandPos)
			{
				if (Pos == iter)
				{
					++Pos;
					if (Pos >= 3)
					{
						Pos = rand()%2;
					}
				}
			}
		}
	}
}

void CGaster_Spawner::SansState1(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 5:
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 1, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 1, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 1, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 2, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 2, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 3, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 3, 0.2f);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 3, 0.2f);
		break;
	case 6:
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Middle, 0, 1, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Right, 0, 1, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::left, 0, 2, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Middle, 0, 2, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Right, 0, 2, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::left, 0, 3, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Middle, 0, 3, 0.2f);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Right, 0, 3, 0.2f);
		break;
	case 7:
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 2, 0);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState2(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		Add_Bundle(SansGasterFirePos::Straight, 3);
		break;
	case 2:
		Add_Bundle(SansGasterFirePos::Right , 2);
		break;
	case 3:
		Add_Bundle(SansGasterFirePos::Up, 5);
		break;
	case 4:
		Add_Bundle(SansGasterFirePos::Straight, 3);
		break;
	case 5:
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState3(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		Add_Bundle(SansGasterFirePos::Straight, 3);
		break;
	case 3:
		Add_Bundle(SansGasterFirePos::Left, 1);
		Add_Bundle(SansGasterFirePos::Right, 1);
		break;
	case 4:
		Add_Bundle(SansGasterFirePos::Straight, 2);
		break;
	case 5:
		Add_Bundle(SansGasterFirePos::Right, 3);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState4(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		Add_Bundle(SansGasterFirePos::Straight, 2);
		Add_Bundle(SansGasterFirePos::Down, 1);
		break;
	case 3:
		Add_Bundle(SansGasterFirePos::Left, 2);
		Add_Bundle(SansGasterFirePos::Right, 1);
		break;
	case 4:
		Add_Bundle(SansGasterFirePos::Straight, 2);
		Add_Bundle(SansGasterFirePos::Up, 1);
		break;
	case 5:
		Add_Bundle(SansGasterFirePos::BackWard, 2);
		Add_Bundle(SansGasterFirePos::Up, 1);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState5(_uint DetailPatternCount)
{	
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 3, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 3, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 3, 0);
		break;
	case 2:
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::left, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Middle, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Left, SansGasterPos::Right, 0, 1, 0);
		break;
	case 3:
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::left, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::Middle, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::Right, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::left, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::Middle, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::Right, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::left, 0, 3, 0);
		Add_Laser(SansGasterFirePos::Up, SansGasterPos::Right, 0, 3, 0);
		break;
	case 4:
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 1, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 2, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::left, 0, 3, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Middle, 0, 3, 0);
		Add_Laser(SansGasterFirePos::Straight, SansGasterPos::Right, 0, 3, 0);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState6(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		Add_Bundle(SansGasterFirePos::BackWard, 3);
		break;
	case 2:
		Add_Bundle(SansGasterFirePos::Straight, 5);
		break;
	case 3:
		Add_Bundle(SansGasterFirePos::Up, 3);
		break;
	case 4:
		Add_Bundle(SansGasterFirePos::Left, 1);
		Add_Bundle(SansGasterFirePos::Right, 1);
		Add_Bundle(SansGasterFirePos::Up, 1);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::DefaultState(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	_uint RandomFirePosint = rand() % 4;
	SansGasterFirePos RandomFirePos;
	switch (RandomFirePosint)
	{
	case 0:
		RandomFirePos = SansGasterFirePos::Left;
		break;
	case 1:
		RandomFirePos = SansGasterFirePos::Right;
		break;
	case 2:
		RandomFirePos = SansGasterFirePos::Straight;
		break;
	case 3:
		RandomFirePos = SansGasterFirePos::Up;
		break;
	default:
		RandomFirePos = SansGasterFirePos::End;
		break;
	}
	_uint Count = (rand() % 3) + 1;
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		Add_Bundle(RandomFirePos, Count);
		break;
	case 2:
		Add_Bundle(RandomFirePos, Count);
		break;
	case 3:
		Add_Bundle(RandomFirePos, Count);
		break;
	case 4:
		Add_Bundle(RandomFirePos, Count);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState17(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:

		break;
	case 2:
		
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		Add_Bundle(SansGasterFirePos::Straight, 7);
		break;
	case 8:
		Add_Bundle(SansGasterFirePos::Up, 5);
		break;
	case 9:
		Add_Bundle(SansGasterFirePos::BackWard, 2);
		break;
	case 10:
		Add_Bundle(SansGasterFirePos::Right, 5);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::SansState18(_uint DetailPatternCount)
{
	GasterRoopArg pArg{};
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:

		break;
	case 2:

		break;
	case 3:
		Add_Bundle(SansGasterFirePos::Straight, 4);
		break;
	case 4:
		Add_Bundle(SansGasterFirePos::Up, 7);
		break;
	case 5:
		Add_Bundle(SansGasterFirePos::Left, 3);
		break;
	case 6:
		Add_Bundle(SansGasterFirePos::Right, 5);
		break;
	default:
		break;
	}
}

void CGaster_Spawner::RandomDominateFloor(SansGasterFirePos FirePos, _uint DetailPatternCount)
{
	_uint RandomFloor = (rand() % 3) + 1;
	GasterRoopArg pArg{};
	
	pArg = CreateRoop(FirePos, SansGasterPos::left, 0, RandomFloor, 0);
	m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Gaster", L"Prototype_CGasterRoop",
		&pArg);
	pArg = CreateRoop(FirePos, SansGasterPos::Middle, 0, RandomFloor, 0);
	m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Gaster", L"Prototype_CGasterRoop",
		&pArg);
	pArg = CreateRoop(FirePos, SansGasterPos::Right, 0, RandomFloor, 0);
	m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Gaster", L"Prototype_CGasterRoop",
		&pArg);
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
