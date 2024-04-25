#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "Beholder.h"
#include "PlayerManager.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Box;
END

BEGIN(Client)
enum class TrackingLaserState
{
	Warning,
	Fire,
	End
};
class CTrackingLaser final : public CGameObject
{
private:
	CTrackingLaser(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrackingLaser(const CTrackingLaser& rhs);
	virtual ~CTrackingLaser() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	void Initialize_Arg(void* pArg);
	void Initialize_Order();
	void Set_Collider();
	void PriorityTick(_float fTimeDelta) override;
	void Tick(_float fTimeDelta) override;
	void LateTick(_float fTimeDelta) override;
	void BeginRenderState();
	HRESULT Render() override;
	void EndRenderState();


private:
	void OnTriggerEnter(CGameObject* pOther) override;

private:
	void Prepair_Order();
	void Execute_Order();
	void PlayerTracking_Order();

private:
	HRESULT Add_Components();
	HRESULT Add_Texture();
	void Arg_InitializeSetPosScale();


private:
	void AdjustAlpha(_float fTimeDelta);
	void AdjustScale(_float fTimeDelta);


private:
	TrackingLaserState m_eState = { TrackingLaserState::End };
	_float m_fLife = { 5.f };
	_float m_fAlphaTime = { 0.f };
	_float m_fScaleTime = { 0.f };
	_float3 m_fScale = { 1, 1, 1 };
	_uint m_iAlpha = { 50 };


private:
	CVIBuffer_Box* m_pVIBufferCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };
	CGameObject* m_pTarget = { nullptr };

	_float3 m_vTargetPos = { 0.f, 0.f, 0.f };
	_float3 m_vMasterPos = { 0.f, 0.f, 0.f };

	CBeholder::ATTACKORDER m_eAttackOrder = { CBeholder::ATTACKORDER::ENDORDER };

public:
	static CTrackingLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; // pArg->_float3(Pos)
	virtual void Free() override;
};

END