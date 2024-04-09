#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Shop_Noise_Part final : public CUi
{
private:
	CUi_Shop_Noise_Part(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Shop_Noise_Part(const CUi_Shop_Noise_Part& rhs);
	virtual ~CUi_Shop_Noise_Part() = default;


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
	void Set_Scale(_float3 Scale);


public:
	static CUi_Shop_Noise_Part* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END
