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

	_float3		m_vVelocity;
	_float3		m_vGravity = { 0.f, -4.f, 0.f };

	_bool		m_bGround = { false };


	//_float		m_fMass;
	//_float3		m_vAccel;
	//_float3		m_vForce;

	_float		m_fFriction;

public:
	void Set_Velocity(const _float3& fVelocity) 
	{
		m_vVelocity = fVelocity;
	}
	const _float3& Get_Velocity() const { return m_vVelocity; }

	void Set_VelocityX(const _float fX) { m_vVelocity.x = fX; }
	void Set_VelocityY(const _float fY) { m_vVelocity.y = fY; }
	void Set_VelocityZ(const _float fZ) { m_vVelocity.z = fZ; }

	void Set_OnGround();

	void Set_Gravity(const _float3& _vGravity) { m_vGravity = _vGravity; }

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

