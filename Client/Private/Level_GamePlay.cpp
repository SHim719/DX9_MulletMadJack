#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"
#include "CUi_PEACE.h"
#include "Level_Loading.h"
#include "CGame_Manager.h"
#include "White_Suit_Monster.h"
#include "Drone_Monster.h"
#include "Chainsaw_Monster.h"
#include "Orange_Pants_Monster.h"
#include "Enemy_Bullet.h"

#include "Wall.h"
#include "CrossHair.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"
#include "PlayerManager.h"
#include "Player.h"

#include "SodaMachine.h"
#include "MapLoader.h"

#include "CUi_Combo.h"
#include "CSans.h"
#include "CSans_Gaster.h"

#include "Trigger_Headers.h"
#include "Level_Map2.h"

#include "CSkyBox.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;

	m_pGameInstance->Stop(L"Loading");
	m_pGameInstance->Play(L"Gameplay", true);
	m_pGameInstance->SetVolume(L"Gameplay", 0.5f);

	CGame_Manager::Get_Instance()->Set_StageProgress(StageProgress::OnGoing);

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Sans_Boss(L"Layer_Sans")))
	//	return E_FAIL;

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test2.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test3.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Sans.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/TestMh.dat", (LEVEL)m_iLevelID);

	//if (FAILED(Ready_Layer_Sans_Boss(L"Layer_Sans")))
		//return E_FAIL;

	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test2.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Sans.dat", (LEVEL)m_iLevelID);
	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);

	Initialize_SodaMachine();

	D3DLIGHT9 lightDesc{};
	lightDesc.Type = D3DLIGHT_DIRECTIONAL;
	lightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightDesc.Ambient = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
	lightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(m_pGraphic_Device->SetLight(9, &lightDesc)))
		return E_FAIL;

	D3DMATERIAL9		MaterialDesc{};
	MaterialDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MaterialDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);

	m_pGraphic_Device->SetMaterial(&MaterialDesc);

	m_pGraphic_Device->LightEnable(9, TRUE);

	static_cast<CStageEndTrigger*>(m_pGameInstance->Find_GameObject(m_iLevelID, L"Trigger", 0))->Set_NextLevel
	(LEVEL_GAMEPLAY2);
	
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	Test_Ui();
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);

}

HRESULT CLevel_GamePlay::Render()
{
	return S_OK;
}

void CLevel_GamePlay::Initialize_SodaMachine()
{
	CLayer* pMachineLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine");
	CLayer* pBannerLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine_Banner");

	if (!pMachineLayer || !pBannerLayer)
		return;
	auto MachineLayerObjects = pMachineLayer->Get_GameObjects();
	auto BannerLayerObjects = pBannerLayer->Get_GameObjects();
	
	auto BannerIt = BannerLayerObjects.begin();
	for (auto it = MachineLayerObjects.begin(); it != MachineLayerObjects.end(); ++it)
	{
		dynamic_cast<CSodaMachine*>(*it)->Set_Banner(*BannerIt);

		++BannerIt;
	}

}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CCoreCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 0.65f, 1.7f);
	CameraDesc.vAt = _float3(0.5f, 0.5f, 10.f);
	CameraDesc.fFovy = D3DXToRadian(90.0f);
	CameraDesc.fNear = 0.001f;
	CameraDesc.fFar = 100.0f;
	CameraDesc.fSpeedPerSec = 6.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Create_Camera(strLayerTag, CFPS_Camera::Create(m_pGraphic_Device, &CameraDesc))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Player"), TEXT("Prototype_Player")));
	CPlayer_Manager::Get_Instance()->Set_Player(pPlayer);

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_SkyBox(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Beholder(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_Beholder")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Sans_Boss(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, 
		TEXT("Prototype_Sans")))
		return E_FAIL;

	return S_OK;
}


void CLevel_GamePlay::Free()
{
	__super::Free();
}


void CLevel_GamePlay::Test_Ui()
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::H))
	{
		CUi_Combo::ComboDesc test;
		test.bFirstCall = true;
		test.iKillCount = CPlayer_Manager::Get_Instance()->Get_Combo();
		m_pGameInstance->Add_Ui_LifeClone(L"CUi_Combo", eUiRenderType::Render_NonBlend, &test);
		//CUi_Combo::ComboDesc test;
		//test.bFirstCall = false;
		//test.iKillCount = 3;
		//m_pGameInstance->Add_Ui_LifeClone(L"CUi_Combo", eUiRenderType::Render_NonBlend, &test);
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::U))
	{
	/*	CGame_Manager::Get_Instance()->Set_StageProgress(StageProgress::StageClear);
		CPlayer_Manager::Get_Instance()->Set_MouseLock(false);
		ShowCursor(TRUE);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), false);*/
		CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_CUTIN_SHOP);
	}

	else if (m_pGameInstance->GetKeyDown(eKeyCode::I))
	{
		CUi_SpecialHit::SpecialHit_Desc Arg;
		Arg.Hit = eSpecialHit::HEADSHOT;
		Arg.iCount = 4;
		m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"),
			eUiRenderType::Render_NonBlend, &Arg);
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::K))
	{
		m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_Special3Sec"),
			eUiRenderType::Render_Blend, nullptr);
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::L))
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Finish"));
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::G))
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"));
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::Z))
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_GreenCrossActive"));
	}

}

//else if (m_pGameInstance->GetKeyDown(eKeyCode::Y))
//{
//	_uint qewr = CGame_Manager::Get_Instance()->Get_TextNumber(TextType::TutorialClear);
//	CGame_Manager::Get_Instance()->Print_Text(TextType::TutorialClear,
//		qewr);
//	CGame_Manager::Get_Instance()->Add_TextNumber(TextType::TutorialClear);
//}