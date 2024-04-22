#pragma once

#include "Client_Defines.h"
#include "Base.h"

#define LIGHT_MAX 9

BEGIN(Client)
class CLight_Manager : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
	void Reset_Light();


	_int Set_Light(const D3DLIGHT9& lightDesc);
	void Remove_Light(_uint iLightIdx);
	HRESULT Set_Material(const D3DMATERIAL9& material);

	HRESULT Enable_Light(_uint iLightIdx) 
	{ 
		return m_pGraphic_Device->LightEnable(iLightIdx, TRUE);
	
	}
	HRESULT Disable_Light(_uint iLightIdx) { return m_pGraphic_Device->LightEnable(iLightIdx, FALSE); }
private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = { nullptr };
	_bool m_bArrUseLight[LIGHT_MAX] = {};
public:
	virtual void Free() override;

};

END

