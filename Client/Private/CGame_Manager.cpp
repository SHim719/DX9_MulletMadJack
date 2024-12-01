#include "CGame_Manager.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "CUi_LoadingBackGround.h"
#include "Animation.h"

#include "Player_Include.h"
#include "Dash_Effect.h"
#include "CTextManager.h"
#include "Light_Manager.h"
#include "Player.h"

#include "Ui_Include.h"

#include "Levels_Header.h"

#include "CUi_LobbyButton.h"
#include "CUi_LobbyLogo.h"


IMPLEMENT_SINGLETON(CGame_Manager)

CGame_Manager::CGame_Manager()
{

}

void CGame_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(pGraphic_Device);

	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);

	m_MainViewPort = { 0, 0, g_iWinSizeX, g_iWinSizeY, 0, 1 };
	m_UiViewPort = { 0, 0, g_iWinSizeX, g_iWinSizeY, 0, 1 };
	m_pGameInstance->Set_UiManager_Winsize(g_iWinSizeX, g_iWinSizeY);
	Ready_Prototype_GameObjects();
	Ready_Prototype_Components();
	Ready_Loading_BackGroundTextureAndUi(); 
	Ready_LobbyUi();
	Ready_Execution_Texture();

	Ready_Active_Execution();

}

void CGame_Manager::Tick(_float fTimeDelta)
{
	m_pGameInstance->Tick_Engine(fTimeDelta);

	if (m_eOldProgress == StageProgress::Level_Change && m_eProgress == StageProgress::Level_Change)
		Level_Changing();

	Change_Check();
	Adjust_ViewPort(fTimeDelta);
	Call_Shop(fTimeDelta);

	Cal_Change_Time(fTimeDelta);
	Cal_StageClear_Time(fTimeDelta);
}

void CGame_Manager::Clear()
{
	m_pGameInstance->Set_Enter(true);
	if (m_eProgress != StageProgress::StageClear)
	{
		m_pGameInstance->Set_Ui_ActiveState(L"CUi_Clear_Time", false);
	}
}

void CGame_Manager::Start()
{
	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), true);
	ShowCursor(FALSE);
	CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_NONE);
	if (m_eToChangeLevel != LEVEL_SANS)
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Floor"));

	Set_StageProgress(StageProgress::OnGoing);
}

void CGame_Manager::Render()
{
	if (m_eProgress == StageProgress::OnGoing)
	{
		m_pGameInstance->Render_Begin();
		m_pGameInstance->Draw();
		m_pGameInstance->Ui_Render();
		Render_Text();
		m_pGameInstance->Render_End();
	}
	else
	{
		m_pGameInstance->Render_Begin();
		m_pGraphic_Device->SetViewport(&m_MainViewPort);
		m_pGameInstance->Draw();
		m_pGameInstance->Ui_Shop_Render();
		m_pGraphic_Device->SetViewport(&m_UiViewPort);
		m_pGameInstance->Ui_Render();
		Render_Text();
		m_pGameInstance->Render_End();
	}

}

void CGame_Manager::Change_Check()
{
	if (m_eOldProgress != m_eProgress)
	{
		switch (m_eProgress)
		{
		case StageProgress::OnGoing:
			break;
		case StageProgress::TutorialMidSpot:
			Clear();
			break;
		case StageProgress::TutorialClear:
			Clear();
			break;
		case StageProgress::StageClear:
			Clear();
			break;
		case StageProgress::Shopping:
			m_pGameInstance->Set_Ui_ShopActiveState(TEXT("CUi_Shop"));
			break;
		case StageProgress::Level_Change:
			if (!m_pFadeInOutUI)
				m_pFadeInOutUI = static_cast<CUI_FadeInOut*>(m_pGameInstance->Get_ActiveBlendUI(L"CUi_FadeInOut"));
			m_pFadeInOutUI->Set_Active(true);
			m_pFadeInOutUI->Set_FadeOut(200.f, CUI_FadeInOut::FADECOLOR::BLACK);
			break;
		case StageProgress::Changing:
			m_fChangeTime = 3.f;
			m_pGameInstance->Set_Enter(false);
			break;
		case StageProgress::Start:
			Start();
			break;
		case StageProgress::End:
			break;
		default:
			break;
		}
		m_eOldProgress = m_eProgress;
	}
}

