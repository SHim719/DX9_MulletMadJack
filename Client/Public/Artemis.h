#pragma once
#include "Client_Defines.h"
#include "Pawn.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CArtemis final : public CPawn
{
	enum STATE
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_DEATH,
		STATE_END,
	};

private:
	CArtemis(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArtemis(const CArtemis& rhs);
	virtual ~CArtemis() = default;

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
	void  OnCollisionEnter(CGameObject* pOther) override;

	void Hit(void* pArg) override;

	void Process_State(_float fTimeDelta);

	void State_Idle(float _fTimeDelta);
	void State_Move(float _fTimeDelta);
	void State_Death();

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

private:
	STATE			m_eState = STATE_IDLE;

	_float			m_fBoundTime = 0.5f;
	_float			m_fBoundTimeAcc = 0.f;

	_float          m_fMoveDelay = 2.f;
	_float          m_fMoveDelayMax = 2.f;

	_float3			m_vTargetPosition = { 0.f, 0.f, 0.f };
	_float3			m_vTargetDirection = { 0.f, 0.f, 0.f };

	_float			m_fMinSpeed = 5.f;
	_float			m_fMaxSpeed = 20.f;
	_float			m_fSpeed = 5.f;

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState();
	HRESULT			End_RenderState();

public:
	static CArtemis* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END