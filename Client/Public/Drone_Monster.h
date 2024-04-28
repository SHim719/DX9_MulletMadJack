#pragma once
#include "Client_Defines.h"
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
		STATE_MOVE,
		STATE_RUSH,
		STATE_BOUND,
		STATE_PUSHED,
		STATE_DEATH,
		STATE_END,
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
	CVIBuffer*		m_pVIBufferCom = { nullptr };
	CAnimation*		m_pAnimationCom = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };
	CRigidbody*		m_pRigidbody = { nullptr };

private:
	_bool On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)		override;
	void  OnCollisionEnter(CGameObject* pOther) override;

	void Hit(void* pArg) override;

	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Move();
	void State_Rush();
	void State_Bound(_float fTimeDelta);
	void State_Pushed(_float fTimeDelta);
	void State_Death();

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Rush();
	void SetState_Bound();
	void SetState_Pushed(_float3 vLook)	override;
	void SetState_Fly(_float3 vLook) override { SetState_Pushed(vLook); } 
	void SetState_Death(ENEMYHIT_DESC* pDesc)	override;

private:
	STATE			m_eState = STATE_IDLE;

	_float			m_fBoundTime = 0.5f;
	_float			m_fBoundTimeAcc = 0.f;

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