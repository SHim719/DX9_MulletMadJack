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


	void Set_FadeIn(_float fSpeed);
	void Set_FadeOut(_float fSpeed);
	_bool IsFinished();
	FADESTATE Get_State() { return m_eFadeState; }

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
	FADESTATE m_eFadeState = FADEOUT;
	_float m_fAlpha = 0.f;
	_float m_fSpeed = 100.f;

public:
	static CUI_FadeInOut* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
