#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Shop final : public CUi
{
protected:
	CUi_Shop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Shop(const CUi_Shop& rhs);
	virtual ~CUi_Shop() = default;

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
	void Move(_float fTimeDelta);
	void Scaling(_float fTimeDelta);
	void Player_Shopping();
	void Player_Choice();


private:
	_float m_fScaleTime = {0.f};
	_float m_fMoveTime = { 1.f };
	bool m_bEnd = { false };


public:
	static CUi_Shop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END