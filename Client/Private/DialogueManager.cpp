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
	Init_ElevatorSansDialogue();
	Init_ElevatorReturnDialogue();
	Init_ElevatoInvadeDialogue();
}

void CDialogue_Manager::Tick(_float fTimeDelta)
{
	switch (m_eDialogueEvent)
	{
	case No_Dial:
		break;
	case FirstDialogue:
		Tick_FirstDialogue(fTimeDelta);
		break;
	case ElevatorSansDialogue:
		Tick_ElevatorSansDialogue(fTimeDelta);
		break;
	case ElevatorReturnDialogue:
		Tick_ElevatorReturnDialogue(fTimeDelta);
		break;
	case ElevatoInvadeDialogue:
		Tick_ElevatorReturnDialogue(fTimeDelta);
		break;
	case BossEntryDialogue:
		Tick_BossEntryDialogue(fTimeDelta);
		break;
	}

}

void CDialogue_Manager::Start_Dialogue(DialogueEvent eDialEvent, _float fTextDelay, _float fDialogueDelay)
{
	m_eDialogueEvent = eDialEvent;

	if (nullptr == m_pUI_Dialogue)
		m_pUI_Dialogue = static_cast<CUI_Dialogue*>(m_pGameInstance->Get_ActiveBlendUI(L"UI_Dialogue"));

	m_pUI_Dialogue->Set_Active(true);
	m_pUI_Dialogue->Start_Dialogue(fTextDelay, fDialogueDelay);
	m_pUI_Dialogue->Set_Dialogue(m_vecDialogues[(size_t)m_eDialogueEvent]);
	m_pUI_Dialogue->Set_PhoneTexDescs(m_vecPhoneTextureDescs[(size_t)m_eDialogueEvent]);
	m_pUI_Dialogue->Set_VoiceTag(m_vecVoiceTag[(size_t)m_eDialogueEvent]);
}

_bool CDialogue_Manager::Check_DialogueEnd()
{
	if (No_Dial == m_eDialogueEvent)
		return true;

	return false;
}

void CDialogue_Manager::Init_FirstDialogue()
{
	m_vecDialogues[FirstDialogue] =
	{
		L"�ȳ��ϼ���, ���������! ���� ����̿���. ���� ���̺� ��Ʈ���� ���Դϴ�!\n ����� ���ִ��� �����ϴ� ����� ���鸸 ���� ���Ѻ��� �ִٰ��!",
		L"������ �ƹ��͵� �� �׿��� ��û�ڸ� �����ϰ� ����ٸ�, \n 15�� �ڿ� �װ� �� �ſ���",
		L"�غ�Ƴ���? �� ���� �κ����� �����ϼ���!"
	};


	m_vecPhoneTextureDescs[FirstDialogue] =
	{
		{ FaceType::Announcer, 0.1f, 0, 1},
		{ FaceType::Announcer, 0.1f, 2, 3},
		{ FaceType::Announcer, 0.1f, 6, 7},
		{ FaceType::Sans, 0.1f, }
	};

	m_vecVoiceTag[FirstDialogue] = L"FirstDialogue";
}

void CDialogue_Manager::Init_ElevatorSansDialogue()
{
	m_vecDialogues[ElevatorSansDialogue] =
	{
		L"����� Ȱ�࿡ ���̺� ��û�ڵ��� �⻵�ϰ� �־��!",
		L"�� ���� ���� �������Դϴ�! �����ϼ���!",
		L"....����?? ..��..����.. ��..��.....",
		L"��������."
	};


	m_vecPhoneTextureDescs[ElevatorSansDialogue] =
	{
		{ FaceType::Announcer, 0.1f, 0, 1},
		{ FaceType::Announcer, 0.1f, 6, 7},
		{ FaceType::Noise, 0.1f, 0, 1},
		{ FaceType::Sans, 0.5f, 0, 4}
	};

	m_vecVoiceTag[ElevatorSansDialogue] = L"ElevatorSansDialogue";

}

void CDialogue_Manager::Init_ElevatorReturnDialogue()
{
	m_vecDialogues[ElevatorReturnDialogue] =
	{
		L"..... ��..��..��..��?",
		L"¥��! ����� �ٽ� ����ƽ��ϴ�.",
		L"����� ���� ���� ���� �� ������ �����̳���?"
	};


	m_vecPhoneTextureDescs[ElevatorReturnDialogue] =
	{
		{ FaceType::Noise, 0.1f, 0, 1},
		{ FaceType::Announcer, 0.1f, 0, 1},
		{ FaceType::Announcer, 0.1f, 2, 3},
	};
}

void CDialogue_Manager::Init_ElevatoInvadeDialogue()
{
	m_vecDialogues[ElevatoInvadeDialogue] =
	{
		L"����� ��ġ�� �߰��Ǿ����!!\n�κ����� ħ���ؿ��� �ֽ��ϴ�!",
		L"�ݵ�� ��Ƴ�������!!!"
	};


	m_vecPhoneTextureDescs[ElevatoInvadeDialogue] =
	{
		{ FaceType::Announcer, 0.1f, 6, 7},
		{ FaceType::Announcer, 0.1f, 6, 7},
	};

}

void CDialogue_Manager::Tick_FirstDialogue(_float fTimeDelta)
{
	if (m_pUI_Dialogue->Is_DialogueEnd())
		m_eDialogueEvent = No_Dial;
}

void CDialogue_Manager::Tick_ElevatorSansDialogue(_float fTimeDelta)
{
	if (m_pUI_Dialogue->Is_DialogueEnd())
		m_eDialogueEvent = No_Dial;
	
	if (3 == m_pUI_Dialogue->Get_NowDialIdx())
	{
		function<void()> playSansVoice = [this]()->void
			{
				m_pGameInstance->Play(L"Sans_Voice", false);
			};

		m_pUI_Dialogue->Set_TextEndBind(playSansVoice);
		m_pUI_Dialogue->Set_TextDelay(1.f);
	}
		
}

void CDialogue_Manager::Tick_ElevatorReturnDialogue(_float fTimeDelta)
{
	if (m_pUI_Dialogue->Is_DialogueEnd())
		m_eDialogueEvent = No_Dial;
}

void CDialogue_Manager::Tick_ElevatoInvadeDialogue()
{
	if (m_pUI_Dialogue->Is_DialogueEnd())
		m_eDialogueEvent = No_Dial;
}

void CDialogue_Manager::Tick_BossEntryDialogue(_float fTimeDelta)
{
	if (m_pUI_Dialogue->Is_DialogueEnd())
		m_eDialogueEvent = No_Dial;
}

void CDialogue_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Destroy_Instance();
}
