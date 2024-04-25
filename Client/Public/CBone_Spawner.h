#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CSans.h"
#include "CBoneRoop.h"


BEGIN(Client)

class CBone_Spawner : public CBase
{
private:
	CBone_Spawner();
	virtual ~CBone_Spawner() = default;


public:
	void Spawn(_uint PatternCount, _uint DetailPatternCount);
	BoneRoopArg CreateRoop(SansBonePos Pos, SansBoneDir Dir, _float RoopGap, _uint Floor, _float Speed, _float LifeTime);
	void Add_Bone(SansBonePos Pos, SansBoneDir Dir, _float RoopGap, _uint Floor, _float Speed, _float LifeTime);


private:
	void SansState1(_uint DetailPatternCount);
	void SansState2(_uint DetailPatternCount);
	void SansState3(_uint DetailPatternCount);
	void SansState4(_uint DetailPatternCount);
	void SansState5(_uint DetailPatternCount);
	void SansState18(_uint DetailPatternCount);
	void RandomDominateState(_uint DetailPatternCount);
	void RandomRepeatState(_uint DetailPatternCount);
	void PatternDominateFloor(_uint Floor);
	void PatternDominatePos(SansBonePos Pos1, SansBonePos Pos2);
	void PatternRepeat(SansBonePos Pos, _float RoopGap, _uint Floor);


private:
	_float m_fRoopTime = { 0 };
	_float m_fLifeTime = { 0 };
	_bool m_bRoop = { false };


private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CBone_Spawner* Create();
	virtual void Free() override;
};

END