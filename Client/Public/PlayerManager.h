#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"
#include "Player.h"

BEGIN(Client)

class CPlayer_Manager : public CBase
{
DECLARE_SINGLETON(CPlayer_Manager)
public:

private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;

public:
	void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);

public:
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
	
	_float2 Get_Lissajous_Run(_float _fPosX, _float _fPosY){
		return Lissajous_Adjust(m_fAdjustTime, _fPosX, _fPosY, 50, 30, 1, 2, 2);
	}

	void Set_AdjustTime(_float _fAdjustTime) { m_fAdjustTime = _fAdjustTime; }
	void Tick_AdjustTime(_float _fTimeDelta, _float _fTimeSpeed) { m_fAdjustTime += _fTimeDelta * _fTimeSpeed; }
	_float Get_AdjustTime() { return m_fAdjustTime; }

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	_float						m_fStageClearTime = { 0.f };
	_int						m_iTextureindex = { 0 };
	_float						m_fAdjustTime = { 0 };

private:

	CPlayer* m_pPlayer = nullptr;

public:
	virtual void Free() override;

};

END