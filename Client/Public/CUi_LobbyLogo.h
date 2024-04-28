#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_LobbyLogo final : public CUi
{
protected:
	CUi_LobbyLogo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_LobbyLogo(const CUi_LobbyLogo& rhs);
	virtual ~CUi_LobbyLogo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void TextureSwitching(_float fTimeDelta);


private:
	_float m_fSwitchingGap = { 0 };
	_float m_fSwitchingTime = { 0 };


public:
	static CUi_LobbyLogo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
