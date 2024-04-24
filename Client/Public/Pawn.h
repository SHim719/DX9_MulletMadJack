#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "CUi_MonsterDie.h"
#include "PlayerManager.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPawn abstract : public CGameObject
{
public:
	enum HITTYPE
	{
		HEAD_SHOT,
		BODY_SHOT,
		EGG_SHOT,
		HITTYPE_END,
	};

	typedef struct tagEnemyHitDesc
	{
		// WeaponType;
		HITTYPE eHitType;
		_float3 fHitWorldPos;
		_float fDist;
	}ENEMYHIT_DESC;
protected:
	CPawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPawn(const CPawn& rhs);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual void SetState_Pushed(_float3 vLook) {}
	virtual _bool SetState_Execution() { return false; }
	virtual void SetState_Fly(_float3 vLook) {}

	virtual _bool Is_DeathState() { return false; }

	void Set_SlopeStand(_bool bStand) { m_bSlopeStand = bStand; }
	void Set_Execution_Target() { CPlayer_Manager::Get_Instance()->Set_Executrion_Target(Get_SubTag()); }

protected:
	void Call_MonsterDieUi(eMonsterGrade Grade);

protected:
	class CFPS_Camera*	m_pCamera = { nullptr };
	class CPlayer*		m_pTarget = { nullptr };

	_float			m_fHp = 8.f;
	_float			m_fSpeed = 1.f;
	_float			m_fPerceptionDist = 5.f;
	_float			m_fRange = 1.f;
	_bool			m_bPushRecovery = { false };

	_float			m_fDeathTimeAcc = 0.f;
	_float			m_fDeathTime = 3.f;

	_float			m_fFlyTimeAcc = 0.f;
	_float			m_fFlyTime = 1.f;
	_bool			m_bWallColl = false;
	_bool			m_bFirstMeet = false;
	_bool			m_bSlopeStand = false;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END