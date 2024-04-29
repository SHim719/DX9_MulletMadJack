#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "FPS_Camera.h"

BEGIN(Client)

class CSansLevel final : public CLevel
{
private:
	CSansLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CSansLevel() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player();
	HRESULT Ready_Layer_Sans_Boss(const wstring& strLayerTag);
	class CPlayer* m_pPlayer = { nullptr };
	class CFPS_Camera* m_pFPS_Camera = { nullptr };
	class CSans* m_pSans = { nullptr };

private:
	_bool m_bBGMPlaying = { false };

	_float m_fEventDelayTimeAcc = 0.f;
	_float m_fEventDelayTime = 2.f;
public:
	static CSansLevel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END