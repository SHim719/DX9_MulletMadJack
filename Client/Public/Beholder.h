#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "MathManager.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBeholder final : public CPawn
{
public:

	enum ATTACKORDER {
		PLAYERTRACKING, AIRSTRIKETRACKING, FREETRACKING, LANDMINESET, AIRSTRIKE, MONSTERSPAWN, ENDORDER
	};
	
	enum BEHOLDER_PHASE {
		PHASE_WAIT, PHASE_CHARGE, PHASE_GROGGY, PHASE_1, PHASE_2, PHASE_3, PHASE_DEATH, PHASE_END

	};

	enum BEHOLDER_PATTERN
	{
		PATTERN_IDLE,
		PATTERN_WAITING,
		PATTERN_PLAYERTRACKING,
		PATTERN_ALLROUNDLASER,
		PATTERN_ALLROUNDLASERLANDMINE,
		PATTERN_SHOOT,
		PATTERN_AIRSTRIKE,
		PATTERN_ROUND_AIRSTRIKE,
		PATTERN_ROUND_AIRSTRIKE_BOOM,
		PATTERN_ROUND_AIRSTRIKE_LANDMINE,
		PATTERN_ROUND_AIRSTRIKE_REVERSE,
		PATTERN_ROUND_AIRSTRIKE_BOOM_REVERSE,
		PATTERN_END
	};

	struct BeholderAttackOrder
	{
		//_float3 vLook;
		//_float fTime = 0.f;
		ATTACKORDER eOrder;
		_float vSpeed = 1.f;
		_float3 vLook = { 0.f,0.f,0.f };
		_float3		vMasterPos;
	};

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
	CBeholder(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeholder(const CBeholder& rhs);
	virtual ~CBeholder() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };

private:
	_bool On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)		override;
	void OnCollisionEnter(CGameObject* pOther) override;

	_bool Check_HeadShot(_float3 vHitLocalPos);
	_bool Check_BodyShot(_float3 vHitLocalPos);
	_bool Check_EggShot(_float3  vHitLocalPos);

	void PatternState(_float _fTimeDelta);
	void ActivePattern(_float fTimeDelta);

public:
	void PhaseControl(_float fTimeDelta);
	void Phase1Pattern(_float fTimeDelta);
	void Phase2Pattern(_float fTimeDelta);
	void Phase3Pattern(_float fTimeDelta);
	void PhaseChargePattern(_float fTimeDelta);
	void PhaseWaitPattern(_float fTimeDelta);
	void PhaseGroggyPattern(_float fTimeDelta);
	void PhaseDeathPattern(_float fTimeDelta);


	void Player_Tracking_Laser();
	void All_Round_Laser();
	void All_Round_Laser_LandMine();
	void Shoot();
	void AirStrike();
	void RoundAirStrike();
	void RoundAirStrikeBoom(float _fTimeDelta);
	void RoundAirStrikeLandMine(float _fTimeDelta);

	void RoundAirStrikeReverse();
	void RoundAirStrikeBoomReverse(float _fTimeDelta);



	void AirBoom(_float3 vPos);

	void Hit(void* pArg) override;
private:
	STATE			m_eState = STATE_IDLE;

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
	_bool SetState_Execution()			override;
	void SetState_Fly(_float3 vLook)	override;
	void SetState_FlyDeath();
	void SetState_Hit();
	void SetState_Death(ENEMYHIT_DESC* pDesc)	override;

	void Set_PatternEndCheck();
	void Set_PatternStart();
	void Set_PatternEnd();

public:
	_bool Is_DeathState() override { return m_eState == STATE_FLYDEATH || m_eState == STATE_DEATH || m_eState == STATE_FLY; }
	_float Get_Hp() const { return m_fHp; }
	//jeongtest
	_float Get_MaxHp() const { return m_fMaxHp; }
	_float Get_RecentHitTime() const { return m_fRecentHitTime; }
	STATE Get_State() const { return m_eState; }
private:
	_float m_fRecentHitTime = { 0 };


private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState()		override;
	HRESULT			End_RenderState()		override;


public:
	static CBeholder* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float			m_fDamagedTime = 0.f;

	_float3			m_vTargetPos = { 0.f,0.f,0.f };
	_float3			m_vTargetRecentPos = { 0.f,0.f,0.f };
	_float3			m_vTargetDir = { 0.f,0.f,0.f };

	_int			m_iPatternCount = 0;
	_int			m_iPatternCountPhase1Max = 6;
	_int			m_iPatternCountPhase2Max = 9;
	
	_float			m_fPatternTimeDelay = 3.f;
	_float			m_fPatternTimeDelayMax = 4.f;

	_int			m_iShootCount = 0;
	_int			m_iShootCountMax = 5;

	_float			m_fShootDelay = 0.2f;
	_float			m_fShootDelayMax = 0.2f;

	_float			m_fRoundStrikeDelay = 0.4f;
	_float			m_fRoundStrikeDelayMax = 0.4f;

	_float			m_fRoundStrikeBoomDelay = 0.05f;
	_float			m_fRoundStrikeBoomDelayMax = 0.05f;

	_float		    m_fRoundStrikeRadius = 6.f;
	_float			m_fRoundStrikeRadiusMin = 6.f;
	_float			m_fRoundStrikeRadiusMax = 18.f;

	_float          m_fLissajousTime = 0.f;

	_float			m_fPhase3ShootDelay = 0.3f;
	_float			m_fPhase3ShootDelayMax = 0.3f;

	_float			m_fDeathDelay = 5.f;
	_float			m_fDeathDelayMax = 5.f;

	_float			m_fDeathExplodeDelay = 0.15f;
	_float			m_fDeathExplodeDelayMax = 0.15f;

	BEHOLDER_PATTERN m_ePattern = PATTERN_IDLE;
	BEHOLDER_PHASE	 m_ePhase = PHASE_WAIT;

	class CUI_FadeInOut* m_pFadeInOut = { nullptr };

	_bool			m_bCutScene = false;
public:
	void Set_CutScene(_bool bCutScene) { m_bCutScene = bCutScene; }

	
	//BEHOLDER_PHASE	 m_ePhase	= PHASE_WAIT;
	BEHOLDER_PHASE	 m_ePhase = PHASE_CHARGE;
};

END