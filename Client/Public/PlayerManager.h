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

	CPlayer::WEAPON_TYPE Get_Player_WeaponType() { CPlayer::WEAPON_TYPE eWeaponType; m_pPlayer->Get_WeaponType(eWeaponType); return eWeaponType; }
	CPlayer::HAND_TYPE Get_Player_HandType() { CPlayer::HAND_TYPE eHandType; m_pPlayer->Get_HandType(eHandType); return eHandType; }
	CPlayer::ANIMATION_TYPE Get_Player_AnimationType() { CPlayer::ANIMATION_TYPE eAnimationType; m_pPlayer->Get_AnimationType(eAnimationType); return eAnimationType; }

	void Set_Player_TextureIndex(_int iTextureindex) { m_iTextureindex = iTextureindex; }
	_int Get_Player_TextureIndex() { return m_iTextureindex; }

	_float Get_PlayerToTargetX(_float _TargetX);
	_float Get_PlayerToTargetY(_float _TargetY);
	_float Get_PlayerToTargetZ(_float _TargetZ);

	_float Get_PlayerToTarget(_float _TargetX, _float _TargetY, _float _TargetZ);
	_float Get_PlayerToTarget(_float3 _Target);

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	_float						m_fStageClearTime = { 0.f };
	_int						m_iTextureindex = { 0 };

private:
	_float m_fAdjustTime = { 0 };

	CPlayer* m_pPlayer = nullptr;

public:
	virtual void Free() override;
};

END