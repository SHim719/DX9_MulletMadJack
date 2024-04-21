#include "CLevel_Lobby.h"
#include "GameInstance.h"
#include "Level_Loading.h"


CLevel_Lobby::CLevel_Lobby(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Lobby::Initialize()
{
	m_iLevelID = LEVEL_LOBBY;

	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT CLevel_Lobby::Render()
{

	return S_OK;
}

CLevel_Lobby* CLevel_Lobby::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Lobby* pInstance = new CLevel_Lobby(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Lobby"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();
}
