#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CTextManager.h"


BEGIN(Client)
enum class StageProgress
{
	OnGoing,
	StageClear,
	TutorialMidSpot,
	TutorialClear,
	Shopping,
	ShopEnd,
	Changing,
	Start,
	End
};
class CGame_Manager : public CBase
{
DECLARE_SINGLETON(CGame_Manager)

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
	void Adjust_ViewPort(_float fTimeDelta);
	void Call_Shop(_float fTimeDelta);
	void Cal_Change_Time(_float fTimeDelta);
	void Cal_StageClear_Time(_float fTimeDelta);


public:
	void Set_StageProgress(StageProgress Progress) { m_eProgress = Progress; }


public:
	void Player_UpGrade(void* pArg);
	_float Get_StageClearTime() { return m_fStageClearTime; }
	StageProgress Get_StageProgress() const { return m_eProgress; }

public:
	HRESULT Ready_Prototype_GameObjects();
	HRESULT Ready_Prototype_Components();
	void Ready_Loading_BackGroundTextureAndUi(); // need Ready_Prototype_Components fuction


public:
	void Print_Text(TextType type, _uint Number);
	void Set_Pos_Text(TextType type, _uint Number, RECT Rect);
	void Initialize_TextManager();

	void Add_TextNumber(TextType type) { ++m_iTextPrintOrder[_uint(type)]; }

	_uint Get_TextNumber(TextType type) const
	{ return m_iTextPrintOrder[_uint(type)]; }

	size_t Get_MaxSize(TextType type) { return m_pTextManager->Get_Max_Size(type); }
private:
	CGameInstance* m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	StageProgress m_eProgress = { StageProgress::End };
	StageProgress m_eOldProgress = { StageProgress::End };


private:
	D3DVIEWPORT9 m_MainViewPort;
	D3DVIEWPORT9 m_UiViewPort;
	_float m_fShopTime = {0.5f};
	_float m_fStageClearTime = { 32.14f };
	_float m_fChangeTime = { 3.5f };


private:
	CTextManager* m_pTextManager = { nullptr };
	_uint m_iTextPrintOrder[_uint(TextType::End)] = {};

public:
	virtual void Free() override;
};

END