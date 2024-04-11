#include "CGame_Manager.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Ui_Include.h"
#include "FPS_Camera.h"
#include "Animation.h"

#include "CUi_SpecialHit_Part.h"
#include "CUi_PEACE.h"
#include "CUi_Fine.h"
#include "CUi_Heart.h"
#include "CUi_Heart_BackGround.h"
#include "CUi_Heart_Line.h"
#include "CUi_Border.h"
#include "CUi_Chat.h"
#include "CUi_LiveStream.h"
#include "CUi_Announcer.h"
#include "CUi_Floor_F.h"
#include "Pistol_Right_Hand.h"
#include "Pistol.h"
#include "CrossHair.h"
#include "Pistol_Shot.h"
#include "Pistol_Spin.h"
#include "Pistol_Reload.h"
#include "Pistol_Gunfire.h"
#include "Pistol_Barrel.h"
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

	Ready_Static_Texture_Prototype();
	Ready_Clear_Texture();
	Ready_Shop_Texture();
	Ready_Start_Texture();

	Ready_Prototype_Ui_Life();
	Ready_Active_Ui();
	Ready_Prototype_Effect();
}

void CGame_Manager::Tick(_float fTimeDelta)
{
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
		case StageProgress::Clear:
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
	if (m_eProgress == StageProgress::Clear)
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
	if (m_eProgress == StageProgress::Clear)
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
		m_fStageClearTime = 0;
	}

}

void CGame_Manager::Player_UpGrade(void* pArg)
{
	// for player
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

#pragma region Model
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Door_Default"),
	//	CVIBuffer_Mesh::Create(m_pGraphic_Device, L"../Bin/Resources/Models/DoorLeft.obj"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_SodaMachine_Default"),
		CVIBuffer_Mesh::Create(m_pGraphic_Device, L"../Bin/Resources/Models/SodaMachine.obj"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Floor_Border_Default"),
		CVIBuffer_Floor_Border::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box_Collider_Default"),
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For Prototype_Component_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Animation"),
		CAnimation::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Prototype_Ui_Life()
{
	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_MonsterDie"),
		CUi_MonsterDie::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_BackGround"),
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

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Floor_Part"),
		CUi_Floor_Part::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Shop_UpGrade"),
		CUi_Shop_UpGrade::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_UpGrade_Select"),
		CUi_UpGrade_Select::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_UpGrade_Focus"),
		CUi_UpGrade_Focus::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Floor_Logo_Num"),
		CUi_Floor_Logo_Num::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Real_ClearTime"),
		CUi_Real_ClearTime::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_ClearTime_BackGround"),
		CUi_ClearTime_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Time_Division"),
		CUi_Time_Division::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Static_Texture_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Background_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterHighGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/3Sec.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Special3Sec_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Special3Sec.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_HEADSHOT_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/HEADSHOT.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_FINISHED_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/FINISHED.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CrossHair_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Crosshair/Crosshair%d.png", 7))))
		return E_FAIL;

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

HRESULT CGame_Manager::Ready_Clear_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Peace_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Green_Peace.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Fine_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Fine.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_LiveStream_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/LiveStream.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_BackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Beat_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_Beat.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Line_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_Line.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Border_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Border.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Chat_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Chat/Chat%d.png", 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Announcer_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Announcer/Announcer.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Sheet_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Sheet/Sheet%d.png", 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Clear_Right_Middle.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Word_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Clear_Floor.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Num_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Clear_Font/Clear_%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Clear_Time_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Victory/FloorTime.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Real_ClearTime_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Victory/Green_%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_ClearTime_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Time_BackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Time_Division_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Time_Division.png"))))
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Idle/PISTOL_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Shot_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Shot/PISTOL_SHOT%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Spin_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Spin/SPIN_AIR%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Reload_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload/PISTOL_RELOAD%d.png", 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Camera_Dash_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Camera/Dash/CircleLines%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Fire_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Gunfire/PISTOL_MUZZLE%d.png", 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Barrel_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload_Barrel/BARREL%d.png", 6))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Prototype_Effect()
{
	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CPistol_Gunfire"),
		CPistol_Gunfire::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Shop_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Shop_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/Clear_Shop%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Shop_UpGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_UpGrade_Select_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade_Select%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_UpGrade_Focus_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade_Focus.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Start_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_F_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_F%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_L_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_L%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_O_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_O%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_R_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_R%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_0_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start%d.png", 6))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Active_Ui()
{
	if (FAILED(Ready_Active_Clear()))
		return E_FAIL;

	if (FAILED(Ready_Active_Shop()))
		return E_FAIL;

	if (FAILED(Ready_Active_Gun()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Active_Clear()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Peace"),
		eUiRenderType::Render_NonBlend,
		CUi_PEACE::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Fine"),
		eUiRenderType::Render_NonBlend,
		CUi_Fine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart_BackGround"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart_Line"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart_Line::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Border"),
		eUiRenderType::Render_NonBlend,
		CUi_Border::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Chat"),
		eUiRenderType::Render_NonBlend,
		CUi_Chat::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_LiveStream"),
		eUiRenderType::Render_NonBlend,
		CUi_LiveStream::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Announcer"),
		eUiRenderType::Render_NonBlend,
		CUi_Announcer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor_Logo"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor_Logo_Word"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor_Logo_Word::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Clear_Time"),
		eUiRenderType::Render_NonBlend,
		CUi_Clear_Time::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGame_Manager::Ready_Active_Shop()
{
	if (FAILED(m_pGameInstance->Add_Ui_ShopActive(TEXT("CUi_Shop"),
		CUi_Shop::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}


HRESULT CGame_Manager::Ready_Active_Gun()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_CrossHair", eUiRenderType::Render_NonBlend, CCrossHair::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Right_Hand", eUiRenderType::Render_NonBlend, CPistol_Right_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol", eUiRenderType::Render_NonBlend, CPistol::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Shot", eUiRenderType::Render_NonBlend, CPistol_Shot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Spin", eUiRenderType::Render_NonBlend, CPistol_Spin::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload", eUiRenderType::Render_NonBlend, CPistol_Reload::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Barrel", eUiRenderType::Render_NonBlend, CPistol_Barrel::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

void CGame_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
