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
}

void CGame_Manager::Tick(_float fTimeDelta)
{
	CLight_Manager::Get_Instance()->Reset_Light();

	m_pGameInstance->Tick_Engine(fTimeDelta);

	Change_Check();
	Adjust_ViewPort(fTimeDelta);
	Call_Shop(fTimeDelta);
	Cal_Change_Time(fTimeDelta);
	Cal_StageClear_Time(fTimeDelta);
}

void CGame_Manager::Clear()
{
	m_pGameInstance->Set_Enter(true);
	if(m_eProgress != StageProgress::StageClear)
	m_pGameInstance->Set_Ui_ActiveState(L"CUi_Clear_Time", false);
}

void CGame_Manager::Start()
{
	m_pGameInstance->Set_Enter(false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Floor"));
}

void CGame_Manager::Render()
{
	if (m_eProgress == StageProgress::OnGoing)
	{
		m_pGameInstance->Render_Begin();
		m_pGameInstance->Draw();
		m_pGameInstance->Ui_Render();
	
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
		case StageProgress::Changing:
			m_fChangeTime = 3.f;
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
	else if (m_eProgress == StageProgress::Start)
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
		Set_StageProgress(StageProgress::Start);
		m_fChangeTime = 3.5f;
	}
	else if (m_fChangeTime < 0 && m_eProgress == StageProgress::Start)
	{
		Set_StageProgress(StageProgress::OnGoing);
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
		m_fStageClearTime = 3.5f;
	}

}

void CGame_Manager::Player_UpGrade(void* pArg)
{
	// for player
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
	m_pTextManager->Print_Text(type, Number);
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
#pragma endregion
	return S_OK;
}

void CGame_Manager::Free()
{
	Safe_Release(m_pTextManager);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
