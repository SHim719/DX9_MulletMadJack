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
	enum PLAYER_STATE { IDLE_STATE, DASH_STATE, JUMP_STATE, PLAYER_STATE_END };
	enum MOVE_STATE { STOP, MOVE, MOVE_END };

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

	void Set_PlayerState(PLAYER_STATE ePlayerState) {
		Camera_Reset();
		this->ePlayerState = ePlayerState; }

	void Set_MoveState(MOVE_STATE eMoveState) { 
		Move_Reset();
		this->eMoveState = eMoveState; 
	}

	//void Get_WeaponType(WEAPON_TYPE& eWeaponType) { eWeaponType = this->eWeaponType; }
	//void Get_HandType(HAND_TYPE& eHandType) { eHandType = this->eHandType; }
	//void Get_AnimationType(ANIMATION_TYPE& eAnimationType) { eAnimationType = this->eAnimationType; }
	//void Get_PlayerState(PLAYER_STATE& ePlayerState) { ePlayerState = this->ePlayerState; }
	//void Get_MoveState(MOVE_STATE& eMoveState) { eMoveState = this->eMoveState; }

	WEAPON_TYPE		Get_WeaponType() { return eWeaponType; }
	HAND_TYPE		Get_HandType() { return eHandType; }
	ANIMATION_TYPE	Get_AnimationType() { return eAnimationType; }
	PLAYER_STATE	Get_PlayerState() { return ePlayerState; }
	MOVE_STATE		Get_MoveState() { return eMoveState; }

private:
	void Key_Input(_float fTimeDelta);
	void HeadTilt(_float fTimeDelta, _float fDirection);
	void HeadTiltReset(_float fTimeDelta);
	void Render_Weapon();
	void Render_Hand();
	
	void Active_Reset();
	void Camera_Reset();
	void Move_Reset();

	void Jump_Tick(_float fTimeDelta);
	void Camera_Shake(_float fTimeDelta, _float fShakePower, _float& fShakeTime);
	void Camera_Event(_float fTimeDelta);

	void Jump(_float _fJumpPower);
	void Shot();
	void SpeedControl(_float fTimeDelta);

	void ColliderCheck(_float fTimeDelta);
	void ColliderTop(_float fTimeDelta);
	void ColliderLeft(_float fTimeDelta);
	void ColliderRight(_float fTimeDelta);
	void ColliderBack(_float fTimeDelta);
	void ColliderFront(_float fTimeDelta);

	void OnCollisionEnter(CGameObject* pOther) override;

public:
	void Camera_Shake_Order(_float fShakePower, _float fShakeTime) { 
		m_fShakePower = fShakePower; 
		m_fShakeTime = fShakeTime; 
	}

	_float Get_PlayerHP() { return m_fPlayerHp; }
	void Set_PlayerHP(_float fPlayerHp) { m_fPlayerHp = fPlayerHp; }
	void Set_PlayerHP_Add(_float fPlayerHp) { m_fPlayerHp += fPlayerHp; }

	void Kick();

	bool Get_PlayerDash() { return bDash; }

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
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
	MOVE_STATE eMoveState = STOP;

	bool bDash = false;

	_float m_fShakePower = 0.f;
	_float m_fShakeTime = 0.f;

	_float m_fJumpPower = 0.f;

	_float m_fPlayerHp = 15.f;

};

END