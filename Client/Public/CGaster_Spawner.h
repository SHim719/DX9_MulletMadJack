#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CSans.h"

BEGIN(Client)

class CGaster_Spawner : public CBase
{
private:
	CGaster_Spawner();
	virtual ~CGaster_Spawner() = default;


public:
	void Spawn(SANSSTATE State, SANSPatternSTATE PatternState);
	void FirstFloorLeft(_float Speed = 1);
	void FirstFloorRight(_float Speed = 1);
	void FirstFloorAll(_float Speed = 1);


private:
	void SansState1(SANSPatternSTATE PatternState);
	void SansState2(SANSPatternSTATE PatternState);
	void SansState3(SANSPatternSTATE PatternState);
	void SansState4(SANSPatternSTATE PatternState);


private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CGaster_Spawner* Create();
	virtual void Free() override;
};

END