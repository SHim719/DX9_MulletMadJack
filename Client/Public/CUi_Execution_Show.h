#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Execution_Show final : public CUi
{
protected:
	CUi_Execution_Show(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Execution_Show(const CUi_Execution_Show& rhs);
	virtual ~CUi_Execution_Show() = default;

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
	void WeaponMove(_float fTimeDelta);
	void WeaponScaling(_float fTimeDelta);
	void WeaponBlink(_float fTimeDelta);

private:
	_float m_fScalingTime = { 0 };
	_float m_fMoveTime = { 0 };
	_float m_fWeaponBlinkTime = { 0 };
	bool m_bWeaponBlink = { false };
	_float3 m_WeaponOriginScale = { 20, 75, 1 };
	_float3 m_WeaponOriginPos = {0, -150, 0};

private:
	CTexture* m_pWeaponTextureCom = { nullptr };
	CTransform* m_pWeaponTransformCom = { nullptr };
	CVIBuffer_Rect* m_pWeaponVIBufferCom = { nullptr };
	Ui_Pos_Size_Rotation m_Weapon = {};

	CTexture* m_pBackGroundTextureCom = { nullptr };
	CTransform* m_pBackGroundTransformCom = { nullptr };
	CVIBuffer_Rect* m_pBackGroundVIBufferCom = { nullptr };
	Ui_Pos_Size_Rotation m_WeaponBackGround = {};

	


public:
	static CUi_Execution_Show* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
