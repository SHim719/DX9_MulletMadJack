#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_LobbyButton final : public CUi
{
protected:
	CUi_LobbyButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_LobbyButton(const CUi_LobbyButton& rhs);
	virtual ~CUi_LobbyButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void FirstButtonRender();
	void SecondButtonRender();
	void ThirdButtonRender();

protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


private:
	void FirstInitialize();
	void SecondInitialize();
	void ThirdInitialize();
	void InitializeText();
	void TextRender();


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Initialize_PickingArea();
	void Check_Picking();
	void Texture_Switching();
	void Player_Select();
	void Set_Focusing(bool Focused, _uint Number) 
	{ m_bFocusedVec[Number] = Focused; }


private:
	vector<_bool> m_bFocusedVec;


private:
	CVIBuffer_Rect* m_pSecondVIBufferCom = { nullptr };
	CTransform* m_pSecondTransformCom = { nullptr };
	CTexture* m_pSecondTextureCom = { nullptr };
	_uint m_iSecondTextureIndex = { 0 };
	Ui_Pos_Size_Rotation m_SecondUiDesc = {};
	CVIBuffer_Rect* m_pThirdVIBufferCom = { nullptr };
	CTransform* m_pThirdTransformCom = { nullptr };
	CTexture* m_pThirdTextureCom = { nullptr };
	_uint m_iThirdTextureIndex = { 0 };
	Ui_Pos_Size_Rotation m_ThirdUiDesc = {};


private:
	vector<RECT> m_ButtonPickingVec;
	vector<CText::Text_Info> m_pTextVec;


public:
	static CUi_LobbyButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
