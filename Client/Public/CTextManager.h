#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"
#include "CText.h"


BEGIN(ENGINE)
class CGameInstance;
END

BEGIN(Client)
enum class TextType
{
	Default,
	TutorialMidSpot,
	TutorialClear,
	ShopEnd,
	Sans,
	End
};
class CTextManager : public CBase
{
private:
	CTextManager();
	virtual ~CTextManager() = default;


public:
	void Initialize();
	void Initialize_TutorialClearText();
	void Initialize_TutorialMidSpotText();
	void Initialize_ShopEndText();
	void Initialize_SansText();

public:
	void Print_Text(TextType type, _uint Number);
	void Set_Pos_Text(TextType type, _uint Number, RECT Rect);
	CText::Text_Info* Get_Text(TextType type, _uint Number);
	size_t Get_Max_Size(TextType type) { return m_Text[_uint(type)].size(); }


private:
	vector<CText::Text_Info> m_Text[(_uint)TextType::End];
	CGameInstance* m_pGameInstance = { nullptr };


public:
	static CTextManager* Create();
	virtual void Free() override;
};

END