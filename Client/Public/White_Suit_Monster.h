#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "FPS_Camera.h"
#include "Core_Camera.h"
#include "Pawn.h"

enum EventType
{
	SSDA,
	ADSD,
	ADDSD,
};

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CWhite_Suit_Monster final : public CPawn
{
private:

	enum STATE
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_ALERT,
		STATE_PUSHED,
		STATE_SHOT,
		STATE_JUMP,
		STATE_HIT,
		STATE_EXECUTION,
		STATE_FLY,
		STATE_FLYDEATH,
		STATE_DEATH,
		STATE_END
	};

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
	CVIBuffer*		m_pVIBufferCom = { nullptr };
	CAnimation*		m_pAnimationCom = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };
	CRigidbody*		m_pRigidbody = { nullptr };

private:
	_bool On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)		override;
	void OnCollisionEnter(CGameObject* pOther) override;

	_bool Check_HeadShot(_float3 vHitLocalPos);
	_bool Check_BodyShot(_float3 vHitLocalPos);
	_bool Check_EggShot(_float3  vHitLocalPos);

	void Hit(void* pArg) override;
private:
	STATE			m_eState = STATE_IDLE;
	_float			m_fHp = 5.f;
	_float			m_fSpeed = 1.f;
	_float			m_fPerceptionDist = 3.f;
	_bool			m_bPushRecovery = { false };

	_float			m_fTimeAcc = 0.f;
	_float			m_fDeathTime = 3.f;

	_float			m_fFlyTimeAcc = 0.f;
	_float			m_fFlyTime = 1.f;
	_bool			m_bWallColl = false;
private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Move();
	void State_Alert();
	void State_Pushed();
	void State_Shot();
	void State_Jump();
	void State_Execution();
	void State_Fly(_float fTimeDelta);
	void State_FlyDeath(_float fTimeDelta);
	void State_Hit();
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Alert();
	void SetState_Pushed(_float3 vLook) override;
	void SetState_Shot();
	void SetState_Jump();
	void SetState_Execution()			override;
	void SetState_Fly(_float3 vLook)	override;
	void SetState_FlyDeath();
	void SetState_Hit();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

public:
	_bool Is_DeathState() override { return m_eState == STATE_FLYDEATH || m_eState == STATE_DEATH || m_eState == STATE_FLY; }

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState()		override;
	HRESULT			End_RenderState()		override;


public:
	static CWhite_Suit_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END