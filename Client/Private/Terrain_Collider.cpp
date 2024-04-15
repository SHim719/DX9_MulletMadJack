#include "Terrain_Collider.h"

#include "GameInstance.h"
#include "GameObject.h"

CTerrain_Collider::CTerrain_Collider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
}

CTerrain_Collider::CTerrain_Collider(const CTerrain_Collider& rhs)
	: CComponent{ rhs }
	, m_pGameInstance { rhs.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CTerrain_Collider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_Collider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_pOwner = (CGameObject*)pArg;
	Safe_AddRef(m_pOwner);

	return S_OK;
}

void CTerrain_Collider::Update()
{
	Check_Look();
	Check_Up();
	Check_Right();
}


void CTerrain_Collider::Check_Look()
{
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Find_Component(L"Transform"));

	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = pTransform->Get_State(CTransform::STATE_LOOK);
	rayDesc.vRayWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	rayDesc.vRayDir.y = 0.f;
	D3DXVec3Normalize(&rayDesc.vRayDir, &rayDesc.vRayDir);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
			return;
		}
	}

	rayDesc.vRayDir = -rayDesc.vRayDir;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CTerrain_Collider::Check_Up()
{
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Find_Component(L"Transform"));

	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Floor";
	rayDesc.vRayDir = { 0.f, -1.f, 0.f };
	rayDesc.vRayWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	CRigidbody* pRigidbody = dynamic_cast<CRigidbody*>(m_pOwner->Find_Component(L"Rigidbody"));
	if (nullptr == pRigidbody)
		return;
	_bool IsGround = dynamic_cast<CRigidbody*>(m_pOwner->Find_Component(L"Rigidbody"))->IsGround();

	if (!IsGround && m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.75f)
		{
			_float3 vPos = fHitWorldPos;
			vPos.y += 0.75f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
			pRigidbody->Set_OnGround();
			return;
		}
	}

	rayDesc.vRayDir = { 0.f, 1.f, 0.f };
	rayDesc.vRayWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.75f)
		{
			_float3 vPos = fHitWorldPos;
			vPos.y -= 0.75f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CTerrain_Collider::Check_Right()
{
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Find_Component(L"Transform"));

	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = pTransform->Get_State(CTransform::STATE_RIGHT);
	rayDesc.vRayWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	rayDesc.vRayDir.y = 0.f;
	rayDesc.vRayDir = *D3DXVec3Normalize(&rayDesc.vRayDir, &rayDesc.vRayDir);


	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
			return;
		}
	}

	rayDesc.vRayDir = -rayDesc.vRayDir;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

CTerrain_Collider* CTerrain_Collider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain_Collider* pInstance = new CTerrain_Collider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain_Collider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CTerrain_Collider::Clone(void* pArg)
{
	CTerrain_Collider* pInstance = new CTerrain_Collider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrain_Collider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Collider::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pOwner);
}
