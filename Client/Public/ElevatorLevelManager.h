#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

#define SPAWNPOS_COUNT 12

BEGIN(Client)
class CElevatorLevelManager : public CBase
{
	DECLARE_SINGLETON(CElevatorLevelManager)

public:
	enum LEVEL_STATE
	{
		CutScene,
		OnGoing,
		EndState,
	};

private:
	CElevatorLevelManager();
	virtual ~CElevatorLevelManager() = default;

public:
	void Initialize();
	void Tick(_float fDeltaTime);
	

private:
	CGameInstance* m_pGameInstance = { nullptr };

	LEVEL_STATE m_eState =	OnGoing;
	_float3 m_vSpawnPos[SPAWNPOS_COUNT] = {};
	_float m_fSpawnTime = 3.f;
	_float m_fSpawnTimeAcc = 0.f;

private:
	void Cut_Scene(_float fDeltaTime);
	void On_Going(_float fDeltaTime);
	void End_State(_float fDeltaTime);

	void Spawn_Monsters();
public:
	virtual void Free() override;
};

END

