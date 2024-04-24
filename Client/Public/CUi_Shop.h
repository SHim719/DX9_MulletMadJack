#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Shop final : public CUi
{
public:
	enum class ShopProgress
	{
		Clear,
		ApproachUp,
		Approach,
		Noise,
		Shopping,
		Select,
		End
	};
protected:
	CUi_Shop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Shop(const CUi_Shop& rhs);
	virtual ~CUi_Shop() = default;

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
	void Initialize_Progress_TextureIndex();
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	void Initialize_UpGradeVec();


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Change_Progress(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Scaling(_float fTimeDelta);
	void Approach(_float fTimeDelta);
	void Move_Down(_float fTimeDelta);
	void Texture_Switching(_float fTimeDelta);
	void Set_UpgradePos();
	void Check_Picking();
	bool Player_Select(_uint iNumber);

private:
	_float m_fScaleTime = { 0.f };
	_float m_fMoveTime = { 1.f };
	_float m_fTextureSwitchingTime = { 0 };
	_float m_fUniqueTextureSwitchingTime = { 0 };
	_float m_fChangeProgressTime = { 1.f };
	ShopProgress m_eProgress = { ShopProgress::End };
	_float3 m_OriginScale = { 2900, 1200, 1 };
	_float3 m_OriginPos = { 0, -139, 0 };

	_bool m_bCheck_UpgradeHover = { false };

private:
	vector<class CUi_Shop_UpGrade*> m_UpgradeVec;
	vector<RECT> m_PickScale;



public:
	static CUi_Shop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END