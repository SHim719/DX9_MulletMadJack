#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "CGame_Manager.h"

#include "Core_Camera.h"
#include "PlayerManager.h"
#include "Player.h"

#include "SodaMachine.h"
#include "MapLoader.h"

#include "Trigger_Headers.h"
#include "Monster_Headers.h"
#include "Level_Map2.h"
#include "DialogueManager.h"


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

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test2.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test3.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Sans.dat", (LEVEL)m_iLevelID);
	//CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/TestMh.dat", (LEVEL)m_iLevelID);

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
	
	Ready_First_Monster();
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);

	if (!m_bFirstTrigger)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= m_fStartTriggerDelay)
		{
			m_fTimeAcc = 0.f;
			CDialogue_Manager::Get_Instance()->Start_Dialogue(FirstDialogue, 0.07f, 1.f);
			m_bFirstTrigger = true;
		}
	}
	else
	{
		if (CDialogue_Manager::Get_Instance()->Check_DialogueEnd() && !m_bGo)
		{
			CPlayer_Manager::Get_Instance()->Get_Player()->Change_SuperInvincible();
			m_bGo = true;
		}
			
	}
	
	

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

	pPlayer->Active_Reset();
	pPlayer->Set_Weapon_Render(false);

	pPlayer->Change_SuperInvincible();
	return S_OK;
}

void CLevel_GamePlay::Ready_First_Monster()
{
	m_pFirstMonster = dynamic_cast<CWhite_Suit_Monster*>(m_pGameInstance->Add_Clone
	(m_iLevelID, TEXT("Monster"), TEXT("Prototype_White_Suit")));
	m_pFirstMonster->Get_Transform()->Set_Pos({ 0.f, 0.5f, 8.f });
	static_cast<CBoxCollider*>(m_pFirstMonster->Find_Component(L"Collider"))->Update_BoxCollider(m_pFirstMonster->Get_Transform()->Get_WorldMatrix());
	m_pFirstMonster->Set_TutorialMob(true);
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
