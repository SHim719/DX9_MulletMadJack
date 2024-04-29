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
		Sans,
		Return,
		OnGoing,
		EndState,
		LS_END,
	};

private:
	CElevatorLevelManager();
	virtual ~CElevatorLevelManager() = default;

public:
	void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
	void Tick(_float fDeltaTime);
	

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

	LEVEL_STATE m_eState = Sans;
	_float3 m_vSpawnPos[SPAWNPOS_COUNT] = {};
	_float m_fSpawnTime = 3.f;
	_float m_fSpawnTimeAcc = 0.f;

	_float m_fEventDelayTime = 5.f;
	_float m_fEventDelayTimeAcc = 0.f;
	_bool m_bAnotherBranch = false;

	_bool m_bUltimate = false;

	class CUI_FadeInOut* m_pFadeInOutUI = { nullptr };
private:
	void State_Sans(_float fDeltaTime);
	void State_Return(_float fDeltaTime);
	void Warning();
	void State_On_Going(_float fDeltaTime);
	void State_End_State(_float fDeltaTime);

	void Spawn_Monsters();
public:
	virtual void Free() override;
};

END

