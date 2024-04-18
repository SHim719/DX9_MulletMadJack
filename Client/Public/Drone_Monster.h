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

class CDrone_Monster final : public CPawn
{
	enum STATE
	{
		STATE_IDLE,
		STATE_AIM,
		STATE_ALERT,
		STATE_ATTACK,
		STATE_DEATH,
		STATE_END
	};

private:
	CDrone_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDrone_Monster(const CDrone_Monster& rhs);
	virtual ~CDrone_Monster() = default;

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
	virtual void	On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg = nullptr) override;
	virtual void	OnCollisionEnter(CGameObject* pOther) override;

	_bool Check_Hit(_float3 vHitLocalPos);

	void Hit(void* pArg) override;

private:
	STATE			m_eState = STATE_IDLE;
	_float			m_fHp = 1.f;
	_float			m_fSpeed = 0.1f;
	_float			m_fPerceptionDist = 3.f;
	_float			m_fRange = 6.f;
	_bool			m_bPushRecovery = { false };
	_bool			m_bPerceivedPlayer = { false };

	_float			m_fTimeAcc = 0.f;
	_float			m_fDeathTime = 3.f;

private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Aim();
	void State_Alert();
	void State_Attack(_float fTimeDelta);
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Aim();
	void SetState_Alert();
	void SetState_Attack();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState();
	HRESULT			End_RenderState();

public:
	static CDrone_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END