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
	void Prepair_Order(float _fTimeDelta);
	void Execute_Order();

	void PlayerTracking_Order();
	void PlayerTracking_Prepair();

	void FreeTracking_Order();
	void FreeTracking_Prepair(float _fTimeDelta);

	void LandMineSet_Order();
	void LandMineSet_Prepair(float _fTimeDelta);

	void AirStrike_Order();
	void AirStrike_Prepair(float _fTimeDelta);

	void TrackingAirStrike_Order();
	void TrackingAirStrike_Prepair(float _fTimeDelta);

	void MonsterSpawn_Order();
	void MonsterSpawn_Prepair(float _fTimeDelta);

	void AirStrike();
	void AirStrikeBoom(_float3 vPos);

	void MonsterSpawn();
private:
	HRESULT Add_Components();
	HRESULT Add_Texture();
	void Arg_InitializeSetPosScale();


private:
	void AdjustAlpha(_float fTimeDelta);
	void AdjustScale(_float fTimeDelta);


private:
	TrackingLaserState m_eState = { TrackingLaserState::End };
	_float m_fLife = { 3.f };
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
	_float3 m_vLissajousPos = { 0.f, 0.f, 0.f };

	_float  m_fLissajousTime = { 0.f };
	_float  m_fLissajousSpeed = { 1.f };
	_float  m_fLissajousLagrangianX = { 0.f };
	_float  m_fLissajousLagrangianY = { 0.f };
	_float  m_fLissajousPhaseDelta = { 0.f };

	_float  m_fLandMineCoolTime = { 0.f };
	_float  m_fLandMineCoolTimeMax = { 0.2f };

	_float3 m_vMasterPos = { 0.f, 0.f, 0.f };
	_float  m_fRotate = { 0.f };

	CBeholder::ATTACKORDER m_eAttackOrder = { CBeholder::ATTACKORDER::ENDORDER };

public:
	static CTrackingLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; // pArg->_float3(Pos)
	virtual void Free() override;
};

END