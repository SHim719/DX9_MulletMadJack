#include "..\Public\Level.h"

#include "GameInstance.h"
#include "GameObject.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{

	return S_OK;
}


void CLevel::PriorityTick(_float fTimeDelta)
{
}

void CLevel::Tick(_float fTimeDelta)
{
}

void CLevel::LateTick(_float fTimeDelta)
{
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
