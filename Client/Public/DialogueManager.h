#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Phone.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
enum DialogueEvent
{
	No_Dial,
	FirstDialogue,
	ElevatorSansDialogue,
	ElevatorReturnDialogue,
	ElevatoInvadeDialogue,
	ElevatoManyEnemyDialogue,
	ElevatorEndDialogue,
	BossEntryDialogue,
	DE_END
};

class CDialogue_Manager : public CBase
{
	DECLARE_SINGLETON(CDialogue_Manager)

private:
	CDialogue_Manager();
	virtual ~CDialogue_Manager() = default;

public:
	void Initialize();
	void Tick(_float fTimeDelta);

	void Start_Dialogue(DialogueEvent eDialEvent, _float fTextDelay = 0.1f, _float fDialogueDelay = 0.2f);
	_bool Check_DialogueEnd();
private:
	void Init_FirstDialogue();
	void Init_ElevatorSansDialogue();
	void Init_ElevatorReturnDialogue();
	void Init_ElevatoInvadeDialogue();
	void Init_ElevatorManyEnemyDialogue();
	void Init_ElevatorEndDialogue();
	void Init_BossEntryDialogue();

	void Tick_FirstDialogue(_float fTimeDelta);
	void Tick_ElevatorSansDialogue(_float fTimeDelta);
	void Tick_ElevatorReturnDialogue(_float fTimeDelta);
	void Tick_ElevatoInvadeDialogue();
	void Tick_ElevatorManyEnemyDialogue();
	void Tick_ElevatorEndDialogue();
	void Tick_BossEntryDialogue(_float fTimeDelta);

private:
	CGameInstance* m_pGameInstance = { nullptr };
	class CUI_Dialogue* m_pUI_Dialogue = { nullptr };

	DialogueEvent m_eDialogueEvent = No_Dial;

	vector<wstring> m_vecDialogues[DE_END];
	vector<CPhone::Phone_Texture_Desc>	m_vecPhoneTextureDescs[DE_END];
	wstring m_vecVoiceTag[DE_END] = {};

	_float m_fTimeAcc = 0.f;
public:
	virtual void Free() override;
};

END

