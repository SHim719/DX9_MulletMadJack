#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
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
	enum PLAYER_STATE { IDLE_STATE, DASH_STATE, AIRDASH_STATE, SLOPE_STATE, EXECUTION_STATE, PLAYER_STATE_END };
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
		//Move_Reset();
		this->eMoveState = eMoveState; 
	}

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

	void Camera_Shake(_float fTimeDelta, _float fShakePower, _float& fShakeTime);
	void Camera_Event(_float fTimeDelta);

	void Shot();

	void ColliderCheck();
	void ColliderUpDown();

	void OnCollisionEnter(CGameObject* pOther) override;

	void Process_State(_float fTimeDelta);

	void Idle_State(_float fTimeDelta);
	void Dash_State(_float fTimeDelta);
	void AirDash_State(_float fTimeDelta);
	void Slope_State(_float fTimeDelta);
	void Execution_State();

	void SetState_Idle();
	void SetState_Dash();
	void SetState_AirDash();
	void SetState_Slope();
	void SetState_Execution();
public:
	void Camera_Shake_Order(_float fShakePower, _float fShakeTime) { 
		m_fShakePower = fShakePower; 
		m_fShakeTime = fShakeTime; 
	}

	_float Get_PlayerHP() { return m_fPlayerHp; }
	void Set_PlayerHP(_float fPlayerHp) { m_fPlayerHp = fPlayerHp; }
	void Set_PlayerHP_Add(_float fPlayerHp) { m_fPlayerHp += fPlayerHp; }

	void Kick();
	
	void Set_TimeDivide(_float fTimeDivide) { CGameInstance::Get_Instance()->Set_TimeDivide(fTimeDivide); }
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CBoxCollider* m_pBoxCollider = nullptr;
	CRigidbody* m_pRigidbody = nullptr;
	CTransform* m_pSlopeTransform = nullptr;

	_float fMaxHeadTilt = 2.0f;
	_float fHeadTilt = 0.0f;

	_float fDefaultLissajousTime = 0.0f;

	WEAPON_TYPE eWeaponType = PISTOL;
	HAND_TYPE eHandType = IDLE_HAND;
	ANIMATION_TYPE eAnimationType = IDLE;
	PLAYER_STATE ePlayerState = IDLE_STATE;
	MOVE_STATE eMoveState = STOP;

	_bool m_bHaveWeapon = true;
	class CPawn* m_pExecutionEnemy = nullptr;

	_float m_fDashTime = 0.2f;
	_float m_fDashTimeAcc = 0.f;
	_bool m_bCanAirDash = true;

	_float m_fMoveSpeed = 5.f;

	_float m_fShakePower = 0.f;
	_float m_fShakeTime = 0.f;

	_float m_fJumpPower = 0.f;

	_float m_fPlayerHp = 15.f;

};

END