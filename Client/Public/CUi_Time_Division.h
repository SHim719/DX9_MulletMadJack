#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Time_Division final : public CUi
{
private:
	CUi_Time_Division(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Time_Division(const CUi_Time_Division& rhs);
	virtual ~CUi_Time_Division() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;

protected:
	void Default_Set_LifeTime();
	void Default_Set_Size();

	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


public:
	void Set_Pos(_float3 Position);


public:
	static CUi_Time_Division* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> nullptr
	virtual void Free() override;
};

END