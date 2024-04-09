#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CGame_Manager : public CBase
{
DECLARE_SINGLETON(CGame_Manager)
public:
	enum class StageProgress
	{
		OnGoing,
		Clear,
		Start,
		End
	};
private:
	CGame_Manager();
	virtual ~CGame_Manager() = default;


public:
	void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
	void Tick(_float fTimeDelta);
	void Clear();
	void Start();
	void Render();


private:
	void Change_Check();
	void Reduce_ViewPort(_float fTimeDelta);
	void Extend_ViewPort(_float fTimeDelta);


public:
	void Set_StageProgress(StageProgress Progress) { m_eProgress = Progress; }


public:
	HRESULT Ready_Prototype_GameObjects();
	HRESULT Ready_Prototype_Components();
	//Custom Prototype
	HRESULT Ready_Prototype_Ui_Life();
	HRESULT Ready_Static_Texture_Prototype();
	HRESULT Ready_Clear_Texture();
	HRESULT Ready_Shop_Texture();
	HRESULT Ready_Start_Texture();
	HRESULT Ready_Active_Ui();


private:
	CGameInstance* m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	StageProgress m_eProgress = { StageProgress::End };
	StageProgress m_eOldProgress = { StageProgress::End };
	_float m_fStageClearTime = { 0.f };


private:
	D3DVIEWPORT9 m_MainViewPort;
	D3DVIEWPORT9 m_UiViewPort;
	_float m_fAdjustTime = {0};

public:
	virtual void Free() override;
};

END