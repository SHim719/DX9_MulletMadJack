#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CSans.h"

BEGIN(Client)

class CBone_Spawner : public CBase
{
private:
	CBone_Spawner();
	virtual ~CBone_Spawner() = default;


public:
	void Spawn(SANSSTATE State, SANSPatternSTATE PatternState);
	void FirstFloorLeft(_float Speed = 500);
	void FirstFloorRight(_float Speed = 500);
	void FirstFloorAll(_float Speed = 500);
	void SecondFloorLeft(_float Speed = 500);
	void SecondFloorRight(_float Speed = 500);
	void SecondFloorAll(_float Speed = 500);


private:
	void SansState1(SANSPatternSTATE PatternState);
	void SansState2(SANSPatternSTATE PatternState);
	void SansState3(SANSPatternSTATE PatternState);
	void SansState4(SANSPatternSTATE PatternState);


public:
	static CBone_Spawner* Create();
	virtual void Free() override;
};

END