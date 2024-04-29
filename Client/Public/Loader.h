#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	_uint Loading();
	_bool isFinished() const {
		return m_isFinished;
	}

	void Show_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

	_float Get_ProgressPercent() const { return m_fProgress; }
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

	HANDLE				m_hThread = {};
	CRITICAL_SECTION	m_CriticalSection = {};
	LEVEL				m_eNextLevelID = { LEVEL_END };

	_tchar				m_szLoadingText[MAX_PATH] = TEXT("");
	_float				m_fLoadingPercent = { 0.0f };
	_bool				m_isFinished = { false };

private:
	CGameInstance*		m_pGameInstance = { nullptr };
	_float				m_fProgress = { 0 };
private:
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();
	HRESULT Loading_For_Boss_Level();
	HRESULT Loading_For_Lobby_Level();
	HRESULT Loading_For_Sans_Level();

	HRESULT Loading_For_WhiteSuitMonster();
	HRESULT Loading_For_Chainsaw_Monster();
	HRESULT Loading_For_Drone_Monster();
	HRESULT Loading_For_Map_Texture();
	HRESULT Loading_For_Effect_Texture();
	HRESULT Loading_For_Sans_Texture();
	HRESULT Loading_For_Boss_Texture();

	HRESULT Ready_MapObject_Prototype();
	HRESULT Ready_Monster_Prototype();
	HRESULT	Ready_Effect_Prototype();

	HRESULT Loading_For_Ui();
	HRESULT Ready_ClearUi_Texture();
	HRESULT Ready_ShopUi_Texture();
	HRESULT Ready_StartUi_Texture();
	HRESULT Ready_OnGoingUi_Texture();
	HRESULT Ready_TextUi_Texture();
	HRESULT Ready_PlayerUi_Texture();
	HRESULT Ready_PlayerGunUi_Texture();
	HRESULT Ready_Camera_Effect_Texture();

	HRESULT Ready_Prototype_Ui_Life();
	HRESULT Ready_Active_UiClear();
	HRESULT Ready_Active_UiShop();
	HRESULT Ready_Active_UiOnGoing();
	HRESULT Ready_Active_Gun();
	HRESULT Ready_Prototype_Effect();
	HRESULT Ready_Active_Camera_Effect();

	HRESULT Ready_BGM();
	HRESULT Ready_Player_Weapon_Sound();
	HRESULT Ready_Player_Sound();
	HRESULT Ready_Monster_Sound();
	HRESULT Ready_Effect_Sound();
	HRESULT Ready_Announcer_Sound();
	HRESULT Ready_Streamer_Sound();
	HRESULT Ready_Beholder_Sound();
	HRESULT Ready_Sans_Sound();

	HRESULT Ready_SkyBox();

	void Initialize_TextManager();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END