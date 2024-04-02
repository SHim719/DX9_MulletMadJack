#pragma once
#include "Client_Defines.h"
#include "CUi_Life.h"


BEGIN(Client)

class CUi_Background final : public CUi_Life
{
private:
	CUi_Background(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Background(const CUi_Background& rhs);
	virtual ~CUi_Background() = default;


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
	virtual void Default_Set_LifeTime() override;
	virtual void Default_Set_Size() override;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


public:
	void Set_Pos(_float3 Position);
	void Set_Rotation(_float3 Rotation);


private:
	void Copy(void* pArg);


public:
	static CUi_Background* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi_Life* Clone(void* pArg);
	virtual void Free() override;
};

END
