#include "Client_Defines.h"
#include "CUi.h"
#include "CText.h"


BEGIN(Client)

class CUI_Dialogue final : public CUi
{
public:
	enum DialogueState
	{
		APPEAR,
		ONGOING,
		FINISH,
		DS_END,
	};

protected:
	CUI_Dialogue(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Dialogue(const CUI_Dialogue& rhs);
	virtual ~CUI_Dialogue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override {}
	virtual void Initialize_Set_Speed() override {}

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;

	
public:
	void Set_Dialogue(vector<wstring> vecDials) { m_vecDialogues = vecDials; }
	void Start_Dialogue(_float fTextDelay = 0.1f, _float fDialogueDelay = 0.2f);
	void Set_TextDelay(_float fTextDelay) { m_fTextDelay = fTextDelay; }
	void Set_DialogueDelay(_float fDelay) { m_fDialogueDelay = fDelay; }
	void Set_VoiceTag(const wstring& strVoiceTag) { m_strVoiceTag = strVoiceTag; }
	_uint Get_NowDialIdx() { return m_iNowDialogueIndex; }

private:
	void State_Appear(_float fTimeDelta);
	void State_OnGoing(_float fTimeDelta);
	void State_Finish(_float fTimeDelta);

private:
	_float3 m_OriginScale = { 650.f, 137.f, 1.f };
	_float m_fScalingSpeed = 500.f;
	_float m_fScalingY = 0.f;
private:
	CText::Text_Info info;

	vector<wstring> m_vecDialogues;
	wstring m_strDialogue = L"";
	wstring m_strVoiceTag = L"";

	_uint m_iNowDialogueIndex = 0;
	_uint m_iTextIndex = 0;		// 글자 하나씩뜨게하는 인덱스

	_float m_fTextDelay = 0.1f;
	_float m_fTextDelayAcc = 0.f;

	_float m_fDialogueDelay = 3.f;
	_float m_fDialogueDelayAcc = 0.f;
	RECT   m_tTextRect;

	DialogueState m_eState = APPEAR;
public:
	static CUI_Dialogue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END