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

class COrange_Pants_Monster final : public CPawn
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
		STATE_DEATH,
		STATE_END
	};

private:
	COrange_Pants_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	COrange_Pants_Monster(const COrange_Pants_Monster& rhs);
	virtual ~COrange_Pants_Monster() = default;

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

	_bool Check_HeadShot(_float3 vHitLocalPos);
	_bool Check_BodyShot(_float3 vHitLocalPos);
	_bool Check_EggShot(_float3  vHitLocalPos);

	void Hit(void* pArg) override;

private:
	STATE			m_eState = STATE_IDLE;
	_float			m_fHp = 3.f;
	_float			m_fSpeed = 1.f;
	_float			m_fPerceptionDist = 3.f;
	_float			m_fRange = 6.f;
	_bool			m_bPushRecovery = { false };
	_bool			m_bPerceivedPlayer = { false };

	_float			m_fTimeAcc = 0.f;
	_float			m_fDeathTime = 3.f;

private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Move();
	void State_Alert();
	void State_Pushed();
	void State_Shot();
	void State_Jump();
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Alert();
	void SetState_Pushed(_float3 vLook);
	void SetState_Shot();
	void SetState_Jump();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState()		override;
	HRESULT			End_RenderState()		override;

public:
	static COrange_Pants_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END