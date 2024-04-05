#include "Collision_Manager.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"


CCollision_Manager::CCollision_Manager()
	: m_pGameInstance { CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCollision_Manager::Initialize()
{
	return S_OK;
}

void CCollision_Manager::Tick()
{
	Intersect_Ray();
}

void CCollision_Manager::Intersect_Ray()
{
	for (auto& RayDesc : m_RayDescs)
	{
		CLayer* pLayer = m_pGameInstance->Find_Layer(RayDesc.iLevel, RayDesc.strDstLayer);
		if (pLayer == nullptr)
			continue;

		auto& gameObjects = pLayer->Get_GameObjects();

		Safe_AddRef(pLayer);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>((*it)->Find_Component(L"VIBuffer"));
			if (nullptr == pVIBuffer)
				continue;

			_float3 fHitWorldPos;
			_float fDist;
			if (pVIBuffer->Intersect_Ray((*it)->Get_Transform(), RayDesc.vRayWorldPos, RayDesc.vRayDir, &fHitWorldPos, &fDist))
				(*it)->On_Ray_Intersect(fHitWorldPos, fDist, RayDesc.pArg);
		}

		Safe_Release(pLayer);
	}

	m_RayDescs.clear();
}

CCollision_Manager* CCollision_Manager::Create()
{
	CCollision_Manager* pInstance = new CCollision_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollision_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
