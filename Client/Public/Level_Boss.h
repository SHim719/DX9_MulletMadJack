#pragma once
#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_Boss final : public CLevel
{
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

	void Initialize_SodaMachine();


	class CPlayer* m_pPlayer = { nullptr };
	class CFPS_Camera* m_pFPS_Camera = { nullptr };
};

END