#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "CGame_Manager.h"
#include "Base.h"
#include "Player.h"


BEGIN(Client)

class CPlayer_Manager : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)
public:
	enum ACTION_TYPE { ACTION_NONE, ACTION_EXECUTION, ACTION_WEAPONCHANGE, ACTION_DRINKCAN, ACTION_CUTIN_SHOP, ACTION_END };
private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;

public:
	void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);

public:
	void Tick(_float fTimeDelta);

	void Set_Player(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* Get_Player() { return m_pPlayer; }

	void Set_Player_WeaponType(CPlayer::WEAPON_TYPE eWeaponType) { m_pPlayer->Set_WeaponType(eWeaponType); }
	void Set_Player_HandType(CPlayer::HAND_TYPE eHandType) { m_pPlayer->Set_HandType(eHandType); }
	void Set_Player_AnimationType(CPlayer::ANIMATION_TYPE eAnimationType) { m_pPlayer->Set_AnimationType(eAnimationType); }
	void Set_Player_State(CPlayer::PLAYER_STATE ePlayerState) { m_pPlayer->Set_PlayerState(ePlayerState); }
	void Set_Player_MoveState(CPlayer::MOVE_STATE eMoveState) { m_pPlayer->Set_MoveState(eMoveState); }

	CPlayer::WEAPON_TYPE Get_Player_WeaponType() { return m_pPlayer->Get_WeaponType(); }
	CPlayer::HAND_TYPE Get_Player_HandType() { return m_pPlayer->Get_HandType(); }
	CPlayer::ANIMATION_TYPE Get_Player_AnimationType() { return  m_pPlayer->Get_AnimationType(); }
	CPlayer::PLAYER_STATE Get_Player_State() { return m_pPlayer->Get_PlayerState(); }
	CPlayer::MOVE_STATE Get_Player_MoveState() { return m_pPlayer->Get_MoveState(); }

	void Set_Player_TextureIndex(_int iTextureindex) { m_iTextureindex = iTextureindex; }
	_int Get_Player_TextureIndex() { return m_iTextureindex; }

	_float Get_PlayerToTargetX(_float _TargetX);
	_float Get_PlayerToTargetY(_float _TargetY);
	_float Get_PlayerToTargetZ(_float _TargetZ);

	_float Get_PlayerToTarget(_float _TargetX, _float _TargetY, _float _TargetZ);
	_float Get_PlayerToTarget(_float3 _Target);

	_float2 Lissajous_Adjust(_float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta);

	_float2 Get_Lissajous_Run(_float _fPosX, _float _fPosY) {
		return Lissajous_Adjust(m_fAdjustTime, _fPosX, _fPosY, 50, 30, 1, 2, 2);
	}

	_float2 Get_Lissajous_Run_Phone(_float _fPosX, _float _fPosY) {
		return Lissajous_Adjust(m_fAdjustTime, _fPosX, _fPosY, 20, 20, 1, 2, 2);
	}
	void Set_AdjustTime(_float _fAdjustTime) { m_fAdjustTime = _fAdjustTime; }
	void Tick_AdjustTime(_float _fTimeDelta, _float _fTimeSpeed) { m_fAdjustTime += _fTimeDelta * _fTimeSpeed; }
	_float Get_AdjustTime() { return m_fAdjustTime; }

	_float Get_LeftArmRotate() { return m_fLeftArmRotate; }

	_float Get_PlayerHP() { return m_pPlayer->Get_PlayerHP(); }
	void Set_PlayerHP(_float _fPlayerHp) { m_pPlayer->Set_PlayerHP(_fPlayerHp); }
	void Set_PlayerHP_Add(_float _fPlayerHp) { m_pPlayer->Set_PlayerHP_Add(_fPlayerHp); }
	void Set_PlayerHP_Damaged(_float _fPlayerHp) {
		m_pPlayer->Set_PlayerHP_Damaged(_fPlayerHp);
		if (m_pPlayer->Get_SuperInvincible())
		{

		}
		else
		{
			m_pPlayer->Set_PlayerHP_Damaged(_fPlayerHp);
		}
	}

	void Camera_Shake_Order(_float _fShakeTime, _float _fShakePower) { m_pPlayer->Camera_Shake_Order(_fShakePower, _fShakeTime); }

	void Execution_Ready() { Set_Action_Type(ACTION_EXECUTION); }
	//void Set_Execution_Action(_bool _bExecution) { m_bExecution = _bExecution; }
	//_bool Get_Execution_Action() { return m_bExecution; }

	void WeaponChange_Ready() { Set_Action_Type(ACTION_WEAPONCHANGE); }
	void Set_WeaponChange_Action(_bool _bWeaponChange) { m_bWeaponChange = _bWeaponChange; }
	_bool Get_WeaponChange_Action() { return m_bWeaponChange; }

	void DrinkCan_Ready() { Set_Action_Type(ACTION_DRINKCAN); }
	void Set_DrinkCan_Action(_bool _bDrinkCan) { m_bDrinkCan = _bDrinkCan; }
	_bool Get_DrinkCan_Action() { return m_bDrinkCan; }

	void Set_TempDisable(_bool _bTempDisable) { m_bTempDisable = _bTempDisable; }
	_bool Get_TempDisable() { return m_bTempDisable; }

	void Set_TempDisablePosition(_float _fTempDisablePosition) { m_fTempDisablePosition = _fTempDisablePosition; }
	_float Get_TempDisablePosition() { return m_fTempDisablePosition; }
	_float2 Get_TempDisablePosition_BothHand();


	const _float Get_TempDisablePositionLimit() { return m_fTempDisablePositionLimit; }

	void Set_Action_Type(ACTION_TYPE _eActionType) {
		Set_DisableEnd(false);
		m_eActionType = _eActionType;
	}
	ACTION_TYPE Get_Action_Type() { return m_eActionType; }

	void Set_DisableEnd(_bool _bTempDisableEnd) { m_bTempDisableEnd = _bTempDisableEnd; }
	_bool Get_DisableEnd() { return m_bTempDisableEnd; }

	void Execution_Monster() { Execution_Ready(); }

	void Set_MouseLock(_bool _bMouseLock) { m_bMouseLock = _bMouseLock; }
	_bool Get_MouseLock() { return m_bMouseLock; }

	void Set_ComboTime() { m_fComboTime = m_fComboTimeLimit; }
	void Add_Combo() {
		m_iCombo++;
		Set_ComboTime();
	}
	_int Get_Combo() { return m_iCombo; }

	void Combo_System(_float fTimeDelta);

	_float Get_InvincibleTime() { return m_pPlayer->Get_InvincibleTime(); }
	void Set_InvincibleTime(_float _fInvincibleTime) { m_pPlayer->Set_InvincibleTime(_fInvincibleTime); }

	_bool Get_Invincible() { return m_pPlayer->Get_Invincible(); }
	void Set_Invincible(_bool _bInvincible) { m_pPlayer->Set_Invincible(_bInvincible); }

	_int Get_Magazine() { return m_iWeaponMagezine; }
	void Set_Magazine(_int _iMagazine) { m_iWeaponMagezine = _iMagazine; }
	void Fire_Magazine() { m_iWeaponMagezine--; }
	void Reload_Magazine() { m_iWeaponMagezine = m_iWeaponMaxMagezine; }

	_int Get_MaxMagazine() { return m_iWeaponMaxMagezine; }
	void Set_MaxMagazine(_int _iMagazine) { m_iWeaponMaxMagezine = _iMagazine; m_iWeaponMagezine = m_iWeaponMaxMagezine; }

	void Shop_System(void* Arg);

	void Set_WeaponType(CPlayer::WEAPON_TYPE eWeaponType) { m_pPlayer->Set_WeaponType(eWeaponType); }
	CPlayer::WEAPON_TYPE Get_WeaponType() { return m_pPlayer->Get_WeaponType(); }

	void WeaponChange(CPlayer::WEAPON_TYPE eWeaponType);

	_int Get_SlashCount() { m_iSlashCount++; if (m_iSlashCount >= 3) m_iSlashCount = 0; return m_iSlashCount; }
	_int Get_RealSlashCount() { return m_iSlashCount; }

	void Set_Executrion_Target(string _strExecutionTarget) { m_strExecutionTarget = _strExecutionTarget; }
	string Get_Execution_Target() { return m_strExecutionTarget; }

	_bool Is_ActionIdle() { return m_bActionIDLE; }

	void Set_Pattern(_bool _bPattern) { m_bPattern = _bPattern; }
	_bool Get_IsPattern() { return m_bPattern; }

	void Set_RoundPattern(_bool _bRoundPattern) { m_bRoundPattern = _bRoundPattern; }
	_bool Get_IsRoundPattern() { return m_bRoundPattern; }

	void Set_NextWeapon(CPlayer::WEAPON_TYPE eWeaponType) { m_eNextWeapon = eWeaponType; }
	CPlayer::WEAPON_TYPE Get_NextWeapon() { return m_eNextWeapon; }
private:
	CGameInstance* m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	_float						m_fStageClearTime = { 0.f };
	_int						m_iTextureindex = { 0 };
	_float						m_fAdjustTime = { 0 };

	_float						m_fLeftArmRotate = 30.f;

	ACTION_TYPE					m_eActionType = ACTION_NONE;

	_bool						m_bActionIDLE = true;


	_float						m_fTempDisablePosition = 0.f;
	_float						m_fTempDisablePositionLimit = 500.f;

	_bool						m_bTempDisable = false;
	_bool						m_bTempDisableEnd = false;

	_bool						m_bWeaponChange = false;
	_bool						m_bWeaponChangeReady = false;

	_bool						m_bDrinkCan = false;
	_bool						m_bDrinkCanReady = false;

	_bool				        m_bMouseLock = false;

	_int						m_iCombo = 0;

	_float						m_fComboTime = 0.f;
	_float						m_fComboTimeLimit = 5.f;

	//Weapon
	_int						m_iWeaponMagezine = 8;
	_int						m_iWeaponMaxMagezine = 8;

	_int						m_iSlashCount = 0;

	string						m_strExecutionTarget = "";

	_bool						m_bPattern = false;
	_bool						m_bRoundPattern = false;

	CPlayer::WEAPON_TYPE		m_eNextWeapon = CPlayer::PISTOL;

private:
	CPlayer* m_pPlayer = nullptr;

public:
	virtual void Free() override;

};

END