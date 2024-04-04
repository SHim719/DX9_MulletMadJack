#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "CUi_MonsterDie.h"
#include "CUi_Background.h"
#include "CUi_Special3Sec.h"
#include "CUi_SpecialHit.h"
#include "FPS_Camera.h"
#include "CUi_SpecialHit_Part.h"


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

	m_pGameInstance->Set_UiManager_Winsize(g_iWinSizeX, g_iWinSizeY);

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObjects()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Ui_Life()))
		return E_FAIL;

	if(FAILED(Ready_Prototype_Camera()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_Prototype(L"Background"
		, CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK; 
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Transform_Default"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Ui_Life()
{
	if(FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_MonsterDie"),
		CUi_MonsterDie::Create(m_pGraphic_Device))))
		return E_FAIL;


	if(FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_BackGround"),
		CUi_Background::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Special3Sec"),
		CUi_Special3Sec::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_SpecialHit"),
		CUi_SpecialHit::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_SpecialHit_Part"),
		CUi_SpecialHit_Part::Create(m_pGraphic_Device))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Camera()
{
	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CFPS_Camera::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);	


	CGameInstance::Release_Engine();

}

