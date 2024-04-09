#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	enum WEOPON_TYPE { PISTOL, TYPE_END };
	enum HAND_TYPE { LEFT_HAND, RIGHT_HAND, BOTH_HAND };

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Key_Input(_float fTimeDelta);
	void HeadTilt(_float fTimeDelta, _float fDirection);
	void HeadTiltReset(_float fTimeDelta);
public:
	/* 원형을 생성하기위한 함수. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	/* 사본을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CBoxCollider* m_pBoxCollider = nullptr;

	_float fMaxHeadTilt = 2.0f;

	_float fMaxHeadTiltSpeed = 1200.f;
	
	_float fHeadTilt = 0.0f;
};

END