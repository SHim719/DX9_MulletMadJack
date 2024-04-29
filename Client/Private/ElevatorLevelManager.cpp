#include "ElevatorLevelManager.h"
#include "Monster_Headers.h"
#include "DialogueManager.h"
#include "Levels_Header.h"
#include "CGame_Manager.h"
#include "PlayerManager.h"
#include "MathManager.h"

#include "GameInstance.h"
#include "Ui_Include.h"

IMPLEMENT_SINGLETON(CElevatorLevelManager)

CElevatorLevelManager::CElevatorLevelManager()
{
}

void CElevatorLevelManager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
 	if (Sans != m_eState)
		return;

	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);

	m_vSpawnPos[0] = { -4.f, 3.0f, 4.f };
	m_vSpawnPos[1] = { 0.f,  3.0f, 4.f };
	m_vSpawnPos[2] = { 4.f,  3.0f, 4.f };
	m_vSpawnPos[3] = {  4.f, 3.0f, 4.f };
	m_vSpawnPos[4] = {  4.f, 3.0f, 0.f };
	m_vSpawnPos[5] = {  4.f, 3.0f, -4.f };
	m_vSpawnPos[6] = {  4.f, 3.0f, -4.f };
	m_vSpawnPos[7] = {  0.f, 3.0f, -4.f };
	m_vSpawnPos[8] = { -4.f, 3.0f, -4.f };
	m_vSpawnPos[9] = { -4.f, 3.0f, -4.f };
	m_vSpawnPos[10] = { -4.f, 3.0f,  0.f };
	m_vSpawnPos[11] = { -4.f, 3.0f, 4.f };

	m_eState = EndState;
	m_fEventDelayTime = 2.f;
}

void CElevatorLevelManager::Tick(_float fDeltaTime)
{
	switch (m_eState)
	{
	case LEVEL_STATE::Sans:
		State_Sans(fDeltaTime);
		break;
	case LEVEL_STATE::Return:
		State_Return(fDeltaTime);
		break;
	case LEVEL_STATE::OnGoing:
		State_On_Going(fDeltaTime);
		break;
	case LEVEL_STATE::EndState:
		State_End_State(fDeltaTime);
		break;
	}
}

void CElevatorLevelManager::State_Sans(_float fDeltaTime)
{
	if (!m_bAnotherBranch)
	{
		m_fEventDelayTimeAcc += fDeltaTime;
		if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
		{
			CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::ElevatorSansDialogue, 0.05f, 0.8f);
			m_bAnotherBranch = true;
			m_fEventDelayTime = 2.f;
			m_fEventDelayTimeAcc = 0.f;
		}
	}
	else
	{
		if (CDialogue_Manager::Get_Instance()->Check_DialogueEnd())
		{
			CPlayer_Manager::Get_Instance()->Get_Player()->Set_Active(false);
			CPlayer_Manager::Get_Instance()->Get_Player()->Active_Reset();
			CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Ui_Phone"), false);
			m_pGameInstance->Set_Ui_ActiveState(L"Noise_Filter");
			m_fEventDelayTimeAcc += fDeltaTime;
			if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
			{
				m_pGameInstance->Stop(L"Elevator_Level_SFX");
				m_pGameInstance->Set_Ui_ActiveState(L"Noise_Filter", false);
				CLevel* pSansLevel = CSansLevel::Create(m_pGraphic_Device);
				m_pGameInstance->Change_Level(pSansLevel);
				
				m_eState = Return;
				m_bAnotherBranch = false;
				m_fEventDelayTimeAcc = 0.f;
				m_fEventDelayTime = 3.f;
			}
		}
	}
}


void CElevatorLevelManager::State_Return(_float fDeltaTime)
{
	if (m_bAnotherBranch)
	{
		if (CDialogue_Manager::Get_Instance()->Check_DialogueEnd())
			Warning();
	}
	else
	{
		m_fEventDelayTimeAcc += fDeltaTime;
		if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
		{
			m_fEventDelayTimeAcc = 0.f;
			CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::ElevatorReturnDialogue, 0.1f, 1.f);
			m_bAnotherBranch = true;
		}
	}


}

void CElevatorLevelManager::Warning()
{
	static _bool bEndEvent = false;
	static _bool bWaitingFade = false;
	static _uint iWarningCount = 2;
	if (bEndEvent)
		return;

	if (nullptr == m_pFadeInOutUI)
	{
		m_pFadeInOutUI = static_cast<CUI_FadeInOut*>(m_pGameInstance->Get_ActiveBlendUI(L"CUi_FadeInOut"));
		m_pGameInstance->Play(L"Warning", false);
	}
		

	if (!bWaitingFade)
	{
		if (0 == iWarningCount)
		{
			bEndEvent = true;
			m_pGameInstance->Play(L"Elevator_BGM", true);
			m_pGameInstance->SetVolume(L"Elevator_BGM", 0.5f);
			CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::ElevatoInvadeDialogue);
			m_bAnotherBranch = false;
			CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer::KATANA);
			m_fEventDelayTimeAcc = 0.f;
			m_fEventDelayTime = 5.f;//30.f;//80.f;
			m_eState = OnGoing;
			return;
		}
			
		iWarningCount -= 1;
		m_pFadeInOutUI->Set_Active(true);
		m_pFadeInOutUI->Set_FadeOut(300.f, CUI_FadeInOut::FADECOLOR::RED, 125.f);
		bWaitingFade = true;
	}
	else if (m_pFadeInOutUI->Get_State() == CUI_FadeInOut::FADEOUT && m_pFadeInOutUI->IsFinished())
	{
		m_pFadeInOutUI->Set_FadeIn(300.f, CUI_FadeInOut::FADECOLOR::RED);
		bWaitingFade = false;
	}
		
}

