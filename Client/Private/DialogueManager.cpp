#include "DialogueManager.h"
#include "UI_Dialogue.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CDialogue_Manager)


CDialogue_Manager::CDialogue_Manager()
{
}

void CDialogue_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
	Init_FirstDialogue();
}

void CDialogue_Manager::Tick(_float fTimeDelta)
{
	switch (m_eDialogueEvent)
	{
	case No_Dial:
		break;
	case FirstDialogue:
		Tick_FirstDialogue();
		break;
	case ElevatorSansDialogue:
		Tick_ElevatorSansDialogue();
		break;
	case ElevatorReturnDialogue:
		Tick_ElevatorReturnDialogue();
		break;
	case BossEntryDialogue:
		Tick_BossEntryDialogue();
		break;
	}

}

void CDialogue_Manager::Start_Dialogue(DialogueEvent eDialEvent)
{
	m_eDialogueEvent = eDialEvent;

	if (nullptr == m_pUI_Dialogue)
		m_pUI_Dialogue = static_cast<CUI_Dialogue*>(m_pGameInstance->Get_ActiveBlendUI(L"UI_Dialogue"));

	m_pUI_Dialogue->Set_Active(true);
	m_pUI_Dialogue->Set_Dialogue(m_vecDialogues[(size_t)m_eDialogueEvent]);
	m_pUI_Dialogue->Set_VoiceTag(m_vecVoiceTag[(size_t)m_eDialogueEvent]);
	m_pUI_Dialogue->Start_Dialogue(0.07f);
}

void CDialogue_Manager::Init_FirstDialogue()
{
	m_vecDialogues[FirstDialogue] =
	{
		L"안녕하세요, 모더레이터! 공식 방송이에요. 지금 라이브 스트리밍 중입니다!\n 당신이 공주님을 구출하는 모습을 수백만 명이 지켜보고 있다고요!",
		L"하지만 아무것도 못 죽여서 시청자를 지루하게 만든다면, \n 15초 뒤에 죽게 될 거예요",
		L"준비됐나요? 저 망할 로봇들을 제압하세요!"
	};
	m_vecVoiceTag[FirstDialogue] = L"FirstDialogue";
}

void CDialogue_Manager::Tick_FirstDialogue()
{
}

void CDialogue_Manager::Tick_ElevatorSansDialogue()
{
}

void CDialogue_Manager::Tick_ElevatorReturnDialogue()
{
}

void CDialogue_Manager::Tick_BossEntryDialogue()
{
}

void CDialogue_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Destroy_Instance();
}
