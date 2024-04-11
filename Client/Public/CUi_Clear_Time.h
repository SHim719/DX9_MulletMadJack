#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Clear_Time final : public CUi
{
protected:
	CUi_Clear_Time(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Clear_Time(const CUi_Clear_Time& rhs);
	virtual ~CUi_Clear_Time() = default;

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


public:
	virtual void Enter(bool _Enter) override;


private:
	void Scaling(_float fTimeDelta);


private:
	_float m_fScaleTime = { 0 };
	_float m_fCallClearTime = {0.5};
	bool m_bCallClearTime = { false };


public:
	static CUi_Clear_Time* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END