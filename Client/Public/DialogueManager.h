#pragma once

#include "Client_Defines.h"
#include "Base.h"

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

	void Start_Dialogue(DialogueEvent eDialEvent);
private:
	void Init_FirstDialogue();

	void Tick_FirstDialogue();
	void Tick_ElevatorSansDialogue();
	void Tick_ElevatorReturnDialogue();
	void Tick_BossEntryDialogue();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	class CUI_Dialogue* m_pUI_Dialogue = { nullptr };

	DialogueEvent m_eDialogueEvent = No_Dial;

	vector<wstring> m_vecDialogues[DE_END];
	wstring m_vecVoiceTag[DE_END] = {};
public:
	virtual void Free() override;
};

END

