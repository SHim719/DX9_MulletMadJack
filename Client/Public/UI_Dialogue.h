#pragma once

#include "Client_Defines.h"
#include "CUi.h"
#include "CText.h"
#include "Phone.h"


BEGIN(Client)

class CUI_Dialogue final : public CUi
{
public:
	enum DialogueState
	{
		APPEAR,
		ONGOING,
		DELAY,
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
	void Set_PhoneTexDescs(vector<CPhone::Phone_Texture_Desc> vecDescs) { m_vecPhoneTexDescs = vecDescs;}

	_uint Get_NowDialIdx() { return m_iNowDialogueIndex; }
	_bool Is_DialogueEnd() { return m_eState == DS_END; }

	void Set_TextEndBind(function<void()> pFunc) { m_pTextBindFunc = pFunc; }
	void Set_DialogueEndBind(function<void()> pFunc) { m_pDialogueEndFunc = pFunc; }

	void SetState_OnGoing();
	void SetState_Delay();
	void SetState_Finish();
private:
	void State_Appear(_float fTimeDelta);
	void State_OnGoing(_float fTimeDelta);
	void State_Finish(_float fTimeDelta);
	void State_Delay(_float fTimeDelta);

	
private:
	//_float3 m_OriginScale = { 650.f, 137.f, 1.f };
	_float3 m_OriginScale = { 730.f, 100.f, 1.f };
	_float m_fScalingSpeed = 500.f;
	_float m_fScalingY = 0.f;

private:
	class CPhone* m_pUI_Phone = { nullptr };
	CText::Text_Info info;

	vector<CPhone::Phone_Texture_Desc>	m_vecPhoneTexDescs;
	vector<wstring> m_vecDialogues;

	wstring m_strDialogue = L"";
	wstring m_strVoiceTag = L"";

	_uint m_iNowDialogueIndex = 0;
	_uint m_iTextIndex = 0;		// ���� �ϳ����߰��ϴ� �ε���

	_float m_fTextDelay = 0.1f;
	_float m_fTextDelayAcc = 0.f;

	_float m_fDialogueDelay = 3.f;
	_float m_fDialogueDelayAcc = 0.f;
	RECT   m_tTextRect;

	DialogueState m_eState = DS_END;

private:
	function<void()> m_pTextBindFunc = {nullptr};
	function<void()> m_pDialogueEndFunc = {nullptr};
public:
	static CUI_Dialogue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END