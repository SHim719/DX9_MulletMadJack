#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	enum WEAPON_TYPE { PISTOL, WEAPON_END };
	enum HAND_TYPE { IDLE_HAND, BOTH_HAND, HAND_END };
	enum ANIMATION_TYPE { IDLE, SHOT, RELOAD, SPIN, ANIMATION_END };
	enum PLAYER_STATE { IDLE_STATE, RUN_STATE, PLAYER_STATE_END };

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_WeaponType(WEAPON_TYPE eWeaponType) { 
		Active_Reset();
		this->eWeaponType = eWeaponType; }
	
	void Set_HandType(HAND_TYPE eHandType) {
		Active_Reset();
		this->eHandType = eHandType; }

	void Set_AnimationType(ANIMATION_TYPE eAnimationType) { 
		Active_Reset();
		this->eAnimationType = eAnimationType; }

	void Get_WeaponType(WEAPON_TYPE& eWeaponType) { eWeaponType = this->eWeaponType; }
	void Get_HandType(HAND_TYPE& eHandType) { eHandType = this->eHandType; }
	void Get_AnimationType(ANIMATION_TYPE& eAnimationType) { eAnimationType = this->eAnimationType; }
	void Get_PlayerState(PLAYER_STATE& ePlayerState) { ePlayerState = this->ePlayerState; }

private:
	void Key_Input(_float fTimeDelta);
	void HeadTilt(_float fTimeDelta, _float fDirection);
	void HeadTiltReset(_float fTimeDelta);
	void Render_Weapon();
	void Render_Hand();
	
	void Active_Reset();
	void Camera_Shake(_float fTimeDelta, _float fShakePower, _float& fShakeTime);
	void Camera_Event(_float fTimeDelta);

public:
	void Camera_Shake_Order(_float fShakePower, _float fShakeTime) { 
		m_fShakePower = fShakePower; 
		m_fShakeTime = fShakeTime; 
	}

public:
	/* 원형을 생성하기위한 함수. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	/* 사본을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CBoxCollider* m_pBoxCollider = nullptr;

	_float fMaxHeadTilt = 2.0f;
	_float fHeadTilt = 0.0f;

	_float fDefaultLissajousTime = 0.0f;

	WEAPON_TYPE eWeaponType = PISTOL;
	HAND_TYPE eHandType = IDLE_HAND;
	ANIMATION_TYPE eAnimationType = IDLE;
	PLAYER_STATE ePlayerState = IDLE_STATE;

	_float m_fShakePower = 0.f;
	_float m_fShakeTime = 0.f;
};

END