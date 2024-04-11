#pragma once
#include "Client_Defines.h"
#include "CUi_Floor.h"

BEGIN(Client)

class CUi_Floor_Part final : public CUi_Floor
{
protected:
	CUi_Floor_Part(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Floor_Part(const CUi_Floor_Part& rhs);
	virtual ~CUi_Floor_Part() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual void Default_Set_LifeTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;	
	virtual void Initialize_Set_Speed() override;
	void Initialize_SetNumber_LifeTime();

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;
	HRESULT Set_NumberTexture(_uint LevelId);


private:
	void Sub_Speed(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Rotation(_float fTimeDelta);
	void Finale(_float fTimeDelta);


private:
	_float m_fSpeed = { 0.f };
	_float m_fAngle = { 0.f };
	_float m_fRotationTime = { 0.f };
	bool m_bReverse = { false };
	_uint m_iPart = {0};
	_float m_fMoveTime = { 0.5 };


public:
	static CUi_Floor_Part* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg) override;
	virtual void Free() override;
};

END