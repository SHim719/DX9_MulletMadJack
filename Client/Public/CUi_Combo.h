#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Combo final : public CUi
{
public:
	using ComboDesc = struct ComboDescInfo
	{
		_uint iKillCount = { 0 };
		bool bFirstCall = { false };
	};
private:
	CUi_Combo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Combo(const CUi_Combo& rhs);
	virtual ~CUi_Combo() = default;


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
	void Default_Set_Pos();

	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	void Initialize_SetComboDesc(void* pArg);

private:
	void Scaling(_float fTimeDelta);


private:
	bool m_bFirstCall = { true };
	_float3 m_OriginComboScale = { 150, 50, 1 };
	_float m_fScalingTime = { 0 };
	class CUi_Background* m_pBackGround = { nullptr };
	class CUi_SpecialHit_Part* m_pSpecialHit_Part = { nullptr };
public:
	static CUi_Combo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg) override; // pArg -> ComboDesc
	virtual void Free() override;
};

END