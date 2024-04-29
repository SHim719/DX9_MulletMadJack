#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Boss final : public CLevel
{
public:
	enum Level_State
	{
		NoEntry,
		Entry,
		Battle,
		End,
	};

private:
	CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Boss() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player();
	HRESULT Ready_Layer_Beholder(const wstring& strLayerTag);

public:
	void SetState_Entry();

private:
	Level_State m_eState = NoEntry;

	void Entry_State(_float fTimeDelta);

private:
	_float3 m_vEntryPos{};

	_bool m_bAnotherBranch = false;
	_bool m_bBeholderAttack = false;
	_float m_fTimeAcc = 0.f;
	_float m_fEventDelay = 3.f;
private:
	class CPlayer*		m_pPlayer = { nullptr };
	class CFPS_Camera*	m_pFPS_Camera = { nullptr };
	class CBeholder*	m_pBeholder = { nullptr };

	
};

END