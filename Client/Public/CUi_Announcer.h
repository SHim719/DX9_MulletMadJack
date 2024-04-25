#pragma once
#include "Client_Defines.h"
#include "CUi.h"
#include "CTextManager.h"

BEGIN(Client)

class CUi_Announcer final : public CUi
{
protected:
	CUi_Announcer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Announcer(const CUi_Announcer& rhs);
	virtual ~CUi_Announcer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	virtual void Enter(bool _Enter) override;


private:
	void Move(_float fTimeDelta);
	void Texture_Switching(_float fTimeDelta);
	void Announcer_Texture_Switching();
	void TextRender();
	void Set_PrintTextNumber(TextType type, _uint Length);
	void Set_Pos_TextBackGround();
	void Cal_PrintTextNumber(_float fTimeDelta);

	void Set_AnnouncerTexture();
	void Set_TextureIndexTutorialMidSpot(_uint Number);
	void Set_TextureIndexTutorialClear(_uint Number);
	void Set_TextureIndexShopEnd(_uint Number);

private:
	_float m_fUniqueTextureTime = { 0.f };
	_float m_fAnnouncerTextureTime = { 0 };
	CTexture* m_pUniqueTexture = { nullptr };
	_uint m_iUniqueTextureIndex = { 0 };
	_float m_fPrintTextGap = { 0 };
	_uint m_iPrintTextNumber = { 0 };
	_uint m_iTextureIndexMin = { 0 };
	_uint m_iTextureIndexMax = { 0 };
	_uint m_OldTextNumber = { 0 };
	TextType m_OldType = { TextType::Default };
	bool m_bTextPrint = { false };
	bool m_bTextureIndexChanged = { false };
	_bool m_bAnnounced = { false };
	class CGame_Manager* m_pGameManager = { nullptr };
	class CText_BackGround* m_pTextBackGround = { nullptr };


public:
	static CUi_Announcer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
