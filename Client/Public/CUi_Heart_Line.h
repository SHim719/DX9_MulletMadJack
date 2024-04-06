#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Heart_Line : public CUi
{
protected:
	CUi_Heart_Line(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Heart_Line(const CUi_Heart_Line& rhs);
	virtual ~CUi_Heart_Line() = default;

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


public:
	virtual void Enter(bool _Enter) override;


private:
	_float m_fBeatTime = { 0.f };


public:
	static CUi_Heart_Line* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END