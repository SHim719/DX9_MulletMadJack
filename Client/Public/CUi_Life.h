#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Life abstract : public CUi
{
protected:
	CUi_Life(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Life(const CUi_Life& rhs);
	virtual ~CUi_Life() = default;


public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void PriorityTick(_float fTimeDelta) PURE;
	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) PURE;
	virtual void Initialize_Set_Speed() PURE;


	virtual void Default_Set_LifeTime() PURE;
	virtual void Default_Set_Size() PURE;


protected:
	virtual HRESULT Add_Components(void* pArg) PURE;
	virtual HRESULT Add_Texture(void* pArg) PURE;


protected:
	void Set_Lifetime(_float Lifetime) { m_fLifeTime = Lifetime; }
	void Cal_Life_Blink(_float fTimeDelta);
	bool Cal_BlinkRender(_float BlinkGap);


protected:
	_float m_fLifeTime = { 0.f };
	_float m_iBlink = { 0.f };
	bool m_bBlink = { false };

public:
	virtual CUi_Life* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END
