#include "CBone_Spawner.h"
#include "GameInstance.h"
#include "CBoneRoop.h"


CBone_Spawner::CBone_Spawner()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

void CBone_Spawner::Spawn(_uint PatternCount, _uint DetailPatternCount)
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
		RandomDominateState(DetailPatternCount);
		break;
	case 6:
		RandomRepeatState(DetailPatternCount);
		break;
	case 7:
		RandomRepeatState(DetailPatternCount);
		RandomRepeatState(DetailPatternCount);
		break;
	case 8:
		RandomRepeatState(DetailPatternCount);
		RandomDominateState(DetailPatternCount);
		break;
	case 9:
		RandomRepeatState(DetailPatternCount);
		break;
	case 10:
		RandomRepeatState(DetailPatternCount);
		break;
	case 11:
		RandomDominateState(DetailPatternCount);
		break;
	case 12:
		RandomRepeatState(DetailPatternCount);
		RandomRepeatState(DetailPatternCount);
		break;
	case 13:
		RandomDominateState(DetailPatternCount);
		RandomRepeatState(DetailPatternCount);
		break;
	case 14:
		break;
	case 15:
		RandomDominateState(DetailPatternCount);
		break;
	case 16:
		RandomRepeatState(DetailPatternCount);
		break;
	case 17:
		break;
	case 18:
		SansState18(DetailPatternCount);
		break;
	case 19:
		RandomRepeatState(DetailPatternCount);
		break;
	case 20:
		RandomRepeatState(DetailPatternCount);
		break;
	case 21:
		RandomRepeatState(DetailPatternCount);
		break;
	case 22:
		RandomRepeatState(DetailPatternCount);
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

void CBone_Spawner::Add_Bone(SansBonePos Pos, SansBoneDir Dir, _float RoopGap, _uint Floor, _float Speed, _float LifeTime)
{
	BoneRoopArg pArg{};
	pArg = CreateRoop(Pos, Dir, RoopGap, Floor, Speed, LifeTime);
	m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Bone", L"Prototype_CBoneRoop",
		&pArg);
}

void CBone_Spawner::SansState1(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 0:
		break;
	case 1:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		break;
	case 2:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		break;
	case 3:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		break;
	case 4:
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		break;
	case 5:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 1, 10.f, 1);
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 2, 10.f, 1);
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState2(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState3(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 1:
		break;
	case 2:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, 3, 10.f, 10);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, 3, 10.f, 10);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, 3, 10.f, 10);
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState4(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 1:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.5f, 1, 3.f, 5);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.5f, 3, 3.f, 5);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.5f, 3, 3.f, 5);
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState5(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 1:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.5f, 1, 3.f, 1);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.5f, 1, 3.f, 1);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.5f, 1, 3.f, 1);
		break;
	case 2:
		Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.5f, 3, 5.f, 5);
		Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.5f, 1, 2.f, 5);
		Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.5f, 1, 2.f, 5);
		break;
	default:
		break;
	}
}

void CBone_Spawner::SansState18(_uint DetailPatternCount)
{
	switch (DetailPatternCount)
	{
	case 3:
		PatternDominatePos(SansBonePos::Left, SansBonePos::Middle);
		break;
	default:
		break;
	}

}

void CBone_Spawner::RandomDominateState(_uint DetailPatternCount)
{
	_uint Floor = rand() % 4;
	switch (DetailPatternCount)
	{
	case 1:
		PatternDominateFloor(Floor);
		break;
	default:
		break;
	}
}

void CBone_Spawner::RandomRepeatState(_uint DetailPatternCount)
{
	_uint Floor = rand() % 4;

	_uint Posint = rand() % 3;
	SansBonePos Pos{};
	switch (Posint)
	{
	case 0:
		Pos = SansBonePos::Left;
		break;
	case 1:
		Pos = SansBonePos::Middle;
		break;
	case 2:
		Pos = SansBonePos::Right;
		break;
	default:
		break;
	}

	_uint RoopGapint= rand() % 10;
	_float RoopGap = RoopGapint / 10.f;

	switch (DetailPatternCount)
	{
	case 1:
		PatternRepeat(Pos, RoopGap, Floor);
		break;
	default:
		break;
	}
}

void CBone_Spawner::PatternDominateFloor(_uint Floor)
{
	if (Floor == 0)
		return;

	Add_Bone(SansBonePos::Left, SansBoneDir::Straight, 0.1f, Floor, 3.f, 10);
	Add_Bone(SansBonePos::Middle, SansBoneDir::Straight, 0.1f, Floor, 3.f, 10);
	Add_Bone(SansBonePos::Right, SansBoneDir::Straight, 0.1f, Floor, 3.f, 10);
}

void CBone_Spawner::PatternDominatePos(SansBonePos Pos1, SansBonePos Pos2)
{
	if (Pos1 != SansBonePos::End)
	{
		Add_Bone(Pos1, SansBoneDir::Straight, 0.1f, 1, 3.f, 10);
		Add_Bone(Pos1, SansBoneDir::Straight, 0.1f, 2, 3.f, 10);
		Add_Bone(Pos1, SansBoneDir::Straight, 0.1f, 3, 3.f, 10);
	}
	if (Pos2 != SansBonePos::End)
	{
		Add_Bone(Pos2, SansBoneDir::Straight, 0.1f, 1, 3.f, 10);
		Add_Bone(Pos2, SansBoneDir::Straight, 0.1f, 2, 3.f, 10);
		Add_Bone(Pos2, SansBoneDir::Straight, 0.1f, 3, 3.f, 10);
	}
}

void CBone_Spawner::PatternRepeat(SansBonePos Pos, _float RoopGap, _uint Floor)
{
	Add_Bone(Pos, SansBoneDir::Straight, RoopGap, Floor, 3.f, 10);
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
