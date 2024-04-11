#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "FPS_Camera.h"
#include "Core_Camera.h"
#include "Pawn.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CWhite_Suit_Monster final : public CPawn
{
	enum STATE { STATE_IDLE, STATE_AIM, STATE_WALK, STATE_SHOOT, STATE_BLOCK, STATE_HEADSHOT, STATE_END };

private:
	CWhite_Suit_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWhite_Suit_Monster(const CWhite_Suit_Monster& rhs);
	virtual ~CWhite_Suit_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };

	CFPS_Camera* m_pFPS_Camera = { nullptr };


private:
	PAWN_DESC		m_White_Suit_Monster_Desc{};

	_float			m_fBullet_TimeGap;
	_float			m_fWalking_TimeGap;
	_float			m_fShooting_TimeGap;
	_float			m_fBlocking_TimeGap;

	bool			IsPlaying;

	bool			m_bAiming;
	bool			m_bDead;
	bool			m_bWalking;
	bool			m_bShooting;
	bool			m_bBlock;

	STATE			m_eState;

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState();
	HRESULT			End_RenderState();

	virtual void	Set_Motions(_float fTimeDelta) override;

	void	Decide_Pawn_Motions(_float fTimeDelta);
	void	Pawn_Aiming_Motion(_float fTimeDelta);
	void	Pawn_Shooting_Motion(_float fTimeDelta);
	void	Pawn_Walking_Motion(_float fTimeDelta);
	void	Pawn_Blocking_Motion(_float fTimeDelta);
	void	Pawn_Dying_Motion(_float fTimeDelta);

public:
	static CWhite_Suit_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END