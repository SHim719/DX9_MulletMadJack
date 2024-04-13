#include "Rigidbody.h"
#include "Transform.h"

CRigidbody::CRigidbody(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CRigidbody::CRigidbody(const CRigidbody& rhs)
	: CComponent{ rhs }
{
}

HRESULT CRigidbody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidbody::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	
	m_pOwnerTransform = (CTransform*)pArg;

	Safe_AddRef(m_pOwnerTransform);

	return S_OK;
}

void CRigidbody::Update(_float fTimeDelta)
{
	if (false == m_bGround)
		m_vVelocity += m_vGravity * fTimeDelta;

	
	_float3 vInvVelocity = m_vVelocity;
	vInvVelocity.y = 0.f;
	D3DXVec3Normalize(&vInvVelocity, &vInvVelocity);
	vInvVelocity *= m_fFriction * fTimeDelta;

	_float3 vXZVelocity = m_vVelocity;
	vXZVelocity.y = 0.f;

	if (D3DXVec3Length(&vXZVelocity) < D3DXVec3Length(&vInvVelocity))
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
	}
	else
	{
		m_vVelocity -= vInvVelocity;
	}

	_float3 vOwnerPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	vOwnerPos += m_vVelocity * fTimeDelta;
	m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, &vOwnerPos);
}


void CRigidbody::Set_OnGround()
{
	m_bGround = true;
	m_vVelocity.y = 0.f;
}

CRigidbody* CRigidbody::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRigidbody* pInstance = new CRigidbody(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRigidbody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CRigidbody::Clone(void* pArg)
{
	CRigidbody* pInstance = new CRigidbody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRigidbody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidbody::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerTransform);
}
