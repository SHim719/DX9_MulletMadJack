#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "CGame_Manager.h"

#include "BackGround.h"
#include "CUi_MonsterDie.h"
#include "CUi_Background.h"
#include "CUi_Special3Sec.h"
#include "CUi_SpecialHit.h"
#include "FPS_Camera.h"
#include "Animation.h"
#include "CUi_SpecialHit_Part.h"
#include "CrossHair.h"
#include "Pistol_Right_Hand.h"
#include "Pistol.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
	, m_UiViewPort{}
	, m_MainViewPort{}
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

	if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	CGame_Manager::Get_Instance()->Tick(fTimeDelta);	
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_RectX_Default"),
		CVIBuffer_RectX::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_RectXY_Default"),
		CVIBuffer_RectXY::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Box_Default"),
		CVIBuffer_Box::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box_Collider_Default"),
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For Prototype_Component_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Animation"),
		CAnimation::Create(m_pGraphic_Device))))
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

HRESULT CMainApp::Ready_Static_Texture_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC,	L"CUi_Background_Texture",
		CTexture::Create(m_pGraphic_Device,	CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Ui_Life_Background.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterLowGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/1Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterMiddleGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/2Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterSpecialGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/3Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Special3Sec_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Special3Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Ui_Life_Background2.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Active_Ui_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CrossHair_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Crosshair/Crosshair%d.png", 7))))
		return E_FAIL;
	
	//Pistol
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Hand_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Hand/Pistol/HAND_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/PISTOL_IDLE%d.png", 3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Active_Ui()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_CrossHair", eUiRenderType::Render_NonBlend, CCrossHair::Create(m_pGraphic_Device))))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Right_Hand", eUiRenderType::Render_NonBlend, CPistol_Right_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol", eUiRenderType::Render_NonBlend, CPistol::Create(m_pGraphic_Device))))
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

	CGame_Manager::Get_Instance()->Free();
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);	


	CGameInstance::Release_Engine();

}

