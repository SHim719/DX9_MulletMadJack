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
#include "Enemy_Bullet.h"

#include "Wall.h"
#include "CrossHair.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"
#include "PlayerManager.h"
#include "Player.h"

#include "SodaMachine.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;

	Load_MapObject(L"../Bin/Resources/DataFiles/Test.dat", OBJTYPE_END);

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Pawns()))
	//	return E_FAIL;

	Initialize_SodaMachine();
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	Test_Ui();
}

HRESULT CLevel_GamePlay::Render()
{

	SetWindowText(g_hWnd, TEXT("GAMEPLAY.."));

	return S_OK;
}

void CLevel_GamePlay::Initialize_SodaMachine()
{
	CLayer* pMachineLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine");
	CLayer* pBannerLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine_Banner");

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
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.0f;
	CameraDesc.fSpeedPerSec = 6.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Create_Camera(strLayerTag, CFPS_Camera::Create(m_pGraphic_Device, &CameraDesc))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	CPlayer_Manager::Get_Instance()->Set_Player(dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Player"), TEXT("Prototype_Player"))));

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Pawns()
{
	if (FAILED(Ready_Layer_White_Suit_Monster(TEXT("Layer_White_Suit_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Drone_Monster(TEXT("Layer_Drone_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Chainsaw_Monster(TEXT("Layer_Chainsaw_Monster"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_White_Suit_Monster(const wstring& strLayerTag)
{
	CWhite_Suit_Monster::PAWN_DESC	White_Suit_MonsterDesc{};

	White_Suit_MonsterDesc.vPosition = _float3(0.f, 0.45f, 20.f);
	White_Suit_MonsterDesc.vAt = _float3(0.f, 0.f, 0.f);
	White_Suit_MonsterDesc.fSpeedPerSec = 0.3f;
	White_Suit_MonsterDesc.fRotationPerSec = D3DXToRadian(90.f);
	
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_White_Suit_Monster"), &White_Suit_MonsterDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Drone_Monster(const wstring& strLayerTag)
{
	CDrone_Monster::PAWN_DESC	Drone_MonsterDesc{};

	Drone_MonsterDesc.vPosition = _float3(1.f, 1.f, 20.f);
	Drone_MonsterDesc.vAt = _float3(0.f, 0.f, 0.f);
	Drone_MonsterDesc.fSpeedPerSec = 0.3f;
	Drone_MonsterDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Drone_Monster"), &Drone_MonsterDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Chainsaw_Monster(const wstring& strLayerTag)
{
	CChainsaw_Monster::PAWN_DESC	Chainsaw_MonsterDesc{};

	Chainsaw_MonsterDesc.vPosition = _float3(-1.f, 0.45f, 20.f);
	Chainsaw_MonsterDesc.vAt = _float3(0.f, 0.f, 0.f);
	Chainsaw_MonsterDesc.fSpeedPerSec = 0.3f;
	Chainsaw_MonsterDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Chainsaw_Monster"), &Chainsaw_MonsterDesc))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Free()
{

	__super::Free();

}

void CLevel_GamePlay::Test_Ui()
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::U))
	{
		CGame_Manager::Get_Instance()->Set_StageProgress(CGame_Manager::StageProgress::Clear);
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::J))
	{
		CUi_MonsterDie::MonsterDie_Arg Arg;
		Arg.MonsterDiePosX = -200;
		Arg.MonsterDiePosY = -200;
		Arg.MonsterGrade = eMonsterGrade::High;
		m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_MonsterDie"),
			eUiRenderType::Render_Blend, &Arg);
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
}