void CGame_Manager::Reduce_ViewPort(_float fTimeDelta)
{
	if (m_MainViewPort.Width > g_iWinSizeX - 300)
	{
		m_MainViewPort.Width -= DWORD(fTimeDelta * 1500);
		m_MainViewPort.Height -= DWORD(fTimeDelta * 710);
	}
	else
	{
		m_MainViewPort.Width = 956;
		m_MainViewPort.Height = 570;
	}
	
}

void CGame_Manager::Extend_ViewPort(_float fTimeDelta)
{
	if (m_MainViewPort.Width < g_iWinSizeX)
	{
		m_MainViewPort.Width += DWORD(fTimeDelta * 1500);
		m_MainViewPort.Height += DWORD(fTimeDelta * 1200);
	}
	else
	{
		m_MainViewPort.Width = g_iWinSizeX;
		m_MainViewPort.Height = g_iWinSizeY;
	}
}

void CGame_Manager::Adjust_ViewPort(_float fTimeDelta)
{
	if (m_eProgress == StageProgress::StageClear)
	{
		Reduce_ViewPort(fTimeDelta);
	}
	else if (m_eProgress == StageProgress::Changing)
	{
		Extend_ViewPort(fTimeDelta);
	}
}

void CGame_Manager::Call_Shop(_float fTimeDelta)
{
	if (m_eProgress == StageProgress::StageClear)
	{
		m_fShopTime -= fTimeDelta;
		if (m_fShopTime < 0)
		{
			m_eProgress = StageProgress::Shopping;
			m_fShopTime = 0.5f;
		}
	}
}

void CGame_Manager::Cal_Change_Time(_float fTimeDelta)
{
	m_fChangeTime -= fTimeDelta;
	if (m_fChangeTime < 0 && m_eProgress == StageProgress::Changing)
	{
		Set_StageProgress(StageProgress::Level_Change);
		m_fChangeTime = 3.5f;
	}
}

void CGame_Manager::Cal_StageClear_Time(_float fTimeDelta)
{
	if (m_eProgress == StageProgress::OnGoing)
	{
		m_fStageClearTime += fTimeDelta;
	}
	else if (m_eProgress == StageProgress::Start)
	{
		m_fStageClearTime = 0.f;
	}

}

void CGame_Manager::Level_Changing()
{
	if (m_pFadeInOutUI->Get_State() == CUI_FadeInOut::FADEOUT && m_pFadeInOutUI->IsFinished())
	{
		CLevel* pLevel = nullptr;
		CPlayer_Manager::Get_Instance()->Get_Player()->Active_Reset();
		switch (m_eToChangeLevel)
		{
		case LEVEL_GAMEPLAY2:
			pLevel = CLevel_Map2::Create(m_pGraphic_Device);
			break;
		case LEVEL_ELEVATOR:
			pLevel = CElevator_Level::Create(m_pGraphic_Device);
			break;
		case LEVEL_BOSS:
			pLevel = CLevel_Boss::Create(m_pGraphic_Device);
			break;
		}
		m_pGameInstance->Change_Level(pLevel);
		m_pFadeInOutUI->Set_FadeIn(450.f, CUI_FadeInOut::FADECOLOR::BLACK);
		m_pGameInstance->Stop(L"Elevator_FX");
		Start();
		m_fStageClearTime = 0.f;
	}

	if (m_pFadeInOutUI->Get_State() == CUI_FadeInOut::FADEIN && m_pFadeInOutUI->IsFinished())
	{
		m_pFadeInOutUI->Set_Active(false);
	}
}

void CGame_Manager::Player_UpGrade(void* pArg)
{
	
}