void CElevatorLevelManager::State_On_Going(_float fDeltaTime)
{
	if (!m_bAnotherBranch)
	{
		m_fEventDelayTimeAcc += fDeltaTime;
		if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
		{
			m_fEventDelayTimeAcc = 0.f;
			m_fSpawnTimeAcc = 0.f;
			m_fSpawnTime = 0.8f;
			m_bAnotherBranch = true;
			m_fEventDelayTime = 5.f;
		}  


		m_fSpawnTimeAcc += fDeltaTime;

		if (m_fSpawnTimeAcc >= m_fSpawnTime)
		{
			for (_uint i = 0; i < 3; ++i)
			{
				Spawn_Monsters();
			}
			m_fSpawnTimeAcc = 0.f;
		}
	}
	else
	{
		static _bool bDial = false;
		m_fEventDelayTimeAcc += fDeltaTime;
		if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
		{
			if (!bDial)
			{
				CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::ElevatoManyEnemyDialogue, 0.07f, 1.0f);
				bDial = true;
			}
			
			if (CDialogue_Manager::Get_Instance()->Check_DialogueEnd() && !m_bUltimate)
			{
				CPlayer_Manager::Get_Instance()->Get_Player()->SetState_Ultimate();
				m_bUltimate = true;
			}

			if (m_bUltimate&& CPlayer_Manager::Get_Instance()->Get_Player_State() != CPlayer::ULTIMATE_STATE)
			{
				m_fEventDelayTimeAcc = 0.f;
				m_fEventDelayTime = 8.f;
				m_eState = EndState;
				m_bAnotherBranch = false;
			}
			return;
		}

		m_fSpawnTimeAcc += fDeltaTime;

		if (m_fSpawnTimeAcc >= m_fSpawnTime)
		{
			for (_uint i = 0; i < 8; ++i)
				Spawn_Monsters();
			
			m_fSpawnTimeAcc = 0.f;
		}
	}
}

void CElevatorLevelManager::State_End_State(_float fDeltaTime)
{
	if (!m_bAnotherBranch)
	{
		m_fEventDelayTimeAcc += fDeltaTime;
		if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
		{
			CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::ElevatorEndDialogue);
			m_fEventDelayTimeAcc = 0.f;
			m_bAnotherBranch = true;
			m_fEventDelayTime = 3.f;
		}
	}
	else
	{
		if (CDialogue_Manager::Get_Instance()->Check_DialogueEnd())
		{
			m_fEventDelayTimeAcc += fDeltaTime;
			if (m_fEventDelayTimeAcc >= m_fEventDelayTime)
			{
				m_pGameInstance->Stop(L"Elevator_BGM");
				m_pGameInstance->Stop(L"Elevator_Level_SFX");

				m_pGameInstance->Play(L"Stage_End", false);
				m_pGameInstance->SetVolume(L"Stage_End", 0.5f);
				m_pGameInstance->Play(L"Congratulations", false);
				m_pGameInstance->SetVolume(L"Congratulations", 0.5f);
				m_pGameInstance->Play(L"Elevator_FX", true);
				m_pGameInstance->SetVolume(L"Elevator_FX", 1.f);
				CGame_Manager::Get_Instance()->Set_Change_Level(LEVEL_BOSS);
				CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_CUTIN_SHOP);
				m_eState = LS_END;
			}
		}
	}
}

void CElevatorLevelManager::Spawn_Monsters()
{
	_int iRandMonsterIndex = CMath_Manager::Get_Instance()->Random_Int(0, MONSTERTYPE::DRONE);
	_int iRandSpawnPosIndex = CMath_Manager::Get_Instance()->Random_Int(0, SPAWNPOS_COUNT);

	wstring strPrototypeTag = L"";
	switch ((MONSTERTYPE)iRandMonsterIndex)
	{
	case WHITE_SUIT:
		strPrototypeTag = L"Prototype_White_Suit";
		break;
	case CHAINSAW:
		strPrototypeTag = L"Prototype_Chainsaw";
		break;
	case DRONE:
		strPrototypeTag = L"Prototype_Drone";
		break;
	default:
		break;
	}

	CPawn* pPawn = static_cast<CPawn*>(m_pGameInstance->Add_Clone(LEVEL_ELEVATOR, L"Monster", strPrototypeTag));
	pPawn->SetState_Air();
	pPawn->Get_Transform()->Set_Pos(m_vSpawnPos[iRandSpawnPosIndex]);
	static_cast<CBoxCollider*>(pPawn->Find_Component(L"Collider"))->Update_BoxCollider(pPawn->Get_Transform()->Get_WorldMatrix());
}

void CElevatorLevelManager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
