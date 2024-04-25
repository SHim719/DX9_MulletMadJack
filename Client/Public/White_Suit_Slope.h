#pragma once
#include "Client_Defines.h"
#include "Pawn.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CWhite_Suit_Slope final : public CPawn
{
private:

	enum STATE
	{
		STATE_IDLE,
		STATE_ALERT,
		STATE_SHOT,
		STATE_HIT,
		STATE_FLY,
		STATE_DEATH,
		STATE_END
	};

private:
	CWhite_Suit_Slope(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWhite_Suit_Slope(const CWhite_Suit_Slope& rhs);
	virtual ~CWhite_Suit_Slope() = default;

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
	void OnTriggerEnter(CGameObject* pOther) override;

	_bool Check_HeadShot(_float3 vHitLocalPos);
	_bool Check_BodyShot(_float3 vHitLocalPos);
	_bool Check_EggShot(_float3  vHitLocalPos);

	void Hit(void* pArg) override;
private:
	STATE			m_eState = STATE_IDLE;

private:
	void Process_State(_float fTimeDelta);

	void State_Idle();
	void State_Alert();
	void State_Shot();
	void State_Fly(_float fTimeDelta);
	void State_Hit();
	void State_Death(_float fTimeDelta);

public:
	void SetState_Idle();
	void SetState_Alert();
	void SetState_Shot();
	void SetState_Fly(_float3 vLook)	override;
	void SetState_Hit();
	void SetState_Death(ENEMYHIT_DESC* pDesc);

public:
	_bool Is_DeathState() override { return m_eState == STATE_DEATH || m_eState == STATE_FLY; }

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState()		override;
	HRESULT			End_RenderState()		override;


public:
	static CWhite_Suit_Slope* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END