#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "CGame_Manager.h"
#include "PlayerManager.h"
#include "Light_Manager.h"
#include "MathManager.h"
#include "DialogueManager.h"
#include "MapLoader.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}


HRESULT CMainApp::Initialize()
{
	GRAPHIC_DESC		GraphicDesc{};
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;
	GraphicDesc.isWindowed = true;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	CGame_Manager::Get_Instance()->Initialize(m_pGraphic_Device);
	CPlayer_Manager::Get_Instance()->Initialize(m_pGraphic_Device);
	CMath_Manager::Get_Instance()->Initialize(m_pGraphic_Device);
	CDialogue_Manager::Get_Instance()->Initialize();
	ShowCursor(FALSE);


	/*if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;*/

	if (FAILED(Open_Level(LEVEL_BOSS)))
		return E_FAIL;

	//m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM.mp3", L"TestBgm");
	//m_pGameInstance->Play(L"TestBgm", true);
	// 적은 볼륨작게(0.1~0.5f) 플레이어는 상대적으로 크게(0.6~1.0), 

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	CGame_Manager::Get_Instance()->Tick(fTimeDelta);	
	CPlayer_Manager::Get_Instance()->Tick(fTimeDelta);
	CMath_Manager::Get_Instance()->Tick(fTimeDelta);
	CDialogue_Manager::Get_Instance()->Tick(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	CGame_Manager::Get_Instance()->Render();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		//assert(false);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	__super::Free();

	CMapLoader::Get_Instance()->Free();
	CGame_Manager::Get_Instance()->Free();
	CPlayer_Manager::Get_Instance()->Free();
	CLight_Manager::Get_Instance()->Free();
	CMath_Manager::Get_Instance()->Free();
	CDialogue_Manager::Get_Instance()->Free();
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);	

	CGameInstance::Release_Engine();

}

