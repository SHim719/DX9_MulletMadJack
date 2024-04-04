#pragma once
#include "Client_Defines.h"
#include "CUi_Active.h"

BEGIN(Client)

class CUi_PEACE final : public CUi_Active
{
protected:
	CUi_PEACE(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_PEACE(const CUi_PEACE& rhs);
	virtual ~CUi_PEACE() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override; 
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	static CUi_PEACE* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END