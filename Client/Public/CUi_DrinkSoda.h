#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_DrinkSoda final : public CUi
{
protected:
	CUi_DrinkSoda(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_DrinkSoda(const CUi_DrinkSoda& rhs);
	virtual ~CUi_DrinkSoda() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void Begin_RenderState();
	virtual HRESULT Render() override;
	void End_RenderState();


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	void Initialize_Set_Alpha();

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Adjust_Alpha(_float fTimeDelta);

private:
	_float m_fAdjustAlphaTime = { 0 };
	_uint m_iAlpha = { 150 };

public:
	static CUi_DrinkSoda* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
