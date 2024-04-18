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
	virtual void SetState_Execution() {}
	virtual void SetState_Fly(_float3 vLook) {}

	virtual _bool Is_DeathState() { return false; }

protected:
	void Call_MonsterDieUi(eMonsterGrade Grade);

protected:
	class CFPS_Camera*	m_pCamera = { nullptr };
	class CPlayer*		m_pTarget = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END