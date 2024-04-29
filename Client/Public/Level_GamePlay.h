#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "FPS_Camera.h"
#include "Player.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	void Initialize_SodaMachine();

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player();
	void Ready_First_Monster();

	class CPlayer* m_pPlayer = { nullptr };
	class CFPS_Camera* m_pFPS_Camera = { nullptr };
	class CWhite_Suit_Monster* m_pFirstMonster = { nullptr };

	_float m_fStartTriggerDelay = 1.5f;
	_float m_fTimeAcc = 0.f;
	_bool  m_bFirstTrigger = false;
};

END