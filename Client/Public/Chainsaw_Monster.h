#pragma once

#include "Client_Defines.h"
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
		STATE_GETUP,
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
	void OnCollisionStay(CGameObject* pOther) override;

	_bool Check_HeadShot(_float3 vHitLocalPos);
	_bool Check_BodyShot(_float3 vHitLocalPos);
	_bool Check_EggShot(_float3  vHitLocalPos);

	void Hit(void* pArg) override;

	STATE			m_eState = STATE_IDLE;
private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Move();
	void State_Pushed();
	void State_Slash();
	void State_Jump();
	void State_GetUp();
	void State_Execution();
	void State_Fly(_float fTimeDelta);
	void State_FlyDeath(_float fTimeDelta);
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Move();
	void SetState_Pushed(_float3 vLook)	override;
	void SetState_Slash();
	void SetState_Jump();
	void SetState_GetUp();
	void SetState_Execution()			override;
	void SetState_Fly(_float3 vLook)	override;
	void SetState_FlyDeath();
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