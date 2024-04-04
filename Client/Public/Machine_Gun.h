#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CMachine_Gun final : public CUi
{
protected:
	CMachine_Gun(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMachine_Gun(const CMachine_Gun& rhs);
	virtual ~CMachine_Gun() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Active();
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	 
protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg);
	virtual void Initialize_Set_Speed();


	virtual void Default_Set_LifeTime();
	virtual void Default_Set_Size();


protected:
	virtual HRESULT Add_Components(void* pArg);
	virtual HRESULT Add_Texture(void* pArg);

public:
	static CUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

public:
	virtual void Free() override;
};

END