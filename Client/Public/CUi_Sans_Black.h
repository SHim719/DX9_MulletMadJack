#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Sans_Black final : public CUi
{
protected:
	CUi_Sans_Black(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Sans_Black(const CUi_Sans_Black& rhs);
	virtual ~CUi_Sans_Black() = default;

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
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	static CUi_Sans_Black* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
