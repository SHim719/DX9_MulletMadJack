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

class CChainsaw_Monster final : public CPawn
{
	enum STATE
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_PUSHED,
		STATE_SLASH,
		STATE_JUMP,
		STATE_HIT,
		STATE_EXECUTION,
		STATE_FLY,
		STATE_FLYDEATH,
		STATE_DEATH,
		STATE_END,
	};

private:
	CChainsaw_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChainsaw_Monster(const CChainsaw_Monster& rhs);
	virtual ~CChainsaw_Monster() = default;

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

	void Hit(void* pArg) override;

private:
	STATE			m_eState = STATE_IDLE;
	_float			m_fHp = 8.f;
	_float			m_fSpeed = 2.f;
	_float			m_fPerceptionDist = 3.f;
	_float			m_fRange = 1.f;
	_bool			m_bPushRecovery = { false };
	_bool			m_bPerceivedPlayer = { false };

	_float			m_fDeathTimeAcc = 0.f;
	_float			m_fDeathTime = 3.f;

private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Move();
	void State_Pushed();
	void State_Slash();
	void State_Jump();
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Pushed(_float3 vLook);
	void SetState_Slash();
	void SetState_Jump();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState();
	HRESULT			End_RenderState();

public:
	static CChainsaw_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END