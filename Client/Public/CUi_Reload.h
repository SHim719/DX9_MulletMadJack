#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Reload final : public CUi
{
protected:
	CUi_Reload(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Reload(const CUi_Reload& rhs);
	virtual ~CUi_Reload() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	static CUi_Reload* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
