#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_UpGrade_Focus final : public CUi
{
private:
	CUi_UpGrade_Focus(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_UpGrade_Focus(const CUi_UpGrade_Focus& rhs);
	virtual ~CUi_UpGrade_Focus() = default;


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


private:
	_float m_fSwitchingTime = { 0.f };

public:
	static CUi_UpGrade_Focus* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> _float3
	virtual void Free() override;
};

END
