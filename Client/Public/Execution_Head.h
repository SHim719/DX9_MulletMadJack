#pragma once
#include "Client_Defines.h"
#include "CUi.h"
#include "CGame_Manager.h"
#include "PlayerManager.h"

BEGIN(Client)

class CExecution_Head final : public CUi
{
protected:
	CExecution_Head(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExecution_Head(const CExecution_Head& rhs);
	virtual ~CExecution_Head() = default;

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

	void Default_Set_Delay(_float fDelay) { m_fTimeDelay = fDelay; m_fTimeDelayOrigin = fDelay; }

private:
	void Set_Ui_Pos(POINT ptPos) { m_UiDesc.m_fX = (_float)ptPos.x; m_UiDesc.m_fY = (_float)ptPos.y; }
	void Set_Ui_Pos(_float fX, _float fY) { m_UiDesc.m_fX = fX; m_UiDesc.m_fY = fY; }

	void Set_Ui_Pos_Add(POINT ptPos) { m_UiDesc.m_fX += (_float)ptPos.x; m_UiDesc.m_fY += (_float)ptPos.y; }
	void Set_Ui_Pos_Add(_float fX, _float fY) { m_UiDesc.m_fX += fX; m_UiDesc.m_fY += fY; }

	void Set_Ui_Size(POINT ptSize) { m_UiDesc.m_fSizeX = (_float)ptSize.x; m_UiDesc.m_fSizeY = (_float)ptSize.y; }
	void Set_Ui_Size(_float fSizeX, _float fSizeY) { m_UiDesc.m_fSizeX = fSizeX; m_UiDesc.m_fSizeY = fSizeY; }

	_float AnimationDelay(_float fTimeDelta) { return m_fTimeDelay -= fTimeDelta; }
	void   AnimationDelayReset() { m_fTimeDelay = m_fTimeDelayOrigin; }

	void Set_Divide(_float fDivide) { m_fDivide = fDivide; }
	_float Get_Divide() { return m_fDivide; }

	//POINT Get_Texture_Info() { return { m_pTextureCom->Get_TextureWidth(), m_pTextureCom->Get_TextureHeight() }; }
	POINT Get_Texture_Info();

	_float2 Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed = 1);

protected:
	virtual HRESULT Add_Components(void* pArg);
	virtual HRESULT Add_Texture(void* pArg);

public:
	static CUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

private:
	_float m_fTimeDelayOrigin = 0.f;
	_float m_fTimeDelay = 0.f;
	_float m_fDivide = 1.f;

	_float3 m_fScale = { 0.f , 0.f, 1.f };
	_float3 m_fRotation = { 0.f, 0.f, 0.f };

	_float m_fLissajousTime = 0.f;

	_float m_fDelayedAnimation = 0.2f;
	_float m_fDelayedAnimation2 = 0.2f;
public:
	virtual void Free() override;
};

END