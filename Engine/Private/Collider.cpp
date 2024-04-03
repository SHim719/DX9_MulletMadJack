#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent { rhs }
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return S_OK;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pOwner);
}
