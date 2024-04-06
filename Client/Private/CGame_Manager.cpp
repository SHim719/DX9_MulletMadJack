#include "CGame_Manager.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "CUi_MonsterDie.h"
#include "CUi_Background.h"
#include "CUi_Special3Sec.h"
#include "CUi_SpecialHit.h"
#include "FPS_Camera.h"
#include "Animation.h"
#include "CUi_SpecialHit_Part.h"
#include "CUi_PEACE.h"
#include "Machine_Gun.h"
#include "CUi_Fine.h"
#include "CUi_Heart.h"
#include "CUi_Heart_BackGround.h"
#include "CUi_Heart_Line.h"
#include "CUi_Border.h"


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
	Ready_Prototype_Ui_Life();
	Ready_Static_Texture_Prototype();
	Ready_Active_Ui();
}

void CGame_Manager::Tick(_float fTimeDelta)
{
	m_pGameInstance->Tick_Engine(fTimeDelta);

	Change_Check();

	if (m_eOldProgress == StageProgress::Clear)
	{
		Reduce_ViewPort(fTimeDelta);
	}
	else if (m_eOldProgress == StageProgress::Start)
	{
		Extend_ViewPort(fTimeDelta);
	}

}

void CGame_Manager::Clear()
{
	m_pGameInstance->Set_Enter(true);
}

void CGame_Manager::Start()
{
	m_pGameInstance->Set_Enter(false);
}

void CGame_Manager::Render()
{
	if (m_eProgress == StageProgress::OnGoing)
	{
		m_pGameInstance->Render_Begin();
		m_pGameInstance->Draw();
		m_pGameInstance->UiRender();
		m_pGameInstance->Render_End();
	}
	else
	{
		m_pGameInstance->Render_Begin();
		m_pGraphic_Device->SetViewport(&m_MainViewPort);
		m_pGameInstance->Draw();
		m_pGraphic_Device->SetViewport(&m_UiViewPort);
		m_pGameInstance->UiRender();
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
		m_MainViewPort.Height -= DWORD(fTimeDelta * 750);
	}
	else
	{
		m_MainViewPort.Width = g_iWinSizeX - 300;
		m_MainViewPort.Height = g_iWinSizeY - 150;
	}
}

void CGame_Manager::Extend_ViewPort(_float fTimeDelta)
{
	if (m_MainViewPort.Width < g_iWinSizeX)
	{
		m_MainViewPort.Width += DWORD(fTimeDelta * 1500);
		m_MainViewPort.Height += DWORD(fTimeDelta * 750);
	}
	else
	{
		m_MainViewPort.Width = g_iWinSizeX;
		m_MainViewPort.Height = g_iWinSizeY;
		Set_StageProgress(StageProgress::OnGoing);
	}
}

HRESULT CGame_Manager::Ready_Prototype_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_Prototype(L"Background"
		, CBackGround::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterSpecialGrade_Texture",
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CrossHair_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Crosshair/Crosshair%d.png", 7))))
		return E_FAIL;

	
	return S_OK;
}


HRESULT CGame_Manager::Ready_Active_Ui()
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

	return S_OK;
}

void CGame_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
