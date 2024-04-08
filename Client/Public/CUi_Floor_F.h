#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Floor_F final : public CUi
{
protected:
	CUi_Floor_F(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Floor_F(const CUi_Floor_F& rhs);
	virtual ~CUi_Floor_F() = default;

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
	void Sub_Speed(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Rotation(_float fTimeDelta);


private:
	_float m_fSpeed = { 0.f };
	_float m_fRotationTime = { 0.f };
	_float m_fAngle = { 0.f };
	bool m_bTemp = true;


public:
	static CUi_Floor_F* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END