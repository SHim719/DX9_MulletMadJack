#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Client)

class CRigidbody : public CComponent
{
protected:
	CRigidbody(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRigidbody(const CRigidbody& rhs);
	virtual ~CRigidbody() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Update(_float fTimeDelta);

private:
	CTransform* m_pOwnerTransform = { nullptr };

	_float3		m_vVelocity = { 0.f, 0.f, 0.f };
	_float3		m_vGravity = { 0.f, -8.f, 0.f };

	_bool		m_bGround = { false };
	_bool		m_bUseGravity = true;

	//_float		m_fMass;
	//_float3		m_vAccel;
	//_float3		m_vForce;

	_float		m_fFriction = 0.f;

public:
	void Set_Velocity(const _float3& fVelocity)
	{
		m_vVelocity = fVelocity;
	}
	void Add_Velocity(const _float3& fVelocity)
	{
		m_vVelocity += fVelocity;
	}
	const _float3& Get_Velocity() const { return m_vVelocity; }

	void Set_VelocityX(const _float fX) { m_vVelocity.x = fX; }
	void Set_VelocityY(const _float fY) { m_vVelocity.y = fY; }
	void Set_VelocityZ(const _float fZ) { m_vVelocity.z = fZ; }
	void Set_GroundVelocity(const _float3& v) { m_vVelocity.x = v.x; m_vVelocity.z = v.z; }
	void Add_VelocityX(const _float fX) { m_vVelocity.x += fX; }
	void Add_VelocityY(const _float fY) { m_vVelocity.y += fY; }
	void Add_VelocityZ(const _float fZ) { m_vVelocity.z += fZ; }
	void Add_GroundVelocity(const _float3& v) { m_vVelocity.x += v.x; m_vVelocity.z += v.z; }

	void Set_OnGround();
	void Set_Ground(_bool bGround) { m_bGround = bGround; }
	_bool IsGround() { return m_bGround; }

	void Set_Gravity(const _float3& _vGravity) { m_vGravity = _vGravity; }
	void Set_UseGravity(_bool b) { m_bUseGravity = b; }
	_bool Is_UseGravity() { return m_bUseGravity; }

	void Set_Friction(_float fFriction) { m_fFriction = fFriction; }

	//void Set_Mass(_float fMass) { m_fMass = fMass; }
	//_float Get_Mass() { return m_fMass; }

	//void Add_Force(const _float3& _fForce) { m_vForce = _fForce; }
public:
	static CRigidbody* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

