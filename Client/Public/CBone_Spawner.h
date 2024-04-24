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
	void Spawn(SANSSTATE State, SANSPatternSTATE PatternState);
	BoneRoopArg CreateRoop(SansBonePos Pos, SansBoneDir Dir, _float RoopGap, _uint Floor, _float Speed, _float LifeTime);


private:
	void SansState1(SANSPatternSTATE PatternState);
	void SansState2(SANSPatternSTATE PatternState);
	void SansState3(SANSPatternSTATE PatternState);
	void SansState4(SANSPatternSTATE PatternState);


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