#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUI_FadeInOut final : public CUi
{
public:
	enum FADESTATE
	{
		FADEIN,
		FADEOUT,
		FADEOUTIN,
		FADESTATE_END,
	};

	enum FADECOLOR
	{
		BLACK,
		RED,
		WHITE,
		COLOR_END,
	};


protected:
	CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_FadeInOut(const CUI_FadeInOut& rhs);
	virtual ~CUI_FadeInOut() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void Begin_RenderState();
	virtual HRESULT Render() override;
	void End_RenderState();


	void Set_FadeIn(_float fSpeed, FADECOLOR eColor, _float fTargetAlpha = 0.f, _bool ManualOff = false);
	void Set_FadeOut(_float fSpeed, FADECOLOR eColor, _float fTargetAlpha = 255.f, _bool ManualOff = false);
	void Set_FadeOutIn(_float fSpeed, FADECOLOR eColor,
		_float fTargetFadeOutAlpha = 255.f, _float fTargetFadeInAlpha = 0.f, _bool ManualOff = false);
	void Set_FadeState(FADESTATE eState) { m_eFadeState = eState; }
	_bool IsFinished();
	FADESTATE Get_State() { return m_eFadeState; }

	_float Get_NowAlpha() { return m_fAlpha; }
protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;

private:
	CTexture* m_arrTextures[COLOR_END] = {};

	FADESTATE m_eFadeState = FADESTATE_END;
	FADECOLOR m_eFadeColor = BLACK;
	_float m_fAlpha = 0.f;
	_float m_fTargetFadeInAlpha = 0.f;
	_float m_fTargetFadeOutAlpha = 0.f;
	_float m_fSpeed = 100.f;
	_bool m_bManualOff = false; // 수동으로 끌것인가 false하면 타겟 알파값이 되면 자동으로 꺼짐.
	_bool m_bFadeOutInLoop = false; // 페이드아웃인 상태일 때 왕복하면은 Loop를 True로 설정

public:
	static CUI_FadeInOut* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
