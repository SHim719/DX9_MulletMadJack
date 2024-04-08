#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Chat final : public CUi
{
protected:
	CUi_Chat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Chat(const CUi_Chat& rhs);
	virtual ~CUi_Chat() = default;

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
	void Move(_float fTimeDelta);
	void Texture_Switching();


private:
	_float m_fTextureTime = { 0.f };



public:
	static CUi_Chat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END