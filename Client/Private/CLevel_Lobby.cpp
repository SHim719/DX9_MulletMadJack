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

	m_pGameInstance->Set_Ui_ActiveState(L"CUi_LobbyLogo");
	m_pGameInstance->Set_Ui_ActiveState(L"CUi_LobbyButton");

	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{

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

	m_pGameInstance->Set_Ui_ActiveState(L"CUi_LobbyLogo", false);
	m_pGameInstance->Set_Ui_ActiveState(L"CUi_LobbyButton", false);

	__super::Free();
}
