#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	CLevel_Lobby(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_Lobby* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END