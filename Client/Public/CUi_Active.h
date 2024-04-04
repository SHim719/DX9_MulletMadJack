#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Active abstract : public CUi
{
protected:
	CUi_Active(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Active(const CUi_Active& rhs);
	virtual ~CUi_Active() = default;

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


	virtual void Initialize_Set_ActiveTime() PURE;
	virtual void Initialize_Set_Size() PURE;


protected:
	virtual HRESULT Add_Components(void* pArg) PURE;
	virtual HRESULT Add_Texture(void* pArg) PURE;


public:
	bool Get_Active() const { return m_bActive; }


protected:
	bool m_bActive = { false };
	_float m_fActiveTime = { 0.f };
	

public:
	virtual void Free() override;

};

END