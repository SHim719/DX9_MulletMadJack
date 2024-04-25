#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CSans.h"
#include "CGasterRoop.h"


BEGIN(Client)

class CGaster_Spawner : public CBase
{
private:
	CGaster_Spawner();
	virtual ~CGaster_Spawner() = default;


public:
	void Spawn(_uint PatternCount, _uint DetailPatternCount);
	GasterRoopArg CreateRoop(SansGasterFirePos FirePos, SansGasterPos Pos, _float RoopGap, _uint Floor, _float LifeTime);
	void Add_Laser(SansGasterFirePos FirePos, SansGasterPos Pos, _float RoopGap, _uint Floor, _float LifeTime);
	void Add_Bundle(SansGasterFirePos FirePos, _uint Count);
	void Adjust_Bundle(_uint& Pos, _uint& Floor);

private:
	void SansState1(_uint DetailPatternCount);
	void SansState2(_uint DetailPatternCount);
	void SansState3(_uint DetailPatternCount);
	void SansState4(_uint DetailPatternCount);
	void SansState5(_uint DetailPatternCount);
	void SansState6(_uint DetailPatternCount);
	void DefaultState(_uint DetailPatternCount);
	void SansState17(_uint DetailPatternCount);
	void SansState18(_uint DetailPatternCount);
	void RandomDominateFloor(SansGasterFirePos FirePos, _uint DetailPatternCount);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	vector<_uint> m_AdjustRandFloor;
	vector<_uint> m_AdjustRandPos;

public:
	static CGaster_Spawner* Create();
	virtual void Free() override;
};

END