void CGame_Manager::Ready_Loading_BackGroundTextureAndUi()
{
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Loading_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Loading/Loading_BackGround%d.png", 4));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Loading_BackGroundBar_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Loading/Loading_BackGround_Bar.png"));

	m_pGameInstance->Add_Ui_LifePrototype(L"CUi_LoadingBackGround"
		, CUi_LoadingBackGround::Create(m_pGraphic_Device));
}

void CGame_Manager::Print_Text(TextType type, _uint Number)
{
	m_pTextManager->Get_Text(type, Number);
	m_pTextManager->Print_Text(type, Number);
}

void CGame_Manager::Print_Text_Sans(CText::Text_Info* Text)
{
	RenderTextVec.emplace_back(Text);
}

void CGame_Manager::Render_Text()
{
	for (auto& iter : RenderTextVec)
		m_pGameInstance->Print_Text(*iter);
}

void CGame_Manager::Clear_Sans_Text()
{
	RenderTextVec.clear();
}

void CGame_Manager::Set_Pos_Text(TextType type, _uint Number, RECT Rect)
{
	m_pTextManager->Set_Pos_Text(type, Number, Rect);
}

void CGame_Manager::Initialize_TextManager()
{
	m_pTextManager = CTextManager::Create();
	if (m_pTextManager == nullptr)
	{
		assert("textmanager create failed");
	}
}

CText::Text_Info* CGame_Manager::Get_Text(TextType type, _uint Number)
{
	return m_pTextManager->Get_Text(type, Number);
}

HRESULT CGame_Manager::Ready_Prototype_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_Prototype(L"Background"
		, CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_Player"
		, CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Prototype_Components()
{
#pragma region Components

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Transform_Default"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box_Collider_Default"),
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For Prototype_Component_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Animation_Default"),
		CAnimation::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rigidbody_Default"),
		CRigidbody::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion
#pragma region Model
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Cube_Default"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_SodaMachine_Default"),
		CVIBuffer_Mesh::Create(m_pGraphic_Device, L"../Bin/Resources/Models/SodaMachine.obj"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Floor_Border_Default"),
		CVIBuffer_Floor_Border::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Door_Default"),
		CVIBuffer_Door::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Bullet_Default"),
		CVIBuffer_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_BoxObject_Default"),
		CVIBuffer_BoxObj::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Elevator_L_Default"),
		CVIBuffer_Elevator_L::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Elevator_R_Default"),
		CVIBuffer_Elevator_R::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_MoveWall_Default"),
		CVIBuffer_MoveWall::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion
	return S_OK;
}

HRESULT CGame_Manager::Ready_LobbyUi()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_LobbyButton_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Lobby/Button%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_LobbyLogo_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Lobby/LOGO%d.png", 12))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Ui_Active(L"CUi_LobbyLogo", eUiRenderType::Render_NonBlend, CUi_LobbyLogo::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"CUi_LobbyButton", eUiRenderType::Render_NonBlend, CUi_LobbyButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Execution_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Neck_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Neck/NECK%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Head_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Head/HEAD%d.png", 17))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Body_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Body/WhiteSuite/BODY%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Hand_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Punch/PUNCH%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Knife_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Weapon/Knife/HAND%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Execution_Body2_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Execution/Body/Chainsaw/BODY%d.png", 3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Active_Execution()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Execution_Neck", eUiRenderType::Render_NonBlend, CExecution_Neck::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Execution_Body", eUiRenderType::Render_NonBlend, CExecution_Body::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Execution_Head", eUiRenderType::Render_NonBlend, CExecution_Head::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Execution_Hand", eUiRenderType::Render_NonBlend, CExecution_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Execution_Knife", eUiRenderType::Render_NonBlend, CExecution_Knife::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

_float2 CGame_Manager::Object_Shake(_float fPower)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1000);
	_float2 ShakeReturn = { 0,0 };
	int i = 0;
	if (0 >= (dis(gen) - 500)) i = -1;
	else i = 1;

	if (dis(gen) % 2 == 0) {
		ShakeReturn.x = fPower * i;
	}
	else {
		ShakeReturn.y = fPower * i;
	}


	return ShakeReturn;
}

void CGame_Manager::Free()
{
	Safe_Release(m_pTextManager);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